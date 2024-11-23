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

    juce::Rectangle<int> knobs_area(title_width, knob_y, area_width, knob_section_height);
    knobs_area.setSize(knobs_area.getWidth() * getSizeRatio(), knobs_area.getHeight() * getSizeRatio());
    placeKnobsInArea(knobs_area, comps);

    for(auto section: sub_sections_)
    {
        if(section->getName() == "ENV")
            section->setBounds(title_width, knob_section_height, area_width, knob_section_height * 4);
    }

     transpose_uses_tuning->setBounds(area_width - title_width, 0 + knob_section_height * 0.5, 100, knob_section_height * 0.5);
    transpositionSlider->setBounds(title_width, 0, area_width, knob_section_height);


    SynthSection::resized();
}