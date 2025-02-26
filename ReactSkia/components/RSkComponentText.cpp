/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
* Copyright (C) Kudo Chien
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "react/renderer/components/text/ParagraphShadowNode.h"
#include "react/renderer/components/text/RawTextShadowNode.h"
#include "react/renderer/components/text/TextShadowNode.h"
#include "ReactSkia/views/common/RSkTextUtils.h"

#include "ReactSkia/components/RSkComponentText.h"
#include "ReactSkia/utils/RnsLog.h"

using namespace skia::textlayout;
using namespace facebook::react::RSkTextUtils;

namespace facebook {
namespace react {

RSkComponentText::RSkComponentText(const ShadowView &shadowView)
    : RSkComponent(shadowView, LAYER_TYPE_DEFAULT) {}

RnsShell::LayerInvalidateMask RSkComponentText::updateComponentProps(SharedProps newviewProps,bool forceUpadte) {
  return RnsShell::LayerInvalidateNone;
}

void RSkComponentText::OnPaint(SkCanvas *canvas) {}

RSkComponentRawText::RSkComponentRawText(const ShadowView &shadowView)
    : RSkComponent(shadowView, LAYER_TYPE_DEFAULT) {}

RnsShell::LayerInvalidateMask RSkComponentRawText::updateComponentProps(SharedProps newviewProps,bool forceUpadte) {
  return RnsShell::LayerInvalidateNone;
}

void RSkComponentRawText::OnPaint(SkCanvas *canvas) {}

RSkComponentParagraph::RSkComponentParagraph(const ShadowView &shadowView)
    : RSkComponent(shadowView, LAYER_TYPE_DEFAULT)
    , expectedAttachmentCount(0)
    , currentAttachmentCount(0){}

RnsShell::LayerInvalidateMask RSkComponentParagraph::updateComponentProps(SharedProps newviewProps,bool forceUpadte) {

  auto const &paragraphProps = *std::static_pointer_cast<ParagraphProps const>(newviewProps);
  paragraphAttributes_ = paragraphProps.paragraphAttributes;
  return RnsShell::LayerInvalidateAll;
}

void RSkComponentParagraph::OnPaint(SkCanvas *canvas) {
    SkAutoCanvasRestore(canvas, true);
    auto component = getComponentData();
    auto state = std::static_pointer_cast<ParagraphShadowNode::ConcreteStateT const>(component.state);
    auto const &props = *std::static_pointer_cast<ParagraphProps const>(component.props);
    auto data = state->getData();
    auto borderMetrics = props.resolveBorderMetrics(component.layoutMetrics);
    Rect borderFrame = component.layoutMetrics.frame;
    bool isParent = false;

    /* TODO : Need to cleanup the following code under macro, once it is not required anymore.*/
    /* Reason : After this commit (Id: fe718401e53a044f384747d3641e946250033353 ), react native framework */
    /*          considering all type of nested texts as fragments, so no more parent-child mechanism hereafter. */
    /*          Commented the below code under a macro to avoid child text components handling. */
#ifdef NESTED_TEXT_PARENT_CHILD_DIFFERENTIATION_SUPPORT
    /* Check if this component has parent Paragraph component */
    RSkComponentParagraph* parent = getParentParagraph();
    /* If parent, this text component is part of nested text(aka fragment attachment)*/
    /*    - use parent paragraph builder to add text & push style */
    /*    - draw the paragraph, when we reach the last fragment attachment*/

    if(parent) {

        auto parentComponent = parent->getComponentData();
        auto const &parentProps = *std::static_pointer_cast<ParagraphProps const>(parentComponent.props);
        auto parentBorderMetrics = parentProps.resolveBorderMetrics(parentComponent.layoutMetrics);
        
        if (parent->textLayout.builder) {
            isParent = true;
            textLayout.builder = parent->textLayout.builder;
            parent->expectedAttachmentCount += data.layoutManager->buildParagraph(textLayout,
                                                                                    props.backgroundColor,
                                                                                    data.attributedString,
                                                                                    paragraphAttributes_,
                                                                                    true);
            textLayout.paragraph = parent->textLayout.builder->Build();
            parent->currentAttachmentCount++;
            borderFrame = parentComponent.layoutMetrics.frame;
            borderFrame.origin.x=0;
            borderFrame.origin.y=0;

            if(!parent->expectedAttachmentCount || (parent->expectedAttachmentCount == parent->currentAttachmentCount)) {
                setTextLines(textLayout,
                            parentComponent.layoutMetrics,
                            paragraphAttributes_);

                drawText(textLayout.paragraph,
                            canvas,
                            data.attributedString,
                            parentComponent.layoutMetrics,
                            parentProps,
                            isParent);

                drawBorder(canvas,
                            borderFrame,
                            parentBorderMetrics,
                            parentProps.backgroundColor);
            }
        }
    }
    else
#endif 
    {
        /* If previously created builder is available,using it will append the text in builder*/
        /* If it reaches here,means there is an update in text.So create new paragraph builder*/
        if(nullptr != textLayout.builder) {
            textLayout.builder.reset();
        }
        textLayout.builder = std::static_pointer_cast<ParagraphBuilder>(std::make_shared<ParagraphBuilderImpl>(textLayout.paraStyle,data.layoutManager->collection_));
        auto layerRef=layer(); 
        if(layerRef->isShadowVisible) {
            textLayout.shadow={layerRef->shadowColor,SkPoint::Make(layerRef->shadowOffset.width(),layerRef->shadowOffset.height()),layerRef->shadowRadius};
        }

        expectedAttachmentCount = data.layoutManager->buildParagraph(textLayout,
                                                                        props.backgroundColor,
                                                                        data.attributedString,
                                                                        paragraphAttributes_,
                                                                        true);
        currentAttachmentCount = 0;
        textLayout.paragraph = textLayout.builder->Build();

        /* If the count is 0,means we have no fragment attachments.So paint right away*/
        if(!expectedAttachmentCount) {

            if(layerRef->isShadowVisible) {
                drawShadow(canvas,
                          borderFrame,
                          borderMetrics,
                          props.backgroundColor,
                          layerRef->shadowColor,
                          layerRef->shadowOffset,
                          layerRef->shadowOpacity,
                          layerRef->opacity,
                          layerRef->shadowImageFilter,
                          layerRef->shadowMaskFilter
                       );
            }

            setTextLines(textLayout,
                        component.layoutMetrics,
                        paragraphAttributes_);

            drawText(textLayout.paragraph,
                        canvas,
                        data.attributedString,
                        component.layoutMetrics,
                        props,
                        isParent);

            drawBorder(canvas,
                        borderFrame,
                        borderMetrics,
                        props.backgroundColor);
        }
    }
}
} // namespace react
} // namespace facebook
