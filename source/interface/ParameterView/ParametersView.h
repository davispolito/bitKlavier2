#pragma once

#include <chowdsp_plugin_state/chowdsp_plugin_state.h>
#include "synth_section.h"
class OpenGlWrapper;

namespace bitklavier {
/** Clone of juce::GenericAudioProcessorEditor, but usable as a generic component */
    class ParametersView : public SynthSection {
    public:
        explicit ParametersView(chowdsp::PluginState &pluginState, chowdsp::ParamHolder &params, OpenGlWrapper *open_gl);

        ~ParametersView() override;

        void paint(juce::Graphics &) override;

        void resized() override;
        void initOpenGlComponents(OpenGlWrapper &open_gl) override;
        void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) override;
        void init_();
        void paintBackground(Graphics& g) override
        {
            SynthSection::paintContainer(g);
            paintHeadingText(g);
            paintBorder(g);
            paintKnobShadows(g);
            paintChildrenBackgrounds(g);
        }
    private:
        struct Pimpl;
        std::unique_ptr<Pimpl> pimpl;

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
