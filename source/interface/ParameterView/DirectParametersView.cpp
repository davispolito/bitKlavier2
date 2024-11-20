//
// Created by Dan Trueman on 11/5/24.
//

#include "DirectParametersView.h"


void DirectParametersView::resized()
{
    DBG("--------" + getName() + "View -------------");
    DBG("bounds x:" + juce::String(getLocalBounds().getX()) + " y:" + juce::String(getLocalBounds().getY()) + " width: " + juce::String(getLocalBounds().getWidth()) + " height: " + juce::String(getLocalBounds().getHeight()));
    //pimpl->groupItem.setBounds(getLocalBounds());

    int knob_section_height = getKnobSectionHeight();
    int knob_y = getHeight() - knob_section_height;

    int widget_margin = findValue(Skin::kWidgetMargin);
    int title_width = getTitleWidth();
    int area_width = getWidth() - 2 * title_width;
    int envelope_height = knob_y - widget_margin;
    //envelope_->setBounds(widget_margin, widget_margin, getWidth() - 2 * widget_margin, envelope_height);

    juce::Rectangle<int> knobs_area(title_width, knob_y, area_width, knob_section_height);
    placeKnobsInArea(knobs_area, comps);



    //placeKnobsInArea(getLocalBounds(), comps);
    for(auto section: sub_sections_)
    {
        if(section->getName() == "ENV")
            section->setBounds(title_width, knob_section_height, area_width, knob_section_height * 4);
    }

    //transpositionSlider->setBounds(0, 200, 400, 200);
    transpositionSlider->setBounds(title_width, 0, area_width, knob_section_height);
    //envelope->setBounds(0,400, 400, 200);


    SynthSection::resized();
}