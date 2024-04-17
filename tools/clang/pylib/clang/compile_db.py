#!/usr/bin/env python
# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

from __future__ import print_function

import json
import os
import re
import sys
import subprocess


_RSP_RE = re.compile(r' (@(.+?\.rsp)) ')
_CMD_LINE_RE = re.compile(
    r'^(?P<gomacc>.*gomacc\.exe"?\s+)?(?P<clang>\S*clang\S*)\s+(?P<args>.*)$')
_debugging = False


def _ProcessCommand(command):
  """Removes gomacc.exe and inserts --driver-mode=cl as the first argument.

  Note that we deliberately don't use shlex.split here, because it doesn't work
  predictably for Windows commands (specifically, it doesn't parse args the same
  way that Clang does on Windows).

  Instead, we just use a regex, with the simplifying assumption that the path to
  clang-cl.exe contains no spaces.
  """
  # If the driver mode is not already set then define it. Driver mode is
  # automatically included in the compile db by clang starting with release
  # 9.0.0.
  if "--driver_mode" in command:
    driver_mode = ""
  # Only specify for Windows. Other platforms do fine without it.
  elif sys.platform == 'win32':
    driver_mode = '--driver-mode=cl'

  match = _CMD_LINE_RE.search(command)
  if match:
    match_dict = match.groupdict()
    command = ' '.join(
        [match_dict['clang'], driver_mode, match_dict['args']])
  elif _debugging:
    print('Compile command didn\'t match expected regex!')
    print('Command:', command)
    print('Regex:', _CMD_LINE_RE.pattern)

  # Remove some blocklisted arguments. These are VisualStudio specific arguments
  # not recognized or used by clangd. They only suppress or activate graphical
  # output anyway.
  blocklisted_arguments = ['/nologo', '/showIncludes']
  command_parts = filter(lambda arg: arg not in blocklisted_arguments,
    command.split())

  return " ".join(command_parts)


def _ProcessEntry(entry):
  """Transforms one entry in a Windows compile db to be clang-tool friendly."""
  entry['command'] = _ProcessCommand(entry['command'])

  # Expand the contents of the response file, if any.
  # http://llvm.org/bugs/show_bug.cgi?id=21634
  try:
    match = _RSP_RE.search(entry['command'])
    if match:
      rsp_path = os.path.join(entry['directory'], match.group(2))
      rsp_contents = open(rsp_path).read()
      entry['command'] = ''.join([
          entry['command'][:match.start(1)],
          rsp_contents,
          entry['command'][match.end(1):]])
  except IOError:
    if _debugging:
      print('Couldn\'t read response file for %s' % entry['file'])

  return entry


def ProcessCompileDatabaseIfNeeded(compile_db):
  """Make the compile db generated by ninja on Windows more clang-tool friendly.

  Args:
    compile_db: The compile database parsed as a Python dictionary.

  Returns:
    A postprocessed compile db that clang tooling can use.
  """
  # TODO(dcheng): Ideally this would check target_os... but not sure there's an
  # easy way to do that, and (for now) cross-compiles don't work without custom
  # patches anyway.
  if sys.platform != 'win32':
    return compile_db

  if _debugging:
    print('Read in %d entries from the compile db' % len(compile_db))
  compile_db = [_ProcessEntry(e) for e in compile_db]
  original_length = len(compile_db)

  # Filter out NaCl stuff. The clang tooling chokes on them.
  # TODO(dcheng): This doesn't appear to do anything anymore, remove?
  compile_db = [e for e in compile_db if '_nacl.cc.pdb' not in e['command']
      and '_nacl_win64.cc.pdb' not in e['command']]
  if _debugging:
    print('Filtered out %d entries...' % (original_length - len(compile_db)))

  # TODO(dcheng): Also filter out multiple commands for the same file. Not sure
  # how that happens, but apparently it's an issue on Windows.
  return compile_db


def GetNinjaPath():
  ninja_executable = 'ninja.exe' if sys.platform == 'win32' else 'ninja'
  return os.path.join(
      os.path.dirname(os.path.realpath(__file__)),
        '..', '..', '..', '..', 'third_party', 'depot_tools', ninja_executable)


# FIXME: This really should be a build target, rather than generated at runtime.
def GenerateWithNinja(path, targets=[]):
  """Generates a compile database using ninja.

  Args:
    path: The build directory to generate a compile database for.
    targets: Additional targets to pass to ninja.

  Returns:
    List of the contents of the compile database.
  """
  # TODO(dcheng): Ensure that clang is enabled somehow.

  # First, generate the compile database.
  json_compile_db = subprocess.check_output(
      [GetNinjaPath(), '-C', path] + targets +
      ['-t', 'compdb', 'cc', 'cxx', 'objc', 'objcxx'])
  return json.loads(json_compile_db)


def Read(path):
  """Reads a compile database into memory.

  Args:
    path: Directory that contains the compile database.
  """
  with open(os.path.join(path, 'compile_commands.json'), 'rb') as db:
    return json.load(db)
