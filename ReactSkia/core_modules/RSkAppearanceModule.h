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

namespace facebook {
namespace react {

using namespace std;
using namespace rnsplugin;

#undef RNS_APPEARENCE_TEST_DUMMY_EVENT // Scehdule a 'appearenceChanged' event 2 seconds after calling getColorScheme API.

class RSkAppearanceModule : public react::RSkEventEmitter {
 public:
  RSkAppearanceModule(
    const std::string &name,
    std::shared_ptr<react::CallInvoker> jsInvoker,
    react::Instance *bridgeInstance);
  ~RSkAppearanceModule(){};

  static jsi::Value getColorSchemeWrapper(
    jsi::Runtime &rt,
    TurboModule &turboModule,
    const jsi::Value *args,
    size_t count);

  void startObserving()override;
  void stopObserving()override;

  std::vector<std::string> supportedEvents() override {
    return {"appearanceChanged"};
  }

 private:
  // RNS Plugin Factory and interface
  RnsPluginFactory *pluginFactory_;
  std::unique_ptr<RNSPlatformManagerInterface> platformManagerHandle_;

  void lazyInit();
  jsi::Value getColorScheme(jsi::Runtime &rt);

  //CallBackClient for Events
  class AppearanceCallBackClient : public RNSPlatformManagerInterface::CallbackClient {
   public:
    AppearanceCallBackClient(RSkAppearanceModule& appearanceModule);
    ~AppearanceCallBackClient(){}

    void onSafeAreaInsetsDidChange() override { };
#ifdef RNS_APPEARENCE_TEST_DUMMY_EVENT // Scehdule a 'appearenceChanged' event 2 seconds after calling getColorScheme API.
    void onAppearanceChange(string newAppearance);
#endif

   private:
    RSkAppearanceModule& appearanceModule_;
  };
  AppearanceCallBackClient appearanceCallBackClient_;
};

} // react
} // facebook
