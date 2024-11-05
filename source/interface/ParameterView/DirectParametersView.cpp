//
// Created by Dan Trueman on 11/5/24.
//

#include "DirectParametersView.h"


void DirectParametersView::resized()
{
    DBG("--------" + getName() + "View -------------");
    DBG("bounds x:" + juce::String(getLocalBounds().getX()) + " y:" + juce::String(getLocalBounds().getY()) + " width: " + juce::String(getLocalBounds().getWidth()) + " height: " + juce::String(getLocalBounds().getHeight()));
    //pimpl->groupItem.setBounds(getLocalBounds());
    placeKnobsInArea(getLocalBounds(), comps);
    for(auto section: sub_sections_)
    {
        if(section->getName() == "ENV")
            section->setBounds(0,400,400,200);
    }
    //envelope->setBounds(0,400, 400, 200);
}