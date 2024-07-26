//
// Created by Davis Polito on 6/28/24.
//

#include "Cable.h"
#include "CableView.h"
#include "ConstructionSite.h"
using namespace CableConstants;

Cable::Cable (const ConstructionSite* site, CableView& cableView) : Component (Cable::componentName.data()),
                                                                                            image_component_(new OpenGlImageComponent()),
                                                                                            site(site),
                                                                                            cableView(cableView)

{
    //this->startPoint.store(startPoint.toFloat());
    //cableView = cableView;
    image_component_->setComponent(this);
}


Cable::~Cable()
{
//    site->open_gl.initOpenGlComp.try_enqueue([this]{
//        image_component_->destroy(site->open_gl);
//    });
}

void Cable::getPoints (juce::Point<float>& p1, juce::Point<float>& p2) const
{
    p1 = lastInputPos;
    p2 = lastOutputPos;

    if (auto* src = site->getComponentForPlugin (connection.source.nodeID))
        p1 = src->getPinPos (connection.source.channelIndex, false);

    if (auto* dest = site->getComponentForPlugin (connection.destination.nodeID))
        p2 = dest->getPinPos (connection.destination.channelIndex, true);
}
//bool Cable::hitTest (int x, int y)
//{
//    juce::Point clickedP ((float) x, (float) y);
//    for (int i = 1; i <= numPointsInPath; ++i)
//    {
//        auto pointOnPath = bezier.getPointOnCubicBezier ((float) i / (float) numPointsInPath);
//        if (clickedP.getDistanceFrom (pointOnPath) < cableThickness)
//        {
//            return true;
//        }
//    }
//
//    return false;
//}

void Cable::updateStartPoint (bool repaintIfMoved)
{
//    auto* startEditor = board->findEditorForProcessor (connectionInfo.startProc);
//    jassert (startEditor != nullptr);
//
//    scaleFactor = board->getScaleFactor();
    startColour = juce::Colours::black;
    cableThickness = getCableThickness();
//
//    const auto newPortLocation = CableViewPortLocationHelper::getPortLocation ({ startEditor, connectionInfo.startPort, false }).toFloat();
//    if (startPoint.load() != newPortLocation)
//    {
//        startPoint.store (newPortLocation);
        if (repaintIfMoved)
            repaintIfNeeded (true);
//    }
}


void Cable::mouseDrag (const MouseEvent& e)
{
    if (dragging)
    {
    cableView.dragConnector (e);
    }
    else if (e.mouseWasDraggedSinceMouseDown())
    {
    dragging = true;

//    graph.graph.removeConnection (connection);

    double distanceFromStart, distanceFromEnd;
    getDistancesFromEnds (getPosition().toFloat() + e.position, distanceFromStart, distanceFromEnd);
    const bool isNearerSource = (distanceFromStart < distanceFromEnd);

    AudioProcessorGraph::NodeAndChannel dummy { {}, 0 };

    cableView.beginConnectorDrag (isNearerSource ? dummy : connection.source,
    isNearerSource ? connection.destination : dummy,
    e);
    }
}

void Cable::mouseUp (const MouseEvent& e)
{
    if (dragging)
    cableView.endDraggingConnector (e);
}

void Cable::updateEndPoint (bool repaintIfMoved)
{
//    //DBG(cableView.getCableMousePosition().getX());
//    if (connection.endNode != nullptr)
//    {
////        auto* endEditor = board->findEditorForProcessor (connectionInfo.endProc);
////        endColour = endEditor->getColour();
////
////        const auto newPortLocation = CableViewPortLocationHelper::getPortLocation ({ endEditor, connectionInfo.endPort, true }).toFloat();
////        if (endPoint.load() != newPortLocation)
////        {
////            endPoint.store (newPortLocation);
////            if (repaintIfMoved)
////                repaintIfNeeded (true);
////        }
//    }
//    else if (cableView.cableBeingDragged())
//    {
//        endColour = startColour;
//        endPoint = cableView.getCableMousePosition();
//        DBG(" cable end point x " + String(cableView.getCableMousePosition().getX()) + " y"  +String(cableView.getCableMousePosition().getY ()));
//    }
//    else
//    {
//        // when the cable has just been created, we don't have the mouse drag
//        // source yet, so let's just set the end point equal to the start point.
//        endPoint.store (startPoint);
//    }
}

