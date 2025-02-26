/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
* Copyright (C) Kudo Chien
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <glog/logging.h>

#include "include/core/SkPaint.h"

#include "react/renderer/components/view/ViewShadowNode.h"

#include "ReactSkia/components/RSkComponentView.h"

namespace facebook {
namespace react {


RSkComponentView::RSkComponentView(const ShadowView &shadowView)
    : RSkComponent(shadowView) {}

RnsShell::LayerInvalidateMask RSkComponentView::updateComponentProps(SharedProps newviewProps,bool forceUpdate) {
  return RnsShell::LayerInvalidateNone;
}

void RSkComponentView::OnPaint(SkCanvas *canvas) {
  auto component = getComponentData();
  auto const &viewProps = *std::static_pointer_cast<ViewProps const>(component.props);
  /* apply view style props */
  auto borderMetrics=viewProps.resolveBorderMetrics(component.layoutMetrics);
  Rect frame = component.layoutMetrics.frame;

  /*Draw Order : 1. Shadow 2. BackGround 3 Border*/
  auto layerRef=layer();
  if(layerRef->isShadowVisible) {
    drawShadow(canvas,frame,borderMetrics,
                    viewProps.backgroundColor,
                    layerRef->shadowColor,layerRef->shadowOffset,layerRef->shadowOpacity,
                    layerRef->opacity,
                    layerRef->shadowImageFilter,layerRef->shadowMaskFilter
                   );
  }
  drawBackground(canvas,frame,borderMetrics,viewProps.backgroundColor);
  drawBorder(canvas,frame,borderMetrics,viewProps.backgroundColor);
}

} // namespace react
} // namespace facebook
