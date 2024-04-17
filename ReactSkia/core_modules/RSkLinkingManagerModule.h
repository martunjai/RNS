/*
* Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "jsi/JSIDynamic.h"
#include "ReactSkia/core_modules/RSkEventEmitter.h"
#include "ReactSkia/JSITurboModuleManager.h"
#include "ReactSkia/pluginfactory/RnsPlugin.h"
#include "ReactSkia/pluginfactory/RnsPluginFactory.h"

#undef RNS_LINKING_TEST_DUMMY_EVENT // emit dummy 'url' event when calling openURL API.

namespace facebook {
namespace react {

using namespace std;
using namespace rnsplugin;

class RSkLinkingManagerModule : public react::RSkEventEmitter {
 public:
  RSkLinkingManagerModule(
    const std::string &name,
    std::shared_ptr<react::CallInvoker> jsInvoker,
    react::Instance *bridgeInstance);
  ~RSkLinkingManagerModule(){};

  static jsi::Value getColorSchemeWrapper(
    jsi::Runtime &rt,
    TurboModule &turboModule,
    const jsi::Value *args,
    size_t count);

  static jsi::Value getInitialURLWrapper(
    jsi::Runtime &rt,
    TurboModule &turboModule,
    const jsi::Value *args,
    size_t count);
  static jsi::Value canOpenURLWrapper(
    jsi::Runtime &rt,
    TurboModule &turboModule,
    const jsi::Value *args,
    size_t count);
  static jsi::Value openURLWrapper(
    jsi::Runtime &rt,
    TurboModule &turboModule,
    const jsi::Value *args,
    size_t count);
  static jsi::Value openSettingsWrapper(
    jsi::Runtime &rt,
    TurboModule &turboModule,
    const jsi::Value *args,
    size_t count);

  void startObserving()override;
  void stopObserving()override;

  std::vector<std::string> supportedEvents() override {
    return {"url"};
  }

 private:

  void lazyInit();
  jsi::Value getInitialURL(jsi::Runtime &rt);
  jsi::Value canOpenURL(jsi::Runtime &rt, std::string url);
  jsi::Value openURL(jsi::Runtime &rt, std::string url);
  jsi::Value openSettings(jsi::Runtime &rt);

};

} // react
} // facebook
