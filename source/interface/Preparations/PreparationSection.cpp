//
// Created by Davis Polito on 2/16/24.
//

#include "PreparationSection.h"
#include "synth_gui_interface.h"
#include "DirectPreparation.h"
PreparationSection::PreparationSection(String name, ValueTree v, UndoManager &um) : SynthSection(name), state(v)
{
    //_parent = findParentComponentOfClass<SynthGuiInterface>();
    x = v.getProperty(IDs::x);
    y = v.getProperty(IDs::y);

}

void PreparationSection::paintBackground(Graphics& g)
{
    g.saveState();
    Rectangle<int> bounds = getLocalArea(item.get(), item->getLocalBounds());
    g.reduceClipRegion(bounds);
    g.setOrigin(bounds.getTopLeft());
    //item->paintBackground(g);
    if(item)
    {item->redoImage();}
    g.restoreState();
}

void PreparationSection::resized()
{
    //Rectangle<float> bounds = getLocalBounds().toFloat();
    int item_padding_y = kItemPaddingY * size_ratio_;
    int item_height = 120 - 2 * item_padding_y;
    int item_padding_x = (120 - item_height) / 2;

    item->setBounds(item_padding_x, item_padding_y, 250, 250);
}
PreparationSection::~PreparationSection()
{
}

void PreparationSection::valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i)
{
    //if (v == state)
    //        if (i == IDs::name || i == IDs::colour)
    //            repaint();
}

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