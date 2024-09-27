//
// Created by Davis Polito on 7/10/24.
//

#include "BKPort.h"
#include "paths.h"
#include "synth_gui_interface.h"
#include "Identifiers.h"
#include "valuetree_utils/VariantConverters.h"
#include "look_and_feel/default_look_and_feel.h"
BKPort::BKPort(SynthGuiInterface* _parent, ValueTree v) :
Button("port"),
state(v),
image_component_(new OpenGlImageComponent())

{
    setLookAndFeel(DefaultLookAndFeel::instance());
    pin = AudioProcessorGraph::NodeAndChannel{VariantConverter<juce::AudioProcessorGraph::NodeID>::fromVar(v.getProperty(IDs::nodeID)), v.getProperty(IDs::chIdx)};

    isInput.referTo(v, IDs::isIn, nullptr);
    setInterceptsMouseClicks(true, true);
    setAlwaysOnTop(true);
    image_component_->setComponent(this);
    auto path = Paths::portPaths();
    port_fill = path.getUnchecked(1);
    port_outline = path.getUnchecked(0);

//    if (auto node = _parent->getSynth()->getNodeForId(pin.nodeID))
//    {
//        String tip;
//
//        if (pin.isMIDI())
//        {
//            tip = isInput ? "MIDI Input"
//                          : "MIDI Output";
//        }
//        else
//        {
//            auto& processor = *node->getProcessor();
//            auto channel = processor.getOffsetInBusBufferForAbsoluteChannelIndex (isInput, pin.channelIndex, busIdx);
//
//            if (auto* bus = processor.getBus (isInput, busIdx))
//                tip = bus->getName() + ": " + AudioChannelSet::getAbbreviatedChannelTypeName (bus->getCurrentLayout().getTypeOfChannel (channel));
//            else
//                tip = (isInput ? "Main Input: "
//                               : "Main Output: ") + String (pin.channelIndex + 1);
//
//        }

//        setTooltip (tip);
//    }
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