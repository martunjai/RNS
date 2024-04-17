/*
 * Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ReactSkia/core_modules/RSkAppearanceModule.h"
#include "ReactSkia/utils/RnsLog.h"

#ifdef RNS_APPEARENCE_TEST_DUMMY_EVENT // Scehdule a 'appearenceChanged' event 2 seconds after calling getColorScheme API.
#include "rns_shell/platform/linux/TaskLoop.h"
#endif

namespace facebook {
namespace react {

RNSP_EXPORT RSkAppearanceModule::AppearanceCallBackClient::AppearanceCallBackClient(RSkAppearanceModule& appearanceModule)
  : appearanceModule_(appearanceModule){ (void)appearanceModule_;}

RSkAppearanceModule::RSkAppearanceModule(
  const std::string &name,
  std::shared_ptr<react::CallInvoker> jsInvoker,
  react::Instance *bridgeInstance)
    : RSkEventEmitter(name, jsInvoker, bridgeInstance),
      pluginFactory_(new RnsPluginFactory),
      appearanceCallBackClient_(*this) {
  methodMap_["getColorScheme"] = MethodMetadata{0, getColorSchemeWrapper};
}

jsi::Value RSkAppearanceModule::getColorSchemeWrapper(
  jsi::Runtime &rt,
  TurboModule &turboModule,
  const jsi::Value *args,
  size_t count) {

  if (count != 0) {
    return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkAppearanceModule &>(turboModule);
  return self.getColorScheme(rt);
}

void RSkAppearanceModule::lazyInit() {
  if(platformManagerHandle_ == nullptr) {
    RNS_LOG_DEBUG("Creating platform handle from Plugin Factory");
    platformManagerHandle_ = pluginFactory_->createPlatformManagerHandle(appearanceCallBackClient_);
    if(platformManagerHandle_ == nullptr) {
      RNS_LOG_ERROR("Could not get Platform handle from RNS platform Plugin");
    } else {
      RNS_LOG_DEBUG(this << " : RNS Platform Plugin Loaded with Platform interface : " << platformManagerHandle_.get() << " : Thread : " << std::this_thread::get_id());
    }
  }
}

jsi::Value RSkAppearanceModule::getColorScheme(jsi::Runtime &rt) {
  lazyInit();
  shared_ptr<RNSPlatformManagerInterface::PlatformDevice> device = nullptr;
  if(platformManagerHandle_ &&
    (device = platformManagerHandle_->currentDevice())) {
      RNS_LOG_TODO("Use Platform plugin API to get colorScheme");
      //string colorSCheme = platformManager->getDeviceColorScheme();
      //return jsi::valueFromDynamic(rt, colorSCheme);
#ifdef RNS_APPEARENCE_TEST_DUMMY_EVENT // Scehdule a 'appearenceChanged' event 2 seconds after calling getColorScheme API.
      static string colorSCheme = "light";
      string newScheme = (colorSCheme == "light") ? "dark" : "light" ;
      RnsShell::TaskLoop::main().scheduleDispatch([this, newScheme]() {appearanceCallBackClient_.onAppearanceChange(newScheme);}, 2000);
      colorSCheme = newScheme;
#endif
  }

  return jsi::valueFromDynamic(rt, "light"); // default
}

void RSkAppearanceModule::startObserving() {RNS_LOG_NOT_IMPL;}
void RSkAppearanceModule::stopObserving() {RNS_LOG_NOT_IMPL;}

#ifdef RNS_APPEARENCE_TEST_DUMMY_EVENT // Scehdule a 'appearenceChanged' event 2 seconds after calling getColorScheme API.
void RSkAppearanceModule::AppearanceCallBackClient::onAppearanceChange(string newAppearance) {
  folly::dynamic appearance = folly::dynamic::object();
  appearance["colorScheme"] = newAppearance;
  RNS_LOG_INFO("onAppearanceChange : " << newAppearance);
  appearanceModule_.sendEventWithName("appearanceChanged", folly::dynamic(appearance));
}
#endif

} // react
} // facebook

