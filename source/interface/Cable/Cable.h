//
// Created by Davis Polito on 6/28/24.
//

#ifndef BITKLAVIER2_CABLE_H
#define BITKLAVIER2_CABLE_H
#include "valuetree_utils/VariantConverters.h"
#include "CubicBezier.h"
#include "open_gl_image_component.h"
#include "Identifiers.h"


class CableView;
namespace CableConstants
{
    const Colour cableColour (0xFFD0592C); // currently only used for "glow"
    constexpr float minCableThickness = 5.0f;
    constexpr float portCircleThickness = 1.5f;

    constexpr int getPortDistanceLimit (float scaleFactor) { return int (20.0f * scaleFactor); }
    constexpr auto portOffset = 50.0f;

    constexpr float floorDB = -60.0f;
} // namespace CableConstants
class ConstructionSite;
class Cable : public Component {
public:
    Cable(ConstructionSite* site, CableView& cableView);
    ~Cable();

    void paint (Graphics& g) override;
//    void resized() override;
//    bool hitTest (int x, int y) override;
    //Connection connection;

    ConstructionSite* site;
    CableView& cableView;
    static constexpr std::string_view componentName = "Cable";
    void repaintIfNeeded (bool force = false);

    void updateStartPoint (bool repaintIfMoved = true);
    void updateEndPoint (bool repaintIfMoved = true);
    std::shared_ptr<OpenGlImageComponent> getImageComponent() { return image_component_; }
    void redoImage() { image_component_->redrawImage (true);}
    std::shared_ptr<OpenGlImageComponent> image_component_;

    void setInput (AudioProcessorGraph::NodeAndChannel newSource)
    {
        if (connection.source != newSource)
        {
            connection.source = newSource;
            state.setProperty(IDs::src,  VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(connection.source.nodeID), nullptr);
            state.setProperty(IDs::srcIdx, connection.source.channelIndex, nullptr);

            update();
        }
    }

    void setOutput (AudioProcessorGraph::NodeAndChannel newDest)
    {
        if (connection.destination != newDest)
        {
            connection.destination = newDest;
            state.setProperty(IDs::dest,  VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(connection.destination.nodeID), nullptr);
            state.setProperty(IDs::destIdx, connection.destination.channelIndex, nullptr);
            update();
        }
    }

    void dragStart (juce::Point<float> pos)
    {
        lastInputPos = pos;
        resizeToFit();
    }

    void dragEnd (juce::Point<float> pos)
    {
        lastOutputPos = pos;
        resizeToFit();
    }

    void update()
    {
        juce::Point<float> p1, p2;
        getPoints (p1, p2);

        if (lastInputPos != p1 || lastOutputPos != p2)
            resizeToFit();
    }

    ValueTree getValueTree();


    void resizeToFit()
    {
        juce::Point<float> p1, p2;
        getPoints (p1, p2);

        auto newBounds = Rectangle<float> (p1, p2).expanded (4.0f).getSmallestIntegerContainer();

        if (newBounds != getBounds())
            setBounds (newBounds);
        else
            resized();

        repaint();
    }
    void getPoints (juce::Point<float>& p1, juce::Point<float>& p2) const;
    AudioProcessorGraph::Connection connection { { {}, 0 }, { {}, 0 } };
    bool hitTest (int x, int y) override
    {
        auto pos = juce::Point<int> (x, y).toFloat();

        if (hitPath.contains (pos))
        {
            double distanceFromStart, distanceFromEnd;
            getDistancesFromEnds (pos, distanceFromStart, distanceFromEnd);

            // avoid clicking the connector when over a pin
            return distanceFromStart > 7.0 && distanceFromEnd > 7.0;
        }

        return false;
    }

    void mouseDown (const MouseEvent&) override
    {
        dragging = false;
    }

    void mouseDrag (const MouseEvent& e) override;



    void mouseUp (const MouseEvent& e) override;


    void resized() override
    {
        juce::Point<float> p1, p2;
        getPoints (p1, p2);

        lastInputPos = p1;
        lastOutputPos = p2;

        p1 -= getPosition().toFloat();
        p2 -= getPosition().toFloat();

        linePath.clear();
        linePath.startNewSubPath (p1);
        linePath.cubicTo (p1.x, p1.y + (p2.y - p1.y) * 0.33f,
                          p2.x, p1.y + (p2.y - p1.y) * 0.66f,
                          p2.x, p2.y);

        PathStrokeType wideStroke (8.0f);
        wideStroke.createStrokedPath (hitPath, linePath);

        PathStrokeType stroke (2.5f);
        stroke.createStrokedPath (linePath, linePath);

        auto arrowW = 5.0f;
        auto arrowL = 4.0f;

        Path arrow;
        arrow.addTriangle (-arrowL, arrowW,
                           -arrowL, -arrowW,
                           arrowL, 0.0f);

        arrow.applyTransform (AffineTransform()
                                      .rotated (MathConstants<float>::halfPi - (float) atan2 (p2.x - p1.x, p2.y - p1.y))
                                      .translated ((p1 + p2) * 0.5f));

        linePath.addPath (arrow);
        linePath.setUsingNonZeroWinding (true);
        MessageManager::callAsync (
                [safeComp = Component::SafePointer<Cable> (this)]
                {
                    if (auto* comp = safeComp.getComponent())
                        comp->redoImage();
                    //comp->repaint (cableBounds);
                });
    }

    void getDistancesFromEnds (juce::Point<float> p, double& distanceFromStart, double& distanceFromEnd) const
    {
        juce::Point<float> p1, p2;
        getPoints (p1, p2);

        distanceFromStart = p1.getDistanceFrom (p);
        distanceFromEnd   = p2.getDistanceFrom (p);
    }

    void setValueTree(const ValueTree& v)
    {
        state = v;
        connection.source = {VariantConverter<juce::AudioProcessorGraph::NodeID>::fromVar(v.getProperty(IDs::src)), v.getProperty(IDs::srcIdx)};
        connection.destination = {VariantConverter<juce::AudioProcessorGraph::NodeID>::fromVar(v.getProperty(IDs::dest)), v.getProperty(IDs::destIdx)};
        update();
    }
    ValueTree state {IDs::CONNECTION};

private:
    float getCableThickness() const;
    void drawCableShadow (Graphics& g, float thickness);
    void drawCableEndCircle (Graphics& g, juce::Point<float> centre, Colour colour) const;
    void drawCable (Graphics& g, juce::Point<float> start, juce::Point<float> end);
    //CableView& cableView;
    //const BoardComponent* board = nullptr;

    //chowdsp::PopupMenuHelper popupMenu;

    Path cablePath {};
    int numPointsInPath = 0;
    CubicBezier bezier;
    float cableThickness = 0.0f;

    using AtomicPoint = std::atomic<juce::Point<float>>;
    static_assert (AtomicPoint::is_always_lock_free, "Atomic point needs to be lock free!");
    AtomicPoint startPoint {};
    AtomicPoint endPoint {};
    std::atomic<float> scaleFactor = 1.0f;
    juce::Point<float> lastInputPos, lastOutputPos;
    Path linePath, hitPath;
    bool dragging = false;

    Colour startColour;
    Colour endColour;
    juce::Range<float> levelRange = { CableConstants::floorDB, 0.0f };
    float levelDB = levelRange.getStart();

    Path createCablePath (juce::Point<float> start, juce::Point<float> end, float scaleFactor);
    CriticalSection pathCrit;
};


#endif //BITKLAVIER2_CABLE_H
