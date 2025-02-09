#pragma once

#include <chowdsp_plugin_state/chowdsp_plugin_state.h>
#include "synth_section.h"
struct OpenGlWrapper;

namespace bitklavier {
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
    protected:
        //chowdsp::PluginState& pluginState;
        std::vector<std::unique_ptr<juce::Component>> comps;
        std::vector<std::unique_ptr<juce::Component>> paramHolderComps;
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
