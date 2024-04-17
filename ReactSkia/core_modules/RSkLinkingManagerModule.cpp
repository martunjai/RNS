/*
 * Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "react/nativemodule/core/ReactCommon/TurboModuleUtils.h"

#include "ReactSkia/core_modules/RSkLinkingManagerModule.h"
#include "ReactSkia/utils/RnsLog.h"

namespace facebook {
namespace react {

RSkLinkingManagerModule::RSkLinkingManagerModule(
  const std::string &name,
  std::shared_ptr<react::CallInvoker> jsInvoker,
  react::Instance *bridgeInstance)
    : RSkEventEmitter(name, jsInvoker, bridgeInstance) {
  methodMap_["getInitialURL"] = MethodMetadata {0, getInitialURLWrapper};
  methodMap_["canOpenURL"] = MethodMetadata {1, canOpenURLWrapper};
  methodMap_["openURL"] = MethodMetadata {1, openURLWrapper};
  methodMap_["openSettings"] = MethodMetadata {0, openSettingsWrapper};
}

jsi::Value RSkLinkingManagerModule::getInitialURLWrapper(
  jsi::Runtime &rt,
  TurboModule &turboModule,
  const jsi::Value *args,
  size_t count) {

  if (count != 0) {
    return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkLinkingManagerModule &>(turboModule);
  return self.getInitialURL(rt);
}

jsi::Value RSkLinkingManagerModule::canOpenURLWrapper(
  jsi::Runtime &rt,
  TurboModule &turboModule,
  const jsi::Value *args,
  size_t count) {

  if (count != 1) {
    return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkLinkingManagerModule &>(turboModule);
  return self.canOpenURL(rt, args[0].getString(rt).utf8(rt));
}


jsi::Value RSkLinkingManagerModule::openURLWrapper(
  jsi::Runtime &rt,
  TurboModule &turboModule,
  const jsi::Value *args,
  size_t count) {

  if (count != 1) {
    return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkLinkingManagerModule &>(turboModule);
  return self.openURL(rt, args[0].getString(rt).utf8(rt));
}

jsi::Value RSkLinkingManagerModule::openSettingsWrapper(
  jsi::Runtime &rt,
  TurboModule &turboModule,
  const jsi::Value *args,
  size_t count) {

  if (count != 0) {
    return jsi::Value::undefined();
  }
  auto &self = static_cast<RSkLinkingManagerModule &>(turboModule);
  return self.openSettings(rt);
}

jsi::Value RSkLinkingManagerModule::getInitialURL(jsi::Runtime &rt) {
  RNS_LOG_DEBUG("[" << std::this_thread::get_id() << "] Get initial URL");
  RNS_LOG_NOT_IMPL;
  return createPromiseAsJSIValue(
    rt, [](jsi::Runtime &rt2, std::shared_ptr<Promise> promise) { // NOTE: This is called in  JS Thread
        promise->resolve(jsi::String::createFromUtf8(rt2, "SimpleViewApp.bundle"));
  });
}

jsi::Value RSkLinkingManagerModule::canOpenURL(jsi::Runtime &rt, std::string url) {
  RNS_LOG_DEBUG("[" << std::this_thread::get_id() << "] Can Open URL ? : " << url);
  RNS_LOG_NOT_IMPL;
  return createPromiseAsJSIValue(
    rt, [url](jsi::Runtime &rt2, std::shared_ptr<Promise> promise) { // NOTE: This is called in  JS Thread
      if(url.empty() == true)
        promise->resolve(jsi::String::createFromUtf8(rt2, "NO"));
      else {
        promise->reject("OpenURL Not Implemented, so URLs cannot be opened");
      }
  });
}

jsi::Value RSkLinkingManagerModule::openURL(jsi::Runtime &rt, std::string url) {
  RNS_LOG_DEBUG("[" << std::this_thread::get_id() << "] Open URL " << url);
  RNS_LOG_NOT_IMPL;

#ifdef RNS_LINKING_TEST_DUMMY_EVENT // emit 'url' event when calling openURL API.
  folly::dynamic urlE = folly::dynamic::object();
  urlE["url"] = "http://www.google.com";
  sendEventWithName("url", folly::dynamic(urlE));
#endif

  return createPromiseAsJSIValue(
    rt, [url](jsi::Runtime &rt2, std::shared_ptr<Promise> promise) { // NOTE: This is called in  JS Thread
      promise->reject("OpenURL Not Implemented. Unable to open URL");
  });
}

jsi::Value RSkLinkingManagerModule::openSettings(jsi::Runtime &rt) {
  RNS_LOG_DEBUG("[" << std::this_thread::get_id() << "] Open Settings");
  RNS_LOG_NOT_IMPL;
  return createPromiseAsJSIValue(
    rt, [](jsi::Runtime &rt2, std::shared_ptr<Promise> promise) { // NOTE: This is called in  JS Thread
      promise->reject("OpenSettings Not Implemented. Unable to open settings");
  });
}

void RSkLinkingManagerModule::startObserving() {RNS_LOG_NOT_IMPL;}
void RSkLinkingManagerModule::stopObserving() {RNS_LOG_NOT_IMPL;}

} // react
} // facebook

