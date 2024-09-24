//
// Created by Davis Polito on 2/16/24.
//

#include "PreparationSection.h"
#include "synth_gui_interface.h"
#include "DirectPreparation.h"
PreparationSection::PreparationSection(String name, ValueTree v, OpenGlWrapper &open_gl) : SynthSection(name), state(v), _open_gl(open_gl)
{
    //_parent = findParentComponentOfClass<SynthGuiInterface>();
    x.referTo(v,IDs::x,nullptr);
    y.referTo(v,IDs::y,nullptr);
    width.referTo(v,IDs::width,nullptr);
    height.referTo(v,IDs::height,nullptr);
    numIns.referTo(v,IDs::numIns,nullptr);
    numOuts.referTo(v,IDs::numOuts,nullptr);
    //constrainer.setBoundsForComponent(this,getParentComponent()->getLocalBounds() );
    createUuidProperty(state);
    uuid.referTo(state,IDs::uuid,nullptr);

    //pluginID.uid = static_cast<uint32>(int(state.getProperty(IDs::uuid)));
    constrainer.setMinimumOnscreenAmounts(0xffffff,0xffffff,0xffffff,0xffffff);
}

void PreparationSection::paintBackground(Graphics& g)
{
//    g.saveState();
//    Rectangle<int> bounds = getLocalArea(item.get(), item->getLocalBounds());
//    g.reduceClipRegion(bounds);
//    g.setOrigin(bounds.getTopLeft());
//    //item->paintBackground(g);
    if(item)
    {
        //item->setColor(findColour (Skin::kWidgetPrimary1, true));
        item->redoImage();
    }


//    g.restoreState();
}

void PreparationSection:: resized() {
    Rectangle<float> bounds = getLocalBounds().toFloat();
    int item_padding_y = kItemPaddingY * size_ratio_;
    int item_height = getHeight() - 2 * item_padding_y;
    int item_padding_x = kItemPaddingX * size_ratio_;
    int item_width = getWidth() - 2 * item_padding_x;
    auto newBounds = getBoundsInParent();

    item->setBounds(item_padding_x, item_padding_y, item_width, item_height);

    if (auto *processor = getProcessor())
    {
        for (auto *port: ports) {
            const bool isInput = port->isInput;
            auto channelIndex = port->pin.channelIndex;
            int busIdx = 0;
            processor->getOffsetInBusBufferForAbsoluteChannelIndex(isInput, channelIndex, busIdx);

            int total = isInput ? numIns : numOuts;
            int index = port->pin.isMIDI() ? (total - 1) : channelIndex;

            auto totalSpaces = static_cast<float> (total) +
                               (static_cast<float> (jmax(0, processor->getBusCount(isInput) - 1)) * 0.5f);
            auto indexPos = static_cast<float> (index) + (static_cast<float> (busIdx) * 0.5f);
            if( port->pin.isMIDI())
            {
                port->setBounds((proportionOfWidth((1.0f + indexPos) / (totalSpaces + 1.0f)) + portSize ) * 0.9,
                                port->isInput ? floor((getHeight() * 0.9) - portSize / 2):(0 + item_padding_y) ,
                                portSize, portSize);
            }
            else {
                port->setBounds(port->isInput ? floor(item_padding_x + portSize/2 * 0.9) - 1: (getWidth() * 0.9),
                                proportionOfHeight((1.0f + indexPos) / (totalSpaces + 1.0f)) - portSize / 2,
                                portSize, portSize);
            }
        }
    }

    SynthSection::resized();
}
PreparationSection::~PreparationSection()
{
}

//void PreparationSection::valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i)
//{
//    //if (v == state)
//    //        if (i == IDs::name || i == IDs::colour)
//    //            repaint();
//}

//PreparationSection* PreparationList::createNewObject (const juce::ValueTree& v)
//{
//
//        auto s = prepFactory.CreateObject((int)v.getProperty(IDs::type), v, undo);
//        addSubSection(s);
//        return s;
//
//
//}

//PreparationList::PreparationList(ValueTree editTree, juce::UndoManager& um)
//    : SynthSection("preplist"),
//      tracktion::engine::ValueTreeObjectList<PreparationSection>(editTree),
//      undo(um)
//{
//
//
//}

//void PreparationSection::mouseDoubleClick(const juce::MouseEvent &event)
//{
//    showPrepPopup(this);
//}