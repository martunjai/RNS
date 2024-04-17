#!/usr/bin/python

# Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

import json
import sys
import io
import os

from RSkFileWrite import *

assetMap = {}

# Copyright year
copyrightYear = '1994-2023'

# Contains all the source file names
ModulefileNames = []

# Function to generate Module.cpp/,h template file
def generateThirdPartyNativeModuleSource(skiaSourcePath, moduleName, withEventEmitter):
  global ModulefileNames
  print('Generating Template source for...')
  if moduleName.endswith('Module'):
    className = moduleName
    exportMacro = 'RNS_EXPORT_MODULE_WITHOUT_SUFFIX(' + moduleName + ')'
  else:
    className = moduleName+'Module'
    exportMacro = 'RNS_EXPORT_MODULE(' + moduleName + ')'

  # append module/class name in a global list
  ModulefileNames.append(className)

  if withEventEmitter == True:
    inhertingClass = 'NativeEventEmitterModule'
    emitterHeader = '#include "ReactSkia/LegacyNativeModules/NativeEventEmitter.h"'
    emitterOverrides = 'void startObserving() override{};\n  void stopObserving() override{};'
    emitterInitializer =': NativeEventEmitterModule(nullptr)'
    emitterGetMethod ='''std::vector<Method> emitterMethodsVector  = NativeEventEmitterModule::getMethods();
  std::vector<Method> supportedMethodsVector = {
  };
  supportedMethodsVector.insert(supportedMethodsVector.end(), emitterMethodsVector.begin(), emitterMethodsVector.end());
  return supportedMethodsVector;'''
  else:
    inhertingClass = 'module::CxxModule'
    emitterHeader = ''
    emitterOverrides = ''
    emitterInitializer =''
    emitterGetMethod ='''return {
  };'''

  # Source template
  TPNativeModuleCppTemplate = '''/*
 * Copyright (C) '''+copyrightYear+''' OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ReactSkia/utils/RnsLog.h"
#include <cxxreact/JsArgumentHelpers.h>

#include "'''+className+'''.h"

using namespace folly;

namespace facebook {
namespace xplat {

'''+className+'''::'''+ className + '''() ''' + emitterInitializer + ''' { }

auto '''+className+'''::getConstants() -> std::map<std::string, folly::dynamic> {
  return {};
}

std::string '''+className+'''::getName() {
  return "'''+moduleName+'''";
}

auto '''+className+'''::getMethods() -> std::vector<Method> {
  '''+emitterGetMethod+'''
}

#ifdef __cplusplus
extern "C" {
#endif
''' + exportMacro +'''
#ifdef __cplusplus
}
#endif

} // namespace xplat
} // namespace facebook

'''

  # header template
  TPNativeModuleHTemplate = '''/*
 * Copyright (C) '''+copyrightYear+''' OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cxxreact/CxxModule.h>
'''+emitterHeader+'''
#include "ReactSkia/utils/RnsUtils.h"

using namespace std;

namespace facebook {
namespace xplat {

class '''+className+''' : public ''' + inhertingClass + ''' {
 public:
  '''+className+'''();
  std::map<std::string, folly::dynamic> getConstants() override;
  std::vector<Method> getMethods() override;
  std::string getName() override;

 private:
  '''+emitterOverrides+'''
};
} // namespace xplat
} // namespace facebook
'''

  # Writing to .cpp file
  cppFilePath = skiaSourcePath + '/'+ className + '.cpp'
  writeContentToFile(cppFilePath, TPNativeModuleCppTemplate, True)

  # Writing to .h file
  headerFilePath = skiaSourcePath + '/'+ className + '.h'
  writeContentToFile(headerFilePath, TPNativeModuleHTemplate, True)
# End of function generateThirdPartyNativeModuleSurce()

# Function to generate Module.Cpp template file
def generateThirdPartyNativeModuleBuildGn(skiaSourcePath, moduleList, libName, withEventEmitter):

  TPNativeModuleBuildGnTemplateSrcList = ''
  for fileName in ModulefileNames:
    TPNativeModuleBuildGnTemplateSrcList +=  '    "' + fileName + '.h",\n'
    TPNativeModuleBuildGnTemplateSrcList +=  '    "' + fileName + '.cpp",\n'

  if withEventEmitter == True:
    emitterDeps ='''"//react-native/ReactCommon/runtimeexecutor",
    "//react-native/ReactCommon/jsi:jsi",
    "//react-native/ReactCommon/callinvoker",'''
  else:
    emitterDeps =''

  # BUILD.gn template
  TPNativeModuleBuildGnTemplate = '''# '''+libName+''' BUILD.gn

import("//react-native/common.gni")

config("Lib'''+libName+'''_config") {
  cflags_cc = [ "-std=c++17"]
  cflags_cc += [
    "-Wno-extra-semi",
    "-Wno-sign-compare",
    "-Wno-header-hygiene",
  ]

  include_dirs = [ ".",
                  "//react-native/ReactCommon/",
                ]
}

rns_cxx_tp_component("'''+libName+'''") {

  # RS Native module and its dependencies
  sources = [
'''+TPNativeModuleBuildGnTemplateSrcList+'''
  ]

  public_configs = [ ":Lib'''+libName+'''_config" ]
  public_configs += [ "//build/secondary/folly:folly_config" ]
  deps = [
    "//folly",
    "//skia",
    '''+emitterDeps+'''
  ]

  with_exceptions = true
  with_rtti = true
}
'''

  # Writing to BUILD.gn file
  buildGnPath = skiaSourcePath + '/' + 'BUILD.gn'
  writeContentToFile(buildGnPath, TPNativeModuleBuildGnTemplate, True)

# End of function generateThirdPartyNativeModuleBuildGn()

# Function to parse Rsk.project file and create a package info object
def parseRskProjectFile(rskProjPath):
  if not os.path.exists(rskProjPath):
    print ("Warning :" + rskProjPath + ' not found')
    return

  rskProjFileHandle = open(rskProjPath)
  codegenData = json.load(rskProjFileHandle)

  if 'codegenConfig' not in codegenData:
    rskProjFileHandle.close()
    return

  codegenConfig = codegenData['codegenConfig']
  libName = codegenConfig['name'] if 'name' in codegenConfig else 'UnknownPkgName'
  libType = codegenConfig['type'] if 'type' in codegenConfig else 'UnknownPkgType'

  classNames = []
  moduleNames = []
  if 'skia' in codegenConfig:
    classNames =  codegenConfig['skia']['codegenNativeComponentNativeProps'] if 'codegenNativeComponentNativeProps' in codegenConfig['skia'] else []
    moduleNames = codegenConfig['skia']['TurboModuleRegistryGetEnforcingSpec'] if 'TurboModuleRegistryGetEnforcingSpec' in codegenConfig['skia'] else []

  withEmitter = sys.argv[2].lower()

  if withEmitter == 'true' or withEmitter == 'yes':
    boolValue = True
  else:
    boolValue = False

  # Generate .cpp/.h for each module
  for module in moduleNames:
    generateThirdPartyNativeModuleSource(sys.argv[1], module, boolValue)

  # Generate BUILD.gn for building all the modules
  generateThirdPartyNativeModuleBuildGn(sys.argv[1], moduleNames, libName, boolValue)

  rskProjFileHandle.close()
  return

# End of function parseRskProjectFile()

# Main function
def main():

  print('Scanning ' + sys.argv[1] + ' for Rsk.project, template source will be generated in same directory')

  modulesSkiaSourcePath = sys.argv[1];

  rskProjPath = modulesSkiaSourcePath + 'Rsk.project'
  parseRskProjectFile(rskProjPath)

if __name__== "__main__":
  main()
