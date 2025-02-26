/*
* Copyright (C) 1994-2023 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "include/core/SkImage.h"

#include <cxxreact/JsArgumentHelpers.h>
#include <filesystem>

#include "ReactSkia/components/RSkComponentImage.h"
#include "ReactSkia/sdk/CurlNetworking.h"
#include "ReactSkia/sdk/RNSAssetManager.h"
#include "ReactSkia/utils/RnsLog.h"
#include "RSkImageLoader.h"

using namespace folly;

namespace facebook {
using namespace xplat;
using namespace xplat::module;

namespace react {

RSkImageLoaderModule::~RSkImageLoaderModule() {
  if(imageRequestList_.size()!=0) {
    for(auto imageRequestList : imageRequestList_ ) {
      CurlNetworking::sharedCurlNetworking()->abortRequest(imageRequestList.second);
    }
    imageRequestList_.clear();
  }
}

auto RSkImageLoaderModule::getConstants() -> std::map<std::string, folly::dynamic> {
  return {};
}

auto RSkImageLoaderModule::getMethods() -> std::vector<Method> {
  return {
      Method(
          "getSize",
          [this] (dynamic args, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock){
            getImageSize(jsArgAsString(args,0),resolveBlock,rejectBlock);
          }),
      Method(
          "getSizeWithHeaders",
          [this] (dynamic args, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock) {
            getImageSizeWithHeaders(jsArgAsString(args, 0), jsArgAsString(args, 1), resolveBlock, rejectBlock);
          }),
      Method(
          "prefetchImage",
          [this] (dynamic args, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock) {
            prefetchImage(jsArgAsString(args, 0), resolveBlock, rejectBlock);
          }),
      Method(
          "queryCache",
          [this] (dynamic args, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock) {
            queryCache(jsArgAsArray(args, 0), resolveBlock, rejectBlock);
          }),
  };
}

std::string RSkImageLoaderModule::getName() {
  return "ImageLoader";
}

void RSkImageLoaderModule::getImageSize(std::string uri, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock) {
  //TODO :currently supporting only http and https, in future if we want to support more schema, implement as inline function.
  std::string path;
  sk_sp<SkImage> imageData{nullptr};
  sk_sp<SkData> data;
  decodedimageCacheData imageCacheData;

  if(RNS_UTILS_IS_HTTP_URL(uri)){
    auto sharedCurlNetworking = CurlNetworking::sharedCurlNetworking();
    std::shared_ptr<CurlRequest> remoteCurlRequest = std::make_shared<CurlRequest>(nullptr,uri,0,"GET");

    auto completionCallback =  [this,remoteCurlRequest , resolveBlock , rejectBlock](void* curlresponseData,void *userdata)->bool {
      CurlResponse *responseData =  (CurlResponse *)curlresponseData;
      CurlRequest * curlRequest = (CurlRequest *) userdata;
      decodedimageCacheData imageCacheData;

      if(responseData  && (responseData->responseBuffer!=nullptr) && (responseData->contentSize >0)) {
        sk_sp<SkData> data = SkData::MakeWithCopy(responseData->responseBuffer,responseData->contentSize);
        if (data){
          sk_sp<SkImage> remoteImageData = SkImage::MakeFromEncoded(data);
          if(remoteImageData) {
            RNS_LOG_DEBUG("Network response received success");
            handleResolveBlock(resolveBlock,remoteImageData);
            imageCacheData.imageData = remoteImageData;
            imageCacheData.expiryTime = (SkTime::GetMSecs() + DEFAULT_MAX_CACHE_EXPIRY_TIME);//convert sec to milisecond 60 *1000
            RSkImageCacheManager::getImageCacheManagerInstance()->imageDataInsertInCache(curlRequest->URL.c_str(), imageCacheData);

            //Reset the lamda callback so that curlRequest shared pointer dereffered from the lamda
            // and gets auto destructored after the completion callback.
            remoteCurlRequest->curldelegator.CURLNetworkingCompletionCallback = nullptr;
            imageRequestList_.erase(curlRequest->URL);
            return 0;
          }
        }
      }
      RNS_LOG_ERROR("Network response received error :"<<curlRequest->URL.c_str());
      handleRejectBlock(rejectBlock);
      imageRequestList_.erase(curlRequest->URL.c_str());
      //Reset the lamda callback so that curlRequest shared pointer dereffered from the lamda
      // and gets auto destructored after the completion callback.
      remoteCurlRequest->curldelegator.CURLNetworkingCompletionCallback = nullptr;
      return 0;
    };//end of completionCallback

    remoteCurlRequest->curldelegator.delegatorData = remoteCurlRequest.get();
    remoteCurlRequest->curldelegator.CURLNetworkingCompletionCallback = completionCallback;
    RNS_LOG_DEBUG("Send Request to network");
    sharedCurlNetworking->sendRequest(remoteCurlRequest,folly::dynamic::object());
    imageRequestList_.insert(std::pair<std::string, std::shared_ptr<CurlRequest> >(uri,remoteCurlRequest));
  } else if( uri.substr(0,5) == "data:" ){
    RNS_LOG_NOT_IMPL;
    handleRejectBlock(rejectBlock);
    return;
  } else {
    if(uri.substr(0,7) != "file://") {
      // Generate application specific path to fetch the Image data.
      std::string imagePath = RNSAssetManager::instance()->getAssetPath(uri);
      RNS_LOG_DEBUG(" Get Imagepath from assetManager"<< imagePath);
      path = imagePath;
    }
    data = SkData::MakeFromFileName(path.c_str());
    if (!data) {
      RNS_LOG_ERROR("Unable to make SkData for path : " << path.c_str());
      handleRejectBlock(rejectBlock);
      return;
    }
    imageData = SkImage::MakeFromEncoded(data);
    if(imageData) {
      imageCacheData.imageData = imageData;
      imageCacheData.expiryTime = (SkTime::GetMSecs() + DEFAULT_MAX_CACHE_EXPIRY_TIME);//convert min to millisecond 30 min *60 sec *1000
      RSkImageCacheManager::getImageCacheManagerInstance()->imageDataInsertInCache(uri.c_str(), imageCacheData);
      handleResolveBlock(resolveBlock,imageData);
      return;
    }
  }
}

inline void RSkImageLoaderModule::handleResolveBlock(CxxModule::Callback resolveBlock,sk_sp<SkImage> remoteImageData) {
  std::vector<dynamic> imageDimensions;
  imageDimensions.push_back(folly::dynamic::array(remoteImageData->width(),remoteImageData->height()));
  resolveBlock(imageDimensions);
}

inline void RSkImageLoaderModule::handleRejectBlock(CxxModule::Callback rejectBlock){
  std::vector<dynamic> imageError;
  imageError.push_back(folly::dynamic::array("Image Load failed"));
  rejectBlock(imageError);
}

void RSkImageLoaderModule::getImageSizeWithHeaders(std::string uri, folly::dynamic headers, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock) {
  RNS_LOG_NOT_IMPL;
}

void RSkImageLoaderModule::prefetchImage(std::string uri, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock) {
  RNS_LOG_NOT_IMPL;
}

void RSkImageLoaderModule::queryCache(folly::dynamic uris, CxxModule::Callback resolveBlock, CxxModule::Callback rejectBlock) {
  RNS_LOG_NOT_IMPL;
}

RSkImageLoader::RSkImageLoader(
    const std::string &name,
    std::shared_ptr<CallInvoker> jsInvoker)
    : TurboCxxModule(std::make_unique<RSkImageLoaderModule>(),jsInvoker){
}

}// namespace react
}//namespace facebook
