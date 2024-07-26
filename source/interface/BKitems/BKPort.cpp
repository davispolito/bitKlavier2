//
// Created by Davis Polito on 7/10/24.
//

#include "BKPort.h"
#include "paths.h"
BKPort::BKPort(bool isIn) : isInput(isIn), Button("port"), image_component_(new OpenGlImageComponent())

{
    setInterceptsMouseClicks(true, true);
    setAlwaysOnTop(true);
    image_component_->setComponent(this);
    auto path = Paths::portPaths();
    port_fill = path.getUnchecked(1);
    port_outline = path.getUnchecked(0);
}
//void BKPort::mouseDown(const MouseEvent& e)
//{
//    //getParentComponent()->mouseDown(e);]
//    for(auto listener : listeners_)
//    {
//        listener->portClicked(getBounds().getCentre());
//    }
//    DBG("Pdown");
//}
//
//void BKPort::mouseEnter(const MouseEvent& e)
//{
//    //getParentComponent()->mouseDown(e);
////    DBG("Popraat");
//}


//void BKPort::mouseDoubleClick(const MouseEvent& e)
//{
//
//}

void BKPort::mouseDown (const MouseEvent& e)
{
    DBG("borp");
    AudioProcessorGraph::NodeAndChannel dummy { {}, 0 };
    for (auto listener: listeners_)
    {
        listener->beginConnectorDrag(isInput ? dummy : pin,
                                 isInput ? pin : dummy,
                                 e);
    }

}

void BKPort::mouseDrag (const MouseEvent& e)
{
    for (auto listener : listeners_)
        listener->dragConnector (e);
}

void BKPort::mouseUp (const MouseEvent& e)
{
    for (auto listener : listeners_)
        listener->endDraggingConnector (e);
}