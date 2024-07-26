//
// Created by Davis Polito on 6/28/24.
//

#include "CableView.h"
#include "ConstructionSite.h"
CableView::CableView (ConstructionSite &site) :site(site), /*pathTask (*this),*/ SynthSection("cableView")
{
    //setInterceptsMouseClicks (false, true);
    //startTimerHz (36);

}

CableView::~CableView() = default;

//void CableView::portClicked(const juce::Point<int>& pos, juce::AudioProcessorGraph::Node::Ptr node){
//
//
//}
bool CableView::mouseOverClickablePort()
{
    if (! mousePosition.has_value())
        return false;

#if ! JUCE_IOS
    if (isDraggingCable)
        return false;
#endif

//    const auto nearestPort = portLocationHelper->getNearestPort (*mousePosition, &board);
//    if (nearestPort.editor == nullptr)
//        return false;



//    const auto hoveringNearConnectedInput = nearestPort.isInput && portLocationHelper->isInputPortConnected (nearestPort);
//    const auto hoveringNearOutput = ! nearestPort.isInput;
//    if (hoveringNearConnectedInput || hoveringNearOutput)
//    {
//        portToPaint = CableViewPortLocationHelper::getPortLocation (nearestPort);
//        return true;
//    }

    return false;
}

bool CableView::mouseDraggingOverOutputPort()
{
    if (! mousePosition.has_value() || ! isDraggingCable)
        return false;

//    const auto nearestInputPort = portLocationHelper->getNearestInputPort (*mousePosition, cables.getLast()->connectionInfo.startProc);
//    if (nearestInputPort.editor != nullptr && nearestInputPort.isInput && ! portLocationHelper->isInputPortConnected (nearestInputPort))
//    {
//        portToPaint = CableViewPortLocationHelper::getPortLocation (nearestInputPort);
//        return true;
//    }

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

    DBG("portclickedcableview");
    DBG("position x " + String(e.getPosition().getX()) + " y : " + String(e.getPosition().getY()));

//    ScopedLock sl (cableMutex);

    addChildComponent(draggingConnector.get(), 0);
    //cables.getLast()->setBounds (getLocalBounds());
    //isDraggingCable = true;
    addOpenGlComponent(draggingConnector->getImageComponent(), false, false);
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
    draggingConnector->toFront (false);

    dragConnector (e);
}

void CableView::paint (Graphics& g)
{
//    TRACE_COMPONENT();



//    if (portGlow)
//    {
//        drawCablePortGlow (g, portToPaint, scaleFactor);
//    }
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
//    TRACE_COMPONENT();
    DBG("cabledowns");
    DBG(currentPort.getX());
    if (!e.mods.isCommandDown() || e.mods.isPopupMenu() || e.eventComponent == nullptr)
        return; // not a valid mouse event


//    const auto nearestPort = portLocationHelper->getNearestPort (e.getEventRelativeTo (this).getMouseDownPosition(), e.source.getComponentUnderMouse());
//    if (nearestPort.editor == nullptr)
//        return; // no nearest port
//
//    if (nearestPort.isInput)
//    {
//        connectionHelper->destroyCable (nearestPort.editor->getProcPtr(), nearestPort.portIndex);
//    }
//    else
//    {
//        connectionHelper->createCable ({ nearestPort.editor->getProcPtr(), nearestPort.portIndex, nullptr, 0 });
//        isDraggingCable = true;
//    }
}

void CableView::mouseDrag (const MouseEvent& e)
{
//    TRACE_COMPONENT();


    if (e.eventComponent == nullptr)
        return;
    mousePosition = e.getEventRelativeTo (this).getPosition();
    const auto eventCompName = e.eventComponent->getName();
//    if (sst::cpputils::contains (std::initializer_list<std::string_view> { "Port", "Board", Cable::componentName },
//                                 std::string_view { eventCompName.getCharPointer(), (size_t) eventCompName.length() }))
//    {
//        mousePosition = e.getEventRelativeTo (this).getPosition();
//    }
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
//            ScopedLock sl (cableMutex);
//            site.open_gl.initOpenGlComp.try_enqueue([this]
//                                                    {
//                                                        auto comp = std::remove(open_gl_components_.begin(), open_gl_components_.end(),cables.getLast()->getImageComponent());
//                                                        delete *comp;
//                                                        MessageManager::callAsync (
//                                                                [safeComp = Component::SafePointer<CableView> (this)]
//                                                                {
//                                                                    safeComp->cables.removeObject (safeComp->cables.getLast());
//
//                                                                    //comp->repaint (cableBounds);
//                                                                });
//
//                                                    });
//
//            std::remove(open_gl_components_.begin(), open_gl_components_.end(),cables.getLast()->getImageComponent());
//                                                        cables.removeObject (cables.getLast());
        }
       //repaint();]
//        if (connectionHelper->releaseCable (e))
            //cables.getLast()->updateEndPoint();
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

//        graph.graph.addConnection (connection);
    }
}
//void CableView::timerCallback()
//{
//    TRACE_COMPONENT();
//
//
////
////    // repaint port glow
////    if (mouseDraggingOverOutputPort() || mouseOverClickablePort())
////    {
////        portGlow = true;
////        repaint (getPortGlowBounds (portToPaint, scaleFactor).toNearestInt());
////    }
////    else if (portGlow)
////    {
////        portGlow = false;
////        repaint (getPortGlowBounds (portToPaint, scaleFactor).toNearestInt());
////    }
//
//    if (isDraggingCable)
//        updateCablePositions();
//
//}

//void CableView::processorBeingAdded (BaseProcessor* newProc)
//{
//    //connectionHelper->processorBeingAdded (newProc);
//}
//
//void CableView::processorBeingRemoved (const BaseProcessor* proc)
//{
//    //connectionHelper->processorBeingRemoved (proc);
//}

//================================================================
//CableView::PathGeneratorTask::PathGeneratorTask (CableView& cv) : cableView (cv)
//{
//    sharedTimeSliceThread->addTimeSliceClient (this);
//
//    if (! sharedTimeSliceThread->isThreadRunning())
//        sharedTimeSliceThread->startThread();
//}
////
//CableView::PathGeneratorTask::~PathGeneratorTask()
//{
//    sharedTimeSliceThread->removeTimeSliceClient (this);
//}

void CableView::updateCablePositions()
{
    for (auto* cable : cables)
    {
        cable->updateStartPoint();
        cable->updateEndPoint();
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


//int CableView::PathGeneratorTask::useTimeSlice()
//{
////    if (cableView.cableBeingDragged())
////    {
////        MessageManager::callAsync (
////                [safeCableView = Component::SafePointer (&cableView)]
////                {
////                    if (auto* cv = safeCableView.getComponent())
////                    {
////                        ScopedLock sl (cv->cableMutex);
////                        if (! cv->cables.isEmpty())
////                            cv->cables.getLast()->redoImage();
////                            //cv->cables.getLast()->repaint();
////                    }
////                });
////    }
////
////    ScopedLock sl (cableView.cableMutex);
////    for (auto* cable : cableView.cables)
////        cable->repaintIfNeeded();
//
//    return 18; // a little less than 60 frames / second
//}
