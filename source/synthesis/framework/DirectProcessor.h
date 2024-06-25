//
// Created by Davis Polito on 5/2/24.
//

#pragma once



#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>
#include <chowdsp_sources/chowdsp_sources.h>
#include <chowdsp_plugin_state/chowdsp_plugin_state.h>
struct DirectParams : chowdsp::ParamHolder
{

    // Adds the appropriate parameters to the Direct Processor
    DirectParams()
    {
        add (gainParam, hammerParam, velocityParam, resonanceParam, attackParam,
             decayParam, sustainParam, releaseParam);
    }

    // Gain param
    chowdsp::GainDBParameter::Ptr gainParam {
        juce::ParameterID { "gain", 100 },
        "Gain",
        juce::NormalisableRange { -30.0f, 0.0f },
        -24.0f
    };

    // Hammer param
    chowdsp::GainDBParameter::Ptr hammerParam {
            juce::ParameterID { "hammer", 100 },
            "Hammer",
            juce::NormalisableRange { -30.0f, 0.0f }, // FIX
            -24.0f
    };

    // Velocity param
    chowdsp::FloatParameter::Ptr velocityParam {
            juce::ParameterID { "Velocity", 100 },
            "Velocity",
            chowdsp::ParamUtils::createNormalisableRange (20.0f, 20000.0f, 2000.0f), // FIX
            1000.0f,
            &chowdsp::ParamUtils::floatValToString,
            &chowdsp::ParamUtils::stringToFloatVal
    };

    // Resonance param
    chowdsp::GainDBParameter::Ptr resonanceParam {
            juce::ParameterID { "resonance", 100 },
            "Resonance",
            juce::NormalisableRange { -30.0f, 0.0f }, // FIX
            -24.0f
    };

    // Attack param
    chowdsp::TimeMsParameter::Ptr attackParam {
            juce::ParameterID { "attack", 100 },
            "attack",
            chowdsp::ParamUtils::createNormalisableRange (2.01f, 10.0f, 4.0f),
            3.5f,
    };

    // Decay param
    chowdsp::TimeMsParameter::Ptr decayParam {
            juce::ParameterID { "decay", 100 },
            "Decay",
            chowdsp::ParamUtils::createNormalisableRange (2.01f, 10.0f, 4.0f), // FIX
            3.5f,
    };

    // Sustain param
    chowdsp::FloatParameter::Ptr sustainParam {
            juce::ParameterID { "sustain", 100 },
            "Sustain",
            chowdsp::ParamUtils::createNormalisableRange (20.0f, 20000.0f, 2000.0f),
            1000.0f,
            &chowdsp::ParamUtils::floatValToString,
            &chowdsp::ParamUtils::stringToFloatVal
    };

    // Release param
    chowdsp::TimeMsParameter::Ptr releaseParam {
            juce::ParameterID { "release", 100 },
            "Release",
            chowdsp::ParamUtils::createNormalisableRange (2.01f, 10.0f, 4.0f), // FIX
            3.5f,
    };

//    // Transpositions param
//    chowdsp::FloatParameter::Ptr transpositionsParam {
//            juce::ParameterID { "transpositions", 100 },
//            "Transpositions",
//            chowdsp::ParamUtils::createNormalisableRange (20.0f, 20000.0f, 2000.0f), // FIX
//            1000.0f,
//            &chowdsp::ParamUtils::floatValToString,
//            &chowdsp::ParamUtils::stringToFloatVal
//    };
//
//
//    // Blendronic Send param
//    chowdsp::GainDBParameter::Ptr blendronicSendParam {
//            juce::ParameterID { "blendronicSend", 100 },
//            "BlendronicSend",
//            juce::NormalisableRange { -30.0f, 0.0f }, // FIX
//            -24.0f
//    };


    /****************************************************************************************/
};

struct DirectNonParameterState : chowdsp::NonParamState
{
    DirectNonParameterState()
    {
        addStateValues ({ &prepPoint });
    }

    chowdsp::StateValue<juce::Point<int>> prepPoint { "prep_point", { 300, 500 } };
};

class DirectProcessor : public chowdsp::PluginBase<chowdsp::PluginStateImpl<DirectParams,DirectNonParameterState>>
{
public:
    DirectProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>& buffer) override;

    bool hasEditor() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }

private:
    //chowdsp::experimental::Directillator<float> oscillator;
    chowdsp::Gain<float> gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectProcessor)
};

