//
// Created by Davis Polito on 4/22/24.
//

#ifndef BITKLAVIER2_BKITEM_H
#define BITKLAVIER2_BKITEM_H

#include "open_gl_image_component.h"
#include "common.h"

class BKItem : /*public DraggableComponent,*/ public Button
{
public:
    BKItem (bitklavier::BKPreparationType type, SynthSection &parent);
    void mouseDown (const MouseEvent& e) override;
    void mouseDoubleClick (const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;


    void resized() override
    {
        //redoImage();
        const DropShadow shadow(Colours::white, 5, juce::Point<int>(0, 0));

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

    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown);

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
    SynthSection &parent;
    juce::Path layer_1_;
    juce::Path layer_2_;
    juce::Path layer_3_;
    juce::Path layer_4_;
    Colour prep_color_;
    bool wasJustDragged;
};

//class KeymapItem : public BKItem
//{
//public:
//    KeymapItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeKeymap, &parent)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() * 23 / 100);
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 9);
//        float layer_2_width = bounds.getWidth() * 54 / 100;
//        float layer_2_height = bounds.getHeight() * 11 / 20;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//        // Ascertains the appropriate location for layer_3 based on the preparation
//        // window size
//        float layer_3_x = bounds.getX() + (bounds.getWidth() * 13 / 32);
//        float layer_3_y = bounds.getY() + (bounds.getHeight() / 9);
//        float layer_3_width = bounds.getWidth() * 1 / 10;
//        float layer_3_height = bounds.getHeight() * 29 / 36;
//
//        // Transforms layer_3 to fit appropriately in the preparation window
//        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(layer_3_x,
//                                                                  layer_3_y,
//                                                                  layer_3_width,
//                                                                  layer_3_height,
//                                                                  false));
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary2, true);
//        g.setColour(c);
//        g.fillPath(layer_1_);
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//
//        g.fillPath(layer_2_);
//        g.fillPath(layer_3_);
//        g.strokePath(layer_1_,juce::PathStrokeType (5, juce::PathStrokeType::mitered) );
//    }
//};
//
//class DirectItem : public BKItem
//{
//public:
//    DirectItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeDirect)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() / 11);
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 5);
//        float layer_2_width = bounds.getWidth() * 9 / 11;
//        float layer_2_height = bounds.getHeight() * 3 / 5;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//        // Ascertains the appropriate location for layer_3 based on the preparation
//        // window size
//        float layer_3_x = bounds.getX() + (bounds.getWidth() * 17 / 30);
//        float layer_3_y = bounds.getY() + (bounds.getHeight() * 8 / 30);
//        float layer_3_width = bounds.getWidth() / 6;
//        float layer_3_height = bounds.getHeight() * 11 / 20;
//
//        // Transforms layer_3 to fit appropriately in the preparation window
//        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(layer_3_x,
//                                                                  layer_3_y,
//                                                                  layer_3_width,
//                                                                  layer_3_height,
//                                                                  true));
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        g.fillPath(layer_1_);
//
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//        g.fillPath(layer_2_);
//        g.fillPath(layer_3_);
//
//        g.strokePath(layer_1_, juce::PathStrokeType(5, juce::PathStrokeType::mitered));
//    }
//};
//
//class NostalgicItem : public BKItem
//{
//public:
//    NostalgicItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeNostalgic)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() / 11);
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 8);
//        float layer_2_width = bounds.getWidth() * 9 / 11;
//        float layer_2_height = bounds.getHeight() * 3 / 4;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//        // Ascertains the appropriate location for layer_3 based on the preparation
//        // window size
//        float layer_3_x = bounds.getX() + (bounds.getWidth() * 18 / 30);
//        float layer_3_y = bounds.getY() + (bounds.getHeight() * 9 / 30);
//        float layer_3_width = bounds.getWidth() / 7;
//        float layer_3_height = bounds.getHeight() * 11 / 20;
//
//        // Transforms layer_3 to fit appropriately in the preparation window
//        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(layer_3_x,
//                                                                  layer_3_y,
//                                                                  layer_3_width,
//                                                                  layer_3_height,
//                                                                  true));
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        g.fillPath(layer_1_);
//
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//        g.fillPath(layer_2_);
//        g.fillPath(layer_3_);
//
//        g.strokePath(layer_1_,juce::PathStrokeType (5, juce::PathStrokeType::mitered) );
//    }
//
//};
//
//class SynchronicItem : public BKItem
//{
//public:
//    SynchronicItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeSynchronic)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() / 15);
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 9);
//        float layer_2_width = bounds.getWidth() * 13 / 15;
//        float layer_2_height = bounds.getHeight() * 7 / 9;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//        // Ascertains the appropriate location for layer_3 based on the preparation
//        // window size
//        float layer_3_x = bounds.getX() + (bounds.getWidth() * 2 / 3);
//        float layer_3_y = bounds.getY() + (bounds.getHeight() / 3);
//        float layer_3_width = bounds.getWidth() / 7;
//        float layer_3_height = bounds.getHeight() / 2;
//
//        // Transforms layer_3 to fit appropriately in the preparation window
//        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(layer_3_x,
//                                                                  layer_3_y,
//                                                                  layer_3_width,
//                                                                  layer_3_height,
//                                                                  true));
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        g.fillPath(layer_1_);
//
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//        g.fillPath(layer_2_);
//        g.fillPath(layer_3_);
//
//        g.strokePath(layer_1_,juce::PathStrokeType (5, juce::PathStrokeType::mitered));
//    }
//
//};
//
//class BlendronicItem : public BKItem
//{
//public:
//    BlendronicItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeBlendronic)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() / 15);
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 7);
//        float layer_2_width = bounds.getWidth() * 23 / 30;
//        float layer_2_height = bounds.getHeight() * 5 / 7;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//        // Ascertains the appropriate location for layer_3 based on the preparation
//        // window size
//        float layer_3_x = bounds.getX() + (bounds.getWidth() * 23 / 30);
//        float layer_3_y = bounds.getY() + (bounds.getHeight() * 12 / 30);
//        float layer_3_width = bounds.getWidth() / 7;
//        float layer_3_height = bounds.getHeight() * 9 / 20;
//
//        // Transforms layer_3 to fit appropriately in the preparation window
//        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(layer_3_x,
//                                                                  layer_3_y,
//                                                                  layer_3_width,
//                                                                  layer_3_height,
//                                                                  true));
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        g.fillPath(layer_1_);
//
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//        g.fillPath(layer_2_);
//        g.fillPath(layer_3_);
//
//        g.strokePath(layer_1_,juce::PathStrokeType (5, juce::PathStrokeType::mitered) );
//    }
//
//};
//
//class ResonanceItem : public BKItem
//{
//public:
//    ResonanceItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeResonance)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() * 9 / 30);
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 7);
//        float layer_2_width = bounds.getWidth() * 18 / 30;
//        float layer_2_height = bounds.getHeight() * 5 / 7;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//        // Ascertains the appropriate location for layer_3 based on the preparation
//        // window size
//        float layer_3_x = bounds.getX() + (bounds.getWidth() / 10);
//        float layer_3_y = bounds.getY() + (bounds.getHeight() * 10 / 30);
//        float layer_3_width = bounds.getWidth() / 7;
//        float layer_3_height = bounds.getHeight() / 2;
//
//        // Transforms layer_3 to fit appropriately in the preparation window
//        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(layer_3_x,
//                                                                  layer_3_y,
//                                                                  layer_3_width,
//                                                                  layer_3_height,
//                                                                  true));
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        g.fillPath(layer_1_);
//
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//        g.fillPath(layer_2_);
//        g.fillPath(layer_3_);
//
//        g.strokePath(layer_1_, juce::PathStrokeType(5, juce::PathStrokeType::mitered));
//    }
//};
//
//class TuningItem : public BKItem
//{
//public:
//    TuningItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeTuning)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() * 3 / 16 );
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 10);
//        float layer_2_width = bounds.getWidth() * 11 / 16;
//        float layer_2_height = bounds.getHeight() * 4 / 5;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        g.fillPath(layer_1_);
//
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//        g.fillPath(layer_2_);
//
//        g.strokePath(layer_1_, juce::PathStrokeType(5, juce::PathStrokeType::mitered));
//    }
//
//};
//
//class TempoItem : public BKItem
//{
//public:
//    TempoItem() : BKItem(bitklavier::BKPreparationType::PreparationTypeTempo)
//    {
//
//    }
//
//    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        // Ascertains the current window size
//        Rectangle<float> bounds = getLocalBounds().toFloat();
//
//        // Ascertains the appropriate location for layer_2 based on the preparation
//        // window size
//        float layer_2_x = bounds.getX() + (bounds.getWidth() / 2 );
//        float layer_2_y = bounds.getY() + (bounds.getHeight() / 8);
//        float layer_2_width = bounds.getWidth() / 2;
//        float layer_2_height = bounds.getHeight() * 19 / 30;
//
//        // Transforms layer_2 to fit appropriately in the preparation window
//        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(layer_2_x,
//                                                                  layer_2_y,
//                                                                  layer_2_width,
//                                                                  layer_2_height,
//                                                                  false));
//
//
//        // Retrieves and sets the color of each layer
//
//        g.setColour(findColour(Skin::kShadow, true));
//        g.drawImageAt(shadow_, 0, 0, true);
//        g.fillPath(layer_1_);
//
//        Colour c;
//        c = findColour(Skin::kWidgetPrimary1, true);
//        g.setColour(c);
//        g.fillPath(layer_2_);
//
//        g.strokePath(layer_1_, juce::PathStrokeType(5, juce::PathStrokeType::mitered));
//    }
//};


#endif //BITKLAVIER2_BKITEM_H
