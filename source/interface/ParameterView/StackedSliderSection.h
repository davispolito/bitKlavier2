//
// Created by Davis Polito on 11/25/24.
//

#ifndef BITKLAVIER2_STACKEDSLIDERSECTION_H
#define BITKLAVIER2_STACKEDSLIDERSECTION_H
#include "synth_section.h"
#include "OpenGlStackedSlider.h"
class StackSliderSection : public SynthSection
{
public:
    StackSliderSection(TransposeParams *params, chowdsp::ParameterListeners& listeners)
    : SynthSection("StackedSlider"), transpositionSlider(std::make_unique<OpenGlStackedSlider>(params,listeners)),
    on_(std::make_unique<SynthButton>("transpose_on")), on_attachment(params->onOff, listeners,*on_,nullptr)
    {
        setActivator(on_.get());
        addButton(on_.get());
        addOpenGlComponent(transpositionSlider->getImageComponent());
        addAndMakeVisible(transpositionSlider.get());
        on_->getGlComponent()->setAlwaysOnTop(true);
        on_->toFront(false);
    }
    void setActive(bool active) override
    {
        SynthSection::setActive(active);
        //transpositionSlider->
    }
    void paintBackground(juce::Graphics &g)
    {
        paintContainer(g);
       // paintHeadingText(g);


        paintChildrenBackgrounds(g);
        paintBorder(g);
    }
    void resized() override
    {
        SynthSection::resized();
        transpositionSlider->setBounds(activator_->getRight() + findValue(Skin::kWidgetMargin), 0, getLocalBounds().getWidth() - activator_->getWidth(), getLocalBounds().getHeight());
    }
    std::unique_ptr<SynthButton> on_;
    chowdsp::ButtonAttachment on_attachment;
    std::unique_ptr<OpenGlStackedSlider> transpositionSlider;
};
#endif //BITKLAVIER2_STACKEDSLIDERSECTION_H
