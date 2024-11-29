#include "ParametersView.h"
#include "synth_section.h"
#include "synth_slider.h"
#include "open_gl_background.h"

#include "envelope_section.h"
namespace bitklavier {
 parameters_view_detail::SliderParameterComponent::SliderParameterComponent(chowdsp::FloatParameter &param,
                                                                            chowdsp::ParameterListeners &listeners,
                                                                            SynthSection &parent) :
                                                                            slider(std::make_shared<SynthSlider>(param.paramID)),
                                                                            attachment(param, listeners, *slider, nullptr) {
     //setName(param.paramID);
     //setLookAndFeel(DefaultLookAndFeel::instance());
     slider->setScrollWheelEnabled(false);
     //addAndMakeVisible(*slider);
     parent.addSlider(slider.get(), true);
     slider->parentHierarchyChanged();
     slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
     _ASSERT(slider->getSectionParent() != nullptr);
     DBG("SliderParameterComponent: create slider for " + param.paramID + " with parent " + parent.getName());
 }
    std::unique_ptr<SynthSection> parameters_view_detail::createEditorSection(chowdsp::ParamHolder &params, chowdsp::ParameterListeners& listeners, SynthSection &parent)
{

     if (auto *envParams = dynamic_cast<EnvParams*>(&params))
         return std::make_unique<EnvelopeSection>("ENV", "ENV",*envParams,listeners, parent);//std::make_unique<BooleanParameterComponent>(*boolParam, listeners);

     return {}; // std::make_unique<ParameterGroupItem>(params,listeners, parent);

 }
//==============================================================================
    ParametersView::ParametersView(chowdsp::PluginState &pluginState, chowdsp::ParamHolder &params, OpenGlWrapper *open_gl)
            : ParametersView (pluginState.getParameterListeners(), params, open_gl) {}

    ParametersView::ParametersView(chowdsp::ParameterListeners& paramListeners, chowdsp::ParamHolder& params, OpenGlWrapper *open_gl)
            :  SynthSection(params.getName(), open_gl)
    {
        params.doForAllParameterContainers(
            [this, &paramListeners](auto &paramVec)
            {
                DBG("----paramvec----");
                for (auto &param : paramVec)
                    createParameterComp(paramListeners, param,*this);
            },
            [this, &paramListeners](auto &paramHolder) {
                DBG("ParametersView: paramholder name " + paramHolder.getName());
                auto section  = parameters_view_detail::createEditorSection(paramHolder,paramListeners,*this);
                addSubSection(section.get());
                paramHolderComps.push_back(std::move(section));
            });

        setLookAndFeel(DefaultLookAndFeel::instance());
        setOpaque(true);
    }

    ParametersView::~ParametersView(){}

    void ParametersView::paint(juce::Graphics &g) {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void ParametersView::resized() {
        DBG("--------" + getName() + "View -------------");
        DBG("bounds x:" + juce::String(getLocalBounds().getX()) + " y:" + juce::String(getLocalBounds().getY()) + " width: " + juce::String(getLocalBounds().getWidth()) + " height: " + juce::String(getLocalBounds().getHeight()));
        //pimpl->groupItem.setBounds(getLocalBounds());
        placeKnobsInArea(getLocalBounds(), slider_pairs);


    }



}//naemspace bitlkavier