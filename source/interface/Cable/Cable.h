//
// Created by Davis Polito on 6/28/24.
//

#ifndef BITKLAVIER2_CABLE_H
#define BITKLAVIER2_CABLE_H
#include <JuceHeader.h>
#include <Connection.h>
#include "CubicBezier.h"
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
    Cable(const ConstructionSite* site, const Connection);

    void paint (Graphics& g) override;
    void resized() override;
    bool hitTest (int x, int y) override;
    Connection connection;
    const ConstructionSite* site;
    static constexpr std::string_view componentName = "Cable";
    void repaintIfNeeded (bool force = false);

    void updateStartPoint (bool repaintIfMoved = true);
    void updateEndPoint (bool repaintIfMoved = true);
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

    Colour startColour;
    Colour endColour;
    juce::Range<float> levelRange = { CableConstants::floorDB, 0.0f };
    float levelDB = levelRange.getStart();

    Path createCablePath (juce::Point<float> start, juce::Point<float> end, float scaleFactor);
    CriticalSection pathCrit;
};


#endif //BITKLAVIER2_CABLE_H
