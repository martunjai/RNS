
/**
 * This code was generated by [react-native-codegen](https://www.npmjs.com/package/react-native-codegen).
 *
 * Do not edit this file as changes may cause incorrect behavior and will be lost
 * once the code is regenerated.
 *
 * @generated by codegen project: GenerateShadowNodeH.js
 */

#pragma once

#include <react/renderer/components/rncore/EventEmitters.h>
#include <react/renderer/components/rncore/Props.h>
#include <react/renderer/components/view/ConcreteViewShadowNode.h>
#include <jsi/jsi.h>

namespace facebook {
namespace react {

JSI_EXPORT extern const char ActivityIndicatorViewComponentName[];

/*
 * `ShadowNode` for <ActivityIndicatorView> component.
 */
using ActivityIndicatorViewShadowNode = ConcreteViewShadowNode<
    ActivityIndicatorViewComponentName,
    ActivityIndicatorViewProps>;

JSI_EXPORT extern const char DatePickerComponentName[];

/*
 * `ShadowNode` for <DatePicker> component.
 */
using DatePickerShadowNode = ConcreteViewShadowNode<
    DatePickerComponentName,
    DatePickerProps,
DatePickerEventEmitter>;

JSI_EXPORT extern const char RCTMaskedViewComponentName[];

/*
 * `ShadowNode` for <RCTMaskedView> component.
 */
using RCTMaskedViewShadowNode = ConcreteViewShadowNode<
    RCTMaskedViewComponentName,
    RCTMaskedViewProps>;

JSI_EXPORT extern const char RCTProgressViewComponentName[];

/*
 * `ShadowNode` for <RCTProgressView> component.
 */
using RCTProgressViewShadowNode = ConcreteViewShadowNode<
    RCTProgressViewComponentName,
    RCTProgressViewProps>;

JSI_EXPORT extern const char PullToRefreshViewComponentName[];

/*
 * `ShadowNode` for <PullToRefreshView> component.
 */
using PullToRefreshViewShadowNode = ConcreteViewShadowNode<
    PullToRefreshViewComponentName,
    PullToRefreshViewProps,
PullToRefreshViewEventEmitter>;

JSI_EXPORT extern const char SwitchComponentName[];

/*
 * `ShadowNode` for <Switch> component.
 */
using SwitchShadowNode = ConcreteViewShadowNode<
    SwitchComponentName,
    SwitchProps,
SwitchEventEmitter>;

JSI_EXPORT extern const char UnimplementedNativeViewComponentName[];

/*
 * `ShadowNode` for <UnimplementedNativeView> component.
 */
using UnimplementedNativeViewShadowNode = ConcreteViewShadowNode<
    UnimplementedNativeViewComponentName,
    UnimplementedNativeViewProps>;

} // namespace react
} // namespace facebook
