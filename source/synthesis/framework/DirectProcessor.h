//
// Created by Davis Polito on 5/2/24.
//

#pragma once

#include "PluginBase.h"
#include "Synthesiser/BKSynthesiser.h"
#include "Synthesiser/Sample.h"
#include <PreparationStateImpl.h>
#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>
#include <chowdsp_serialization/chowdsp_serialization.h>
#include <chowdsp_sources/chowdsp_sources.h>

struct DirectParams : chowdsp::ParamHolder
{
    // gain slider params, for all gain-type knobs
    float rangeStart = -60.0f;
    float rangeEnd = 6.0f;
    float skewFactor = 2.0f;

    // Adds the appropriate parameters to the Direct Processor
    DirectParams() : chowdsp::ParamHolder ("direct")
    {
        add (gainParam, hammerParam, resonanceParam, pedalParam, velocityParam, attackParam, decayParam, sustainParam, releaseParam);
    }

    // juce::Gain param
    chowdsp::GainDBParameter::Ptr gainParam {
        juce::ParameterID { "gain", 100 },
        "Gain",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        -6.0f
    };

    // Hammer param
    chowdsp::GainDBParameter::Ptr hammerParam {
        juce::ParameterID { "hammer", 100 },
        "Hammer",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        -6.0f
    };

    // Resonance param
    chowdsp::GainDBParameter::Ptr resonanceParam {
        juce::ParameterID { "resonance", 100 },
        "Resonance",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        -6.0f
    };

    // Resonance param
    chowdsp::GainDBParameter::Ptr pedalParam {
        juce::ParameterID { "pedal", 100 },
        "Pedal",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        -6.0f
    };

    // Velocity param
    chowdsp::FloatParameter::Ptr velocityParam {
        juce::ParameterID { "Velocity", 100 },
        "Velocity",
        chowdsp::ParamUtils::createNormalisableRange (0.0f, 127.0f, 63.f), // FIX
        127.0f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

    // Attack param
    chowdsp::TimeMsParameter::Ptr attackParam {
        juce::ParameterID { "attack", 100 },
        "attack",
        chowdsp::ParamUtils::createNormalisableRange (0.0f, 1000.0f, 500.0f),
        0.0f
    };

    // Decay param
    chowdsp::TimeMsParameter::Ptr decayParam {
        juce::ParameterID { "decay", 100 },
        "Decay",
        chowdsp::ParamUtils::createNormalisableRange (0.0f, 1000.0f, 500.0f),
        0.0f
    };

    // Sustain param
    chowdsp::FloatParameter::Ptr sustainParam {
        juce::ParameterID { "sustain", 100 },
        "Sustain",
        chowdsp::ParamUtils::createNormalisableRange (0.0f, 1.0f, 0.5f),
        1.0f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

    // Release param
    chowdsp::TimeMsParameter::Ptr releaseParam {
        juce::ParameterID { "release", 100 },
        "Release",
        chowdsp::ParamUtils::createNormalisableRange (0.0f, 1000.0f, 500.0f),
        10.0f
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
        //addStateValues ({ /*,&isSelected*/});
    }

    //chowdsp::StateValue<juce::Point<int>> prepPoint { "prep_point", { 300, 500 } };
    //chowdsp::StateValue<bool> isSelected { "selected", true };
};

class DirectProcessor : public bitklavier::PluginBase<bitklavier::PreparationStateImpl<DirectParams, DirectNonParameterState, chowdsp::XMLSerializer>>
{
public:
    DirectProcessor (const juce::ValueTree& v);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>& buffer) override {};

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    bool acceptsMidi() const override
    {
        return true;
    }

    void addSoundSet (juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* s)
    {
        mainSynth.addSoundSet (s);
    }

    void addSoundSet (
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* s, // main samples
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* h, // hammer samples
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* r, // release samples
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* p) // pedal samples
    {
        DBG ("adding main, hammer and releaseResonance synths");
        mainSynth.addSoundSet (s);
        hammerSynth.addSoundSet (h);
        releaseResonanceSynth.addSoundSet (r);
        pedalSynth.addSoundSet (p);
    }

    juce::AudioProcessor::BusesProperties directBusLayout()
    {
        return BusesProperties()
            .withOutput ("Output1", juce::AudioChannelSet::stereo(), true);
    }
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
    bool hasEditor() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }

    void addToVT (juce::ValueTree& vt)
    {
        state.params.doForAllParameters ([this, &vt] (auto& param, size_t) {
            vt.setProperty (param.paramID, chowdsp::ParameterTypeHelpers::getValue (param), nullptr);
        });
    }

private:
    //chowdsp::experimental::Directillator<float> oscillator;
    chowdsp::Gain<float> gain;
    juce::ADSR::Parameters adsrParams;
    BKSynthesiser mainSynth;
    BKSynthesiser hammerSynth;
    BKSynthesiser releaseResonanceSynth;
    BKSynthesiser pedalSynth;
    chowdsp::ScopedCallbackList adsrCallbacks;
    chowdsp::ScopedCallbackList vtCallbacks;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectProcessor)
};
