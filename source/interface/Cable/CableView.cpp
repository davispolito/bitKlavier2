//
// Created by Davis Polito on 6/28/24.
//

#include "CableView.h"
#include "ConstructionSite.h"
#include "sound_engine.h"

CableView::CableView (ConstructionSite &site) :site(site), /*pathTask (*this),*/ SynthSection("cableView")
{
    setInterceptsMouseClicks (false,false);
    //startTimerHz (36);
    setAlwaysOnTop(true);
}

CableView::~CableView() = default;


bool CableView::mouseOverClickablePort()
{
    if (! mousePosition.has_value())
        return false;

#if ! JUCE_IOS
    if (isDraggingCable)
        return false;
#endif

    return false;
}

bool CableView::mouseDraggingOverOutputPort()
{
    if (! mousePosition.has_value() || ! isDraggingCable)
        return false;

    return false;
}

void CableView::beginConnectorDrag (AudioProcessorGraph::NodeAndChannel source,
                         AudioProcessorGraph::NodeAndChannel dest,
                         const MouseEvent& e)
{
    auto* c = dynamic_cast<Cable*> (e.originalComponent);
    cables.removeObject (c, false);
    draggingConnector.reset (c);

    if (draggingConnector == nullptr)
        draggingConnector.reset (new Cable (&site, *this));

    addChildComponent(draggingConnector.get(), 0);

    addOpenGlComponent(draggingConnector->getImageComponent(), true, false);
    site.open_gl.initOpenGlComp.try_enqueue([this] {
        draggingConnector->getImageComponent()->init(site.open_gl);
        MessageManager::callAsync(
            [safeComp = Component::SafePointer<Cable>(draggingConnector.get())] {
                if (auto *comp = safeComp.getComponent()) {
                comp->setVisible(true);
               comp->getImageComponent()->setVisible(true);
                }
            });
    });
    draggingConnector->setInput (source);
    draggingConnector->setOutput (dest);

    addAndMakeVisible (draggingConnector.get());
    //draggingConnector->toFront (false);

    dragConnector (e);
}

void CableView::paint (Graphics& g)
{

}

void CableView::resized()
{
    for (auto* cable : cables)
        cable->setBounds (getLocalBounds());
}

void CableView::mouseMove (const MouseEvent& e)
{
    mousePosition = e.getEventRelativeTo (this).getPosition();
}

void CableView::mouseExit (const MouseEvent&)
{
    mousePosition = std::nullopt;
}

void CableView::mouseDown (const MouseEvent& e)
{

    DBG("cabledowns");
    DBG(currentPort.getX());
    if (!e.mods.isCommandDown() || e.mods.isPopupMenu() || e.eventComponent == nullptr)
        return; // not a valid mouse event

}

void CableView::mouseDrag (const MouseEvent& e)
{

    if (e.eventComponent == nullptr)
        return;
    mousePosition = e.getEventRelativeTo (this).getPosition();
    const auto eventCompName = e.eventComponent->getName();

}

bool CableView::cableBeingDragged() const
{
    return isDraggingCable;
}

juce::Point<float> CableView::getCableMousePosition() const
{
    return mousePosition.value_or (juce::Point<int> {}).toFloat();
}

void CableView::mouseUp (const MouseEvent& e)
{
    //DBG("mousei[can");
    if (isDraggingCable)
    {
        auto relMouse = e.getEventRelativeTo (this);
        auto mousePos = relMouse.getPosition();

        auto* cable = cables.getLast();
        // not being connected... trash the latest cable
        {
            DBG("delete cable");
        }
        isDraggingCable = false;
    }
}


void CableView::endDraggingConnector (const MouseEvent& e)
{
    if (draggingConnector == nullptr)
        return;

    //draggingConnector->setTooltip ({});

    auto e2 = e.getEventRelativeTo (this);
    auto connection = draggingConnector->connection;
    objectToDelete = draggingConnector->getImageComponent().get();

    draggingConnector = nullptr;
    site.open_gl.initOpenGlComp.try_enqueue([this]{
        destroyOpenGlComponent(*objectToDelete, site.open_gl);
        objectToDelete = nullptr;
    });
    if (auto* pin = site.findPinAt (e2.position))
    {
        if (connection.source.nodeID == AudioProcessorGraph::NodeID())
        {
            if (pin->isInput)
                return;

            connection.source = pin->pin;
        }
        else
        {
            if (! pin->isInput)
                return;

            connection.destination = pin->pin;
        }
//// add connection to actual rendered connections
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        parent->getSynth()->addConnection(connection);
//        graph.graph.addConnection (connection);
    }
}


void CableView::updateCablePositions()
{
    for (auto* cable : cables)
    {
        cable->updateStartPoint();
        cable->updateEndPoint();
    }
}

void CableView::updateComponents()
{
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    for (int i = cables.size(); --i >= 0;)
    {
        if (! parent->getSynth()->getEngine()->processorGraph->isConnected (cables.getUnchecked (i)->connection))
        {
            cables.remove (i);
        }
    }

    for (auto* cc : cables)
        cc->update();

    for (auto& c : parent->getSynth()->getEngine()->processorGraph->getConnections())
    {
        if (getComponentForConnection (c) == nullptr)
        {
            auto* comp = cables.add (new Cable(&site, *this));
            addChildComponent(comp, 0);

            addOpenGlComponent(comp->getImageComponent(), true, false);
            site.open_gl.initOpenGlComp.try_enqueue([this, comp] {
                comp->getImageComponent()->init(site.open_gl);
                MessageManager::callAsync(
                        [safeComp = Component::SafePointer<Cable>(comp)] {
                            if (auto *_comp = safeComp.getComponent()) {
                                _comp->setVisible(true);
                                _comp->getImageComponent()->setVisible(true);
                            }
                        });
            });
            addAndMakeVisible (comp);

            comp->setInput (c.source);
            comp->setOutput (c.destination);
        }
    }
}

void CableView::dragConnector(const MouseEvent& e)
{

        auto e2 = e.getEventRelativeTo (this);

        if (draggingConnector != nullptr)
        {
            //draggingConnector->setTooltip ({});

            auto pos = e2.position;

            if (auto* pin = site.findPinAt (pos))
            {
                auto connection = draggingConnector->connection;

                if (connection.source.nodeID == AudioProcessorGraph::NodeID() && ! pin->isInput)
                {
                    connection.source = pin->pin;
                }
                else if (connection.destination.nodeID == AudioProcessorGraph::NodeID() && pin->isInput)
                {
                    connection.destination = pin->pin;
                }

//                if (graph.graph.canConnect (connection))
//                {
//                    pos = (pin->getParentComponent()->getPosition() + pin->getBounds().getCentre()).toFloat();
//                    draggingConnector->setTooltip (pin->getTooltip());
//                }
            }

            if (draggingConnector->connection.source.nodeID == AudioProcessorGraph::NodeID())
                draggingConnector->dragStart (pos);
            else
                draggingConnector->dragEnd (pos);
        }
}


void CableView::_update()
{
    updateComponents();
}


