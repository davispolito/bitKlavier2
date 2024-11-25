#pragma once

#include <chowdsp_plugin_state/chowdsp_plugin_state.h>
#include "synth_section.h"
struct OpenGlWrapper;

namespace bitklavier {
    namespace parameters_view_detail {

        //==============================================================================
        struct BooleanParameterComponent  {
        public:
            BooleanParameterComponent(chowdsp::BoolParameter &param, chowdsp::ParameterListeners& listeners, SynthSection &parent)
                    : button(std::make_shared<SynthButton>(param.paramID)), attachment(param, listeners, *button, nullptr) {
                parent.addButton(button.get());

            }

            std::shared_ptr<SynthButton> button;
            chowdsp::ButtonAttachment attachment;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BooleanParameterComponent)
        };



        struct SliderParameterComponent {
            SliderParameterComponent(chowdsp::FloatParameter &param, chowdsp::ParameterListeners& listeners, SynthSection &parent);

            std::shared_ptr<SynthSlider> slider;
            chowdsp::SliderAttachment attachment;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderParameterComponent)
        };



        std::unique_ptr<SynthSection> createEditorSection(chowdsp::ParamHolder &params, chowdsp::ParameterListeners& listeners, SynthSection &parent);
    } // namespace parameters_view_detail


    /** Clone of juce::GenericAudioProcessorEditor, but usable as a generic component */
    class  ParametersView : public SynthSection {
    public:
        ParametersView (chowdsp::PluginState& pluginState, chowdsp::ParamHolder& params, OpenGlWrapper *open_gl);
        ParametersView (chowdsp::ParameterListeners& paramListeners, chowdsp::ParamHolder& params, OpenGlWrapper *open_gl);
        ~ParametersView() override;

        void paint(juce::Graphics &) override;

        void resized() override;
//        void initOpenGlComponents(OpenGlWrapper &open_gl) override;
//        void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) override;
        void init_();
        /** Returns nullptr if no component is found for the given parameter */
        [[nodiscard]] juce::Component* getComponentForParameter (const juce::RangedAudioParameter&);
        void paintBackground(juce::Graphics& g) override
        {
            SynthSection::paintContainer(g);
            paintHeadingText(g);
            paintBorder(g);
            paintKnobShadows(g);
            for (auto slider : all_sliders_v) {
                drawLabelForComponent(g, slider->getName(), slider);
            }
            paintChildrenBackgrounds(g);
        }
    private:
        void createParameterComp(chowdsp::ParameterListeners& listeners, juce::RangedAudioParameter &parameter, SynthSection& parent) {
            if (auto *boolParam = dynamic_cast<chowdsp::BoolParameter *> (&parameter))
                boolean_pairs.push_back(std::make_unique<parameters_view_detail::BooleanParameterComponent>(*boolParam, listeners,*this));


            if (auto *sliderParam = dynamic_cast<chowdsp::FloatParameter *> (&parameter))
                slider_pairs.push_back(std::make_unique<parameters_view_detail::SliderParameterComponent>(*sliderParam, listeners,*this));
        }
    protected:
        //chowdsp::PluginState& pluginState;
//        std::vector<std::pair<std::shared_ptr<SynthSlider>,chowdsp::SliderAttachment >> slider_pairs;
//        std::vector<std::pair<std::shared_ptr<SynthButton>,chowdsp::ButtonAttachment >> button_pairs;
        std::vector<std::unique_ptr<parameters_view_detail::SliderParameterComponent>> slider_pairs;
        std::vector<std::unique_ptr<parameters_view_detail::BooleanParameterComponent>> boolean_pairs;
        std::vector<std::unique_ptr<SynthSection>> paramHolderComps;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametersView)
    };

///** Clone of juce::GenericAudioProcessorEditor. */
//    class ParametersViewEditor : public juce::AudioProcessorEditor {
//    public:
//        ParametersViewEditor(juce::AudioProcessor &proc, chowdsp::PluginState &pluginState,
//                             chowdsp::ParamHolder &params)
//                : juce::AudioProcessorEditor(proc),
//                  view(pluginState, params, ) {
//            setResizable(true, false);
//            setSize(view.getWidth(), view.getHeight());
//
//            addAndMakeVisible(view);
//        }
//
//        void resized() override {
//            view.setBounds(getLocalBounds());
//        }
//
//    private:
//        ParametersView view;
//
//        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametersViewEditor)
//    };
}//namespace bitilavier
