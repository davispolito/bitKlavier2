//
// Created by Davis Polito on 4/22/24.
//

#include "BKItem.h"
#include "paths.h"
#include "synth_section.h"

namespace
{
    // Returns the path for each preparation type
    Array<Path> getPathForPreparation (bitklavier::BKPreparationType type)
    {
        // Returns the paths for a keymap preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeKeymap)
            return Paths::keymapPaths();

        // Returns the paths for a direct preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeDirect)
            return Paths::directPaths();

        // Returns the paths for a nostalgic preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeNostalgic)
            return Paths::nostalgicPaths();

        // Returns the paths for a synchronic preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeSynchronic)
            return Paths::synchronicPaths();

        // Returns the paths for a blendronic preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeBlendronic)
            return Paths::blendronicPaths();

        // Returns the paths for a resonance preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeResonance)
            return Paths::resonancePaths();

        // Returns the paths for a tuning preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeTuning)
            return Paths::tuningPaths();

        // Returns the paths for a tempo preparation window
        if (type == bitklavier::BKPreparationType::PreparationTypeTempo)
            return Paths::tempoPaths();
    }
}

BKItem::BKItem (bitklavier::BKPreparationType type, SynthSection &parent) : Button("bkitem"), parent(parent)
{

    image_component_.setComponent(this);

    Array<Path> paths;
    paths = getPathForPreparation(type);
    layer_1_ = paths.getUnchecked(0);
    layer_2_ = paths.getUnchecked(1);
    layer_3_ = paths.getUnchecked(2);
}

void BKItem::paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // Ascertains the current window size
    Rectangle<float> bounds = getLocalBounds().toFloat();

    // Ascertains the appropriate location for layer_2 based on the preparation
    // window size

    // float layer_2_x = bounds.getX() + (bounds.getWidth() * 23 / 100);
    float layer_2_x = bounds.getX() + (bounds.getWidth() * parent.findValue(Skin::kLayer2XOffsetFactor));

    // float layer_2_y = bounds.getY() + (bounds.getHeight() / 9);
    float layer_2_y = bounds.getY() + (bounds.getHeight() * parent.findValue(Skin::kLayer2YOffsetFactor));

    // float layer_2_width = bounds.getWidth() * 54 / 100;
    float layer_2_width = bounds.getWidth() * parent.findValue(Skin::kLayer2WidthFactor);

    // float layer_2_height = bounds.getHeight() * 11 / 20;
    float layer_2_height = bounds.getHeight() * parent.findValue(Skin::kLayer2HeightFactor);

    bool layer_2_preserve_proportions = (int) parent.findValue(Skin::kLayer2PreserveProportions);

    // Transforms layer_2 to fit appropriately in the preparation window
    layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
                                                              layer_2_y,
                                                              layer_2_width,
                                                              layer_2_height,
                                                              layer_2_preserve_proportions));
//
//
//    // float layer_2_x = bounds.getX() + (bounds.getWidth() * 23 / 100);
//    float layer_3_x = bounds.getX() + (bounds.getWidth() * parent.findValue(Skin::kLayer3XOffsetFactor));
//
//    // float layer_2_y = bounds.getY() + (bounds.getHeight() / 9);
//    float layer_3_y = bounds.getY() + (bounds.getHeight() * parent.findValue(Skin::kLayer3YOffsetFactor));
//
//    // float layer_2_width = bounds.getWidth() * 54 / 100;
//    float layer_3_width = bounds.getWidth() * parent.findValue(Skin::kLayer3WidthFactor);
//
//    // float layer_2_height = bounds.getHeight() * 11 / 20;
//    float layer_3_height = bounds.getWidth() * parent.findValue(Skin::kLayer3HeightFactor);
//
//    bool layer_3_preserve_proportions = (int) parent.findValue(Skin::kLayer3PreserveProportions);
//
//
//    // Ascertains the appropriate location for layer_3 based on the preparation
//    // window size
////        float layer_3_x = bounds.getX() + (bounds.getWidth() * 13 / 32);
////        float layer_3_y = bounds.getY() + (bounds.getHeight() / 9);
////        float layer_3_width = bounds.getWidth() * 1 / 10;
////        float layer_3_height = bounds.getHeight() * 29 / 36;
//
//    // Transforms layer_3 to fit appropriately in the preparation window
//    layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(layer_3_x,
//                                                              layer_3_y,
//                                                              layer_3_width,
//                                                              layer_3_height,
//                                                              layer_3_preserve_proportions));

    // Retrieves and sets the color of each layer

    g.setColour(findColour(Skin::kShadow, true));
    g.drawImageAt(shadow_, 0, 0, true);
    Colour c;
    c = findColour(Skin::kBody, true);
    g.setColour(c);
    g.fillPath(layer_1_);
    c = findColour(Skin::kWidgetPrimary1, true);
    g.setColour(c);
    g.fillPath(layer_2_);
    g.fillPath(layer_3_);
    g.strokePath(layer_1_,juce::PathStrokeType (5, juce::PathStrokeType::mitered));
}


void BKItem::mouseDown(const MouseEvent& e)
{
    getParentComponent()->mouseDown(e);
}

void BKItem::mouseDrag(const MouseEvent& e)
{
    getParentComponent()->mouseDrag(e);
}

void BKItem::mouseDoubleClick(const MouseEvent& e)
{

}