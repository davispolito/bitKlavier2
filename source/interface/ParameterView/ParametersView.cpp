#include "ParametersView.h"
#include "synth_section.h"
#include "synth_slider.h"
#include "open_gl_background.h"
#include "envelope_section.h"

namespace bitklavier
{
    parameters_view_detail::SliderParameterComponent::SliderParameterComponent (
                chowdsp::FloatParameter &param,
                chowdsp::ParameterListeners &listeners,
                SynthSection &parent) :
                slider(std::make_shared<SynthSlider>(param.paramID)),
                attachment(param, listeners, *slider, nullptr)
    {
         slider->setScrollWheelEnabled(false);
         parent.addSlider(slider.get(), true);
         slider->parentHierarchyChanged();
         slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
         _ASSERT(slider->getSectionParent() != nullptr);
         DBG("SliderParameterComponent: create slider for " + param.paramID + " with parent " + parent.getName());
    }

    std::unique_ptr<SynthSection> parameters_view_detail::createEditorSection (
                    chowdsp::ParamHolder &params,
                    chowdsp::ParameterListeners& listeners,
                    SynthSection &parent)
    {
         if (auto *envParams = dynamic_cast<EnvParams*>(&params))
             return std::make_unique<EnvelopeSection>("ENV", "ENV",*envParams,listeners, parent);

         return {};
    }

    //==============================================================================
    ParametersView::ParametersView (
        chowdsp::PluginState &pluginState,
        chowdsp::ParamHolder &params,
        OpenGlWrapper *open_gl)
            : ParametersView (
              pluginState.getParameterListeners(),
              params,
              open_gl)
    {}

    ParametersView::ParametersView (
        chowdsp::ParameterListeners& paramListeners,
        chowdsp::ParamHolder& params,
        OpenGlWrapper *open_gl)
            :  SynthSection(
              params.getName(),
              open_gl)
    {
        params.doForAllParameterContainers (
            [this, &paramListeners](auto &paramVec)
            {
                DBG("----paramvec----");
                for (auto &param : paramVec)
                    createParameterComp(paramListeners, param,*this);
            },
            [this, &paramListeners](auto &paramHolder)
            {
                DBG("ParametersView: paramholder name " + paramHolder.getName());
                auto section= parameters_view_detail::createEditorSection (
                    paramHolder,
                    paramListeners,
                    *this);
                addSubSection(section.get());
                paramHolderComps.push_back(std::move(section));

                if(auto *velRangeParams = dynamic_cast<RangeSliderParams*>(&paramHolder))
                {
                    DBG("ParametersView::ParametersView making velocityRangeSlider");
                    velocityRangeSlider = std::make_unique<OpenGL_RangeSlider>(velRangeParams, paramListeners);
                }
            }

        );

        setLookAndFeel(DefaultLookAndFeel::instance());
        setOpaque(true);

        addAndMakeVisible(*velocityRangeSlider);
        addOpenGlComponent(velocityRangeSlider->getImageComponent(), true);
    }

    ParametersView::~ParametersView(){}

    void ParametersView::paint(juce::Graphics &g)
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void ParametersView::resized(){}
//    void ParametersView::resized()
//    {
//        DBG("--------" + getName() + "View -------------");
//        DBG("bounds x:" + juce::String(getLocalBounds().getX()) + " y:" + juce::String(getLocalBounds().getY()) + " width: " + juce::String(getLocalBounds().getWidth()) + " height: " + juce::String(getLocalBounds().getHeight()));
//        //pimpl->groupItem.setBounds(getLocalBounds());
//        placeKnobsInArea(getLocalBounds(), slider_pairs);
//
//        int knob_section_height = getKnobSectionHeight();
//        int title_width = getTitleWidth();
//        int area_width = getWidth() - 2 * title_width;
//        int area_height = getHeight();
//
//        juce::Rectangle<int> knobs_area(title_width, area_height - knob_section_height, area_width, knob_section_height);
//        knobs_area.setSize(knobs_area.getWidth() * getSizeRatio(), knobs_area.getHeight() * getSizeRatio());
//
//        velocityRangeSlider->setBounds(
//            title_width,
//            knobs_area.getY() - knobs_area.getHeight(),
//            knobs_area.getWidth(),
//            knobs_area.getHeight() );
//
//        velocityRangeSlider->setBounds(knobs_area);
//
//        SynthSection::resized();
//    }

}   //namespace bitklavier