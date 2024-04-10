//
// Created by Davis Polito on 1/17/24.
//
#include "draggable_component.h"
#include "paths.h"

namespace
{
    Path getPathForPreparation (bitklavier::BKPreparationType type)
    {
        if (type == bitklavier::BKPreparationType::PreparationTypeDirect)
            return Paths::direct();
    }
}

BKItem::BKItem (bitklavier::BKPreparationType type) :
                                                                                                Button("bkitem")
                                                                                                 //state(v),
//um(um)
                                                      //DraggableComponent(true, false, true, 50, 50, 50, 50)
{

    //setSize(120,120);
    image_component_.setComponent(this);
    //state.addListener(this);
    icon_ = getPathForPreparation(type);
    //setSkinOverride(Skin::kBKItem);
    //setShape(getPathForPreparation(type));
    // background_ = std::make_unique<OpenGlImageComponent>("background");
 ///addOpenGlComponent(background_.get());
}

void BKItem::paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    Rectangle<float> bounds = getLocalBounds().toFloat();
    //Component* component = component_ ? component_ : this;
//    static constexpr float kIconSize = 0.6f;
//    float icon_width = kIconSize * getHeight();
//    float icon_x = getWidth() / 2.0f + (getWidth() / 2.0f - icon_width) / 2.0f;
//    float icon_y = (getHeight() - icon_width) / 2.0f;
//    Rectangle<float> icon_bounds(icon_x, icon_y, icon_width, icon_width);

    //redoImage();
    g.setColour(findColour(Skin::kShadow, true));
    g.drawImageAt(shadow_, 0, 0, true);
    //g.setColour(Colours::red);
    //g.fillPath(icon_, icon_.getTransformToScaleToFit(bounds, true));
}

void BKItem::mouseDown(const MouseEvent& e)
{

}

//void BKItem::itemIsBeingDragged(const MouseEvent& e)
//{
//    wasJustDragged = true;
//}

void BKItem::mouseDoubleClick(const MouseEvent& e)
{

}

void BKItem::resized()
{
    //redoImage();
    const DropShadow shadow(Colours::white, 5, Point<int>(0, 0));

    if (shadow_.getWidth() == getWidth() && shadow_.getHeight() == getHeight())
        return;

    Rectangle<float> bounds = getLocalBounds().toFloat();
    icon_.applyTransform(icon_.getTransformToScaleToFit(bounds,true));
    shadow_ = Image(Image::SingleChannel, getWidth(), getHeight(), true);

    Graphics shadow_g(shadow_);
    shadow.drawForPath(shadow_g, icon_);
    //shadow.drawForPath(shadow_g, ring_);
    redoImage();

    //Rectangle<float> bounds = getLocalBounds().toFloat();

    //OpenGlImageComponent::resized();
}
//
//void BKItem::valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override
//{
//    if (v == state)
////        if (i == IDs::name || i == IDs::colour)
////            repaint();
//}