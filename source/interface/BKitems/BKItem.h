//
// Created by Davis Polito on 4/22/24.
//

#ifndef BITKLAVIER2_BKITEM_H
#define BITKLAVIER2_BKITEM_H
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "open_gl_image_component.h"
#include "common.h"
class BKItem : /*public DraggableComponent,*/ public Button
{
public:
    BKItem (bitklavier::BKPreparationType type);
    void mouseDown (const MouseEvent& e) override;
    void mouseDoubleClick (const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    // void paint(Graphics& g) override;
    void setIcons (const Path& layer_1, const Path& layer_2, const Path& layer_3, const Path& layer_4)
    {
        layer_1_ = layer_1;
        layer_2_ = layer_2;
        layer_3_ = layer_3;
        layer_4_ = layer_4;
    }
    void resized() override
    {
        //redoImage();
        const DropShadow shadow(Colours::white, 5, Point<int>(0, 0));

        if (shadow_.getWidth() == getWidth() && shadow_.getHeight() == getHeight())
            return;

        Rectangle<float> bounds = getLocalBounds().toFloat();//.reduced(10).withX(0).withY(0);

        Rectangle<float> s = bounds.getProportion<float>({0.0f, 0.0f, 0.9f, 0.9f});
        s.setCentre(bounds.getCentre());
        layer_1_.applyTransform(layer_1_.getTransformToScaleToFit(s,true));
        int width = getWidth();
        int height = getHeight();

        shadow_ = Image(Image::SingleChannel,width, height, true);

        Graphics shadow_g(shadow_);
        shadow.drawForPath(shadow_g, layer_1_);

        redoImage();
    };

    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        Rectangle<float> bounds = getLocalBounds().toFloat();

        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(bounds.reduced(40.f ).withX(80 ),true));
        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(bounds.reduced(20 ),true));
        Rectangle<float> s = bounds.getProportion<float>({0.0f, 0.0f, 0.9f, 0.9f});
        s.setCentre(bounds.getCentre());
        layer_1_.applyTransform(layer_1_.getTransformToScaleToFit(s,true));

        g.setColour(findColour(Skin::kShadow, true));
        g.drawImageAt(shadow_, 0, 0, true);


        g.fillPath(layer_1_);
        Colour c = findColour(Skin::kWidgetPrimary1, true);
        g.setColour(c);

        g.fillPath(layer_2_);
        g.fillPath(layer_3_);
        g.strokePath(layer_1_,juce::PathStrokeType (2, juce::PathStrokeType::mitered) );

    }
    class Listener
    {
    public:
        virtual ~Listener() = default;

    };

    OpenGlImageComponent* getImageComponent() { return &image_component_; }
    void redoImage() { image_component_.redrawImage (true); } //image_component_.redrawImage(true); }

    void setColor (Colour col)
    {
        prep_color_ = col;
    }


    float size_ratio;

protected:
    //void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override;
    //    juce::ValueTree &state;
    //    juce::UndoManager &um;
    Image shadow_;
    std::vector<Listener*> listeners_;
    OpenGlImageComponent image_component_;

    juce::Path layer_1_;
    juce::Path layer_2_;
    juce::Path layer_3_;
    juce::Path layer_4_;
    Colour prep_color_;
    bool wasJustDragged;



};

class DirectItem : public BKItem
{
public:
    DirectItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeDirect)
    {

    }




};

class NostalgicItem : public BKItem
{
public:
    NostalgicItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeNostalgic)
    {

    }




};


#endif //BITKLAVIER2_BKITEM_H
