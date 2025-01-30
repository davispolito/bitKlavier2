//
// Created by Davis Polito on 1/30/25.
//

#ifndef BITKLAVIER2_MODULATIONITEM_H
#define BITKLAVIER2_MODULATIONITEM_H
#include "BKItem.h"
class ModulationItem : public BKItem
{
public:
    ModulationItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeModulation)
    {
    }

    void resized() override
    {
        //redoImage();
        const juce::DropShadow shadow(juce::Colours::white, 5, juce::Point<int>(0, 0));

        if (shadow_.getWidth() == getWidth() && shadow_.getHeight() == getHeight())
            return;

        juce::Rectangle<float> bounds = getLocalBounds().toFloat();//.reduced(10).withX(0).withY(0);

        juce::Rectangle<float> s = bounds.getProportion<float>({0.0f, 0.0f, 0.9f, 0.9f});
        s.setCentre(bounds.getCentre());
        layer_1_.applyTransform(layer_1_.getTransformToScaleToFit(s,true));
        int width = getWidth();
        int height = getHeight();

        shadow_ = juce::Image(juce::Image::SingleChannel,width, height, true);

        juce::Graphics shadow_g(shadow_);
        shadow.drawForPath(shadow_g, layer_1_);

        redoImage();
    };
    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        // Ascertains the current window size
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        // Retrieves and sets the color of each layer
//

        g.setColour(findColour(Skin::kShadow, true));
        g.drawImageAt(shadow_, 0, 0, true);
        juce::Colour c;
        c = findColour(Skin::kWidgetPrimary2, true);
        g.setColour(c);
        g.fillPath(layer_1_);

    }
    bool hitTest(int x, int y) override
    {
        layer_1_.contains(x,y, 0.01);
    }
};
#endif //BITKLAVIER2_MODULATIONITEM_H
