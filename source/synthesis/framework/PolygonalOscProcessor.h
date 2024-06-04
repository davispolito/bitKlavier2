//
// Created by Davis Polito on 5/2/24.
//

#ifndef BITKLAVIER2_POLYGONALOSCPROCESSOR_H
#define BITKLAVIER2_POLYGONALOSCPROCESSOR_H



#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>
#include <chowdsp_sources/chowdsp_sources.h>
#include <chowdsp_plugin_state/chowdsp_plugin_state.h>
struct PolygonOscParams : chowdsp::ParamHolder
{
    PolygonOscParams()
    {
        add (gainParam,
            freqParam,
            orderParam,
            teethParam);
    }

    chowdsp::GainDBParameter::Ptr gainParam {
        juce::ParameterID { "gain", 100 },
        "Gain",
        juce::NormalisableRange { -30.0f, 0.0f },
        -24.0f
    };

    chowdsp::FreqHzParameter::Ptr freqParam {
        juce::ParameterID { "freq", 100 },
        "Frequency",
        chowdsp::ParamUtils::createNormalisableRange (20.0f, 20000.0f, 2000.0f),
        1000.0f
    };

    chowdsp::FloatParameter::Ptr orderParam {
        juce::ParameterID { "order", 100 },
        "Order",
        chowdsp::ParamUtils::createNormalisableRange (2.01f, 10.0f, 4.0f),
        3.5f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

    chowdsp::PercentParameter::Ptr teethParam {
        juce::ParameterID { "teeth", 100 },
        "Teeth",
        0.0f
    };
};

struct PreparationNonParameterState : chowdsp::NonParamState
{
    PreparationNonParameterState()
    {
        addStateValues ({ &prepPoint });
    }

    chowdsp::StateValue<juce::Point<int>> prepPoint { "prep_point", { 300, 500 } };
};

class PolygonalOscProcessor : public chowdsp::PluginBase<chowdsp::PluginStateImpl<PolygonOscParams,PreparationNonParameterState>>
{
public:
    PolygonalOscProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>& buffer) override;

    bool hasEditor() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }

private:
    chowdsp::experimental::PolygonalOscillator<float> oscillator;
    chowdsp::Gain<float> gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolygonalOscProcessor)
};

#endif //BITKLAVIER2_POLYGONALOSCPROCESSOR_H
