//
// Created by Davis Polito on 7/10/24.
//

#ifndef BITKLAVIER2_BKPORT_H
#define BITKLAVIER2_BKPORT_H
#include "valuetree_utils/VariantConverters.h"
#include "open_gl_image_component.h"
#include "common.h"
class SynthGuiInterface;
class BKPort : public juce::Button
{
public:
    BKPort( juce::ValueTree v);
    class Listener
    {
    public:
        virtual ~Listener() = default;
        //virtual void portClicked(const juce::Point<int>& pos) = 0;
        virtual void beginConnectorDrag(juce::AudioProcessorGraph::NodeAndChannel source,
                                        juce::AudioProcessorGraph::NodeAndChannel dest,
                                        const juce::MouseEvent& e) = 0;
        virtual void dragConnector(const juce::MouseEvent& e) = 0;
        virtual void endDraggingConnector(const juce::MouseEvent& e) = 0;
    };

    void addListener(Listener* listener) { listeners_.push_back(listener); }
    std::vector<Listener*> listeners_;
    void mouseDown (const juce::MouseEvent& e) override;
    //void mouseDoubleClick (const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void resized()
    {
        redoImage();
    }

    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
//        juce::Path p;
//        g.setColour(juce::Colours::grey);
//        //g.setOpacity(1.f);
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        juce::Rectangle<float> s = bounds.getProportion<float>({0.0f, 0.0f, 0.9f, 0.9f});
//        p.addPieSegment(0, 0, getWidth(), getHeight(),juce::float_Pi *2, juce::float_Pi , -0.8f);
//        g.fillPath(p);
        const auto portBounds = getLocalBounds().toFloat();
        bool isConnected = false;
        if (isConnected)
        {
//            g.setColour (juce::Colours::grey);//g.setColour (procColour);
//            g.fillEllipse (portBounds);
//
//            const auto width = (float) getWidth();
//            const auto xOff = isInput ? -1.0f : 1.0f;
//            const auto bigArcStart = 0.0f;
//            const auto bigArcEnd = isInput ? juce::float_Pi + 0.1f : -juce::MathConstants<float>::pi - 0.1f;
//            const auto smallArcStart = isInput ? juce::float_Pi : -juce::MathConstants<float>::pi;
//            const auto smallArcEnd = isInput ? -0.1f : 0.1f;
//
//            juce::Path arcPath;
//            arcPath.startNewSubPath (portBounds.getCentreX() + xOff, portBounds.getY());
//            arcPath.addArc (portBounds.getX(), portBounds.getY(), portBounds.getWidth(), portBounds.getHeight(), bigArcStart, bigArcEnd);
//            auto pb = portBounds.reduced (width * 0.31f).translated (0.0f, -0.5f);
//            arcPath.lineTo (pb.getCentreX() + xOff, pb.getBottom());
//            arcPath.addArc (pb.getX(), pb.getY(), pb.getWidth(), pb.getHeight(), smallArcStart, smallArcEnd);
//            arcPath.closeSubPath();
//
//            g.setColour (juce::Colours::grey); //g.setColour (getPortColour());
//            g.fillPath (arcPath);
//
//            g.setColour (juce::Colours::white);
//            g.drawEllipse (pb, CableConstants::portCircleThickness);
        }
        else
        {
//            g.setColour (juce::Colours::black);
//            g.fillEllipse (portBounds);
//            juce::Path p;
//        //g.setColour(juce::Colours::grey);
//        //g.setOpacity(1.f);
//            g.setColour (juce::Colours::lightgreen);
//        p.addPieSegment(0, 0, getWidth(),getHeight(),juce::float_Pi *2, juce::float_Pi , 0.1f);
//        g.fillPath(p);


        }
//        juce::Rectangle<float> bounds = getLocalBounds().toFloat();

        port_outline.applyTransform(port_outline.getTransformToScaleToFit(bounds,true).rotated(isInput ? pin.isMIDI()
                                                                                                         ? (juce::MathConstants<float>::pi / 2) :
                                                                                                         juce::MathConstants<float>::pi : pin.isMIDI() ? 3 * juce::MathConstants<float>::pi / 2 : 0, bounds.getWidth()/2, bounds.getHeight()/2));

        port_fill.applyTransform(port_fill.getTransformToScaleToFit(bounds,true).rotated(isInput ? pin.isMIDI()
                                                                                                   ? (juce::MathConstants<float>::pi / 2) :
                                                                                                   juce::MathConstants<float>::pi : pin.isMIDI() ? 3 * juce::MathConstants<float>::pi / 2 : 0, bounds.getWidth()/2, bounds.getHeight()/2));



        g.setColour(juce::Colours::lightgrey);
        g.fillPath(port_fill);
        auto c = findColour(Skin::kWidgetPrimary1, true);
        g.setColour(c);

        g.fillPath(port_outline);
//        g.setColour (juce::Colours::lightgrey);
//        g.drawEllipse (portBounds.reduced (1.0f), 1.0f);
//        g.fillEllipse(s);
    }

    juce::Path port_outline;
    juce::Path port_fill;
    juce::AudioProcessorGraph::NodeAndChannel pin;
    juce::CachedValue<bool> isInput;
    int busIdx = 0;
    std::shared_ptr<OpenGlImageComponent> getImageComponent() { return image_component_; }
    void redoImage() { image_component_->redrawImage (true); } //image_component_.redrawImage(true); }
    std::shared_ptr<OpenGlImageComponent> image_component_;
    juce::ValueTree state;
};




#endif //BITKLAVIER2_BKPORT_H