Path Cable::createCablePath (juce::Point<float> start, juce::Point<float> end, float sf)
{
    const auto pointOff = portOffset + sf;
    bezier = CubicBezier (start, start.translated (pointOff, 0.0f), end.translated (-pointOff, 0.0f), end);
    numPointsInPath = (int) start.getDistanceFrom (end) + 1;
    Path bezierPath;
    bezierPath.preallocateSpace ((numPointsInPath + 1) * 3);
    bezierPath.startNewSubPath (start);
    for (int i = 1; i < numPointsInPath; ++i)
    {
        const auto nextPoint = bezier.getPointOnCubicBezier ((float) i / (float) numPointsInPath);
        bezierPath.lineTo (nextPoint);
    }
    bezierPath.lineTo (end);

    return std::move (bezierPath);
}

void Cable::repaintIfNeeded (bool force)
{
//    const auto regeneratePath = [this]
//    {
//        auto createdPath = createCablePath (startPoint, endPoint, scaleFactor);
//        ScopedLock sl (pathCrit);
//        cablePath = std::move (createdPath);
//
//        const auto cableBounds = cablePath.getBounds().expanded (std::ceil (minCableThickness), std::ceil (2.0f * minCableThickness)).toNearestInt();
//        MessageManager::callAsync (
//                [safeComp = Component::SafePointer<Cable> (this), cableBounds]
//                {
//                    if (auto* comp = safeComp.getComponent())
//                        comp->redoImage();
//                        //comp->repaint (cableBounds);
//                });
//
//    };
//
//    if (force || connection.endNode == nullptr)
//    {
//        regeneratePath();
//        return;
//    }

//    if (connectionInfo.endProc != nullptr)
//    {
//        auto updatedLevelDB = jlimit (floorDB, 0.0f, connectionInfo.endProc->getInputLevelDB (connectionInfo.endPort));
//        auto levelDifference = std::abs (updatedLevelDB - levelDB);
//        if (std::abs (levelDifference) > 2.0f && levelRange.contains (updatedLevelDB) && levelRange.contains (levelDB))
//        {
//            levelDB = updatedLevelDB;
//            cableThickness = getCableThickness();
//            regeneratePath();
//        }
//    }
}

float Cable::getCableThickness() const
{
    auto levelMult = std::pow (jmap (levelDB, floorDB, 0.0f, 0.0f, 1.0f), 0.9f);
    return minCableThickness * (1.0f + 0.9f * levelMult);
}

void Cable::drawCableShadow (Graphics& g, float thickness)
{
    auto cableShadow = [this]
    {
        ScopedLock sl (pathCrit);
        return Path { linePath };
    }();
    cableShadow.applyTransform (AffineTransform::translation (0.0f, thickness * 0.6f));
    g.setColour (Colours::black.withAlpha (0.3f));
    g.strokePath (cableShadow, PathStrokeType (minCableThickness, PathStrokeType::JointStyle::curved));
}

void Cable::drawCableEndCircle (Graphics& g, juce::Point<float> centre, Colour colour) const
{
    auto circle = (juce::Rectangle { minCableThickness, minCableThickness } * 2.4f * scaleFactor.load()).withCentre (centre);
    g.setColour (colour);
    g.fillEllipse (circle);

    g.setColour (Colours::white);
    g.drawEllipse (circle, portCircleThickness);
}

void Cable::drawCable (Graphics& g, juce::Point<float> start, juce::Point<float> end)
{
    drawCableShadow (g, cableThickness);

//DBG("drawcable");
    g.setGradientFill (ColourGradient { startColour, start, endColour, end, false });
    {
        ScopedLock sl (pathCrit);
        g.strokePath (linePath, PathStrokeType (cableThickness, PathStrokeType::JointStyle::curved));
    }

    drawCableEndCircle (g, start, startColour);
    drawCableEndCircle (g, end, endColour);
}

void Cable::paint (Graphics& g)
{
    drawCable (g, startPoint, endPoint);
    //redoImage();
}

//void Cable::resized()
//{
//    updateStartPoint (false);
//    updateEndPoint (false);
//    repaintIfNeeded (true);
//}
