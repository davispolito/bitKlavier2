//
// Created by Davis Polito on 6/28/24.
//

#include "CableView.h"

CableView::CableView (ConstructionSite &site) :site(site), pathTask (*this)
{
    setInterceptsMouseClicks (false, true);
    startTimerHz (36);

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

    if (e.mods.isAnyModifierKeyDown() || e.mods.isPopupMenu() || e.eventComponent == nullptr)
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
    if (isDraggingCable)
    {
//        if (connectionHelper->releaseCable (e))
//            cables.getLast()->updateEndPoint();
        isDraggingCable = false;
    }
}

void CableView::timerCallback()
{
    TRACE_COMPONENT();


//
//    // repaint port glow
//    if (mouseDraggingOverOutputPort() || mouseOverClickablePort())
//    {
//        portGlow = true;
//        repaint (getPortGlowBounds (portToPaint, scaleFactor).toNearestInt());
//    }
//    else if (portGlow)
//    {
//        portGlow = false;
//        repaint (getPortGlowBounds (portToPaint, scaleFactor).toNearestInt());
//    }

    if (isDraggingCable)
        updateCablePositions();
}

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
CableView::PathGeneratorTask::PathGeneratorTask (CableView& cv) : cableView (cv)
{
    sharedTimeSliceThread->addTimeSliceClient (this);

    if (! sharedTimeSliceThread->isThreadRunning())
        sharedTimeSliceThread->startThread();
}

CableView::PathGeneratorTask::~PathGeneratorTask()
{
    sharedTimeSliceThread->removeTimeSliceClient (this);
}

void CableView::updateCablePositions()
{
    for (auto* cable : cables)
    {
        cable->updateStartPoint();
        cable->updateEndPoint();
    }
}

int CableView::PathGeneratorTask::useTimeSlice()
{
    if (cableView.cableBeingDragged())
    {
        MessageManager::callAsync (
                [safeCableView = Component::SafePointer (&cableView)]
                {
                    if (auto* cv = safeCableView.getComponent())
                    {
                        ScopedLock sl (cv->cableMutex);
                        if (! cv->cables.isEmpty())
                            cv->cables.getLast()->repaint();
                    }
                });
    }

    ScopedLock sl (cableView.cableMutex);
    for (auto* cable : cableView.cables)
        cable->repaintIfNeeded();

    return 18; // a little less than 60 frames / second
}
