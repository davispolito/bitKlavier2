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
#include "EnvParams.h"
#include "TransposeParams.h"
#include "RangeSliderParams.h"
#include "buffer_debugger.h"
#include "Identifiers.h"

struct DirectParams : chowdsp::ParamHolder
{
    // gain slider params, for all gain-type knobs
    float rangeStart = -60.0f;
    float rangeEnd = 6.0f;
    float skewFactor = 2.0f;

    // Adds the appropriate parameters to the Direct Processor
    DirectParams() : chowdsp::ParamHolder ("Direct")
    {
        add (gainParam,
            hammerParam,
            releaseResonanceParam,
            pedalParam,
            blendronicSend,
            transpositionUsesTuning,
            env,
            transpose,
            velocityRangeParams);
    }

    // Gain param
    chowdsp::GainDBParameter::Ptr gainParam {
        juce::ParameterID { "Gain", 100 },
        "Gain",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        0.0f
    };

    // Hammer param
    chowdsp::GainDBParameter::Ptr hammerParam {
        juce::ParameterID { "Hammers", 100 },
        "Hammer",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        -6.0f
    };

    // Resonance param
    chowdsp::GainDBParameter::Ptr releaseResonanceParam {
        juce::ParameterID { "Ring", 100 },
        "Release Resonance",
        juce::NormalisableRange { rangeStart, rangeEnd + 24, 0.0f, skewFactor, false },
        6.0f
    };

    // Pedal param
    chowdsp::GainDBParameter::Ptr pedalParam {
        juce::ParameterID { "Pedal", 100 },
        "Pedal",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        -6.0f
    };

    // Gain param
    chowdsp::GainDBParameter::Ptr blendronicSend {
        juce::ParameterID { "Blendronic", 100 },
        "Blendronic Send",
        juce::NormalisableRange { rangeStart, rangeEnd, 0.0f, skewFactor, false },
        0.0f
    };

    // Transposition Uses Tuning param
    chowdsp::BoolParameter::Ptr transpositionUsesTuning {
        juce::ParameterID { "UseTuning", 100 },
        "Use Tuning",
        false
    };

    // ADSR params
    EnvParams env;

    // Transposition slider (holds up to 12 transposition values)
    TransposeParams transpose;

    // Velocity Range Slider
    RangeSliderParams velocityRangeParams;

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

class DirectProcessor : public bitklavier::PluginBase<bitklavier::PreparationStateImpl<DirectParams, DirectNonParameterState, chowdsp::XMLSerializer>>,
public juce::ValueTree::Listener
{
public:
    DirectProcessor (const juce::ValueTree& v);
    ~DirectProcessor(){}

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>& buffer) override {};
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    bool acceptsMidi() const override
    {
        return true;
    }

    void addSoundSet (std::map<juce::String, juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>>* s)
    {
        ptrToSamples = s;
    }

    void addSoundSet (
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* s, // main samples
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* h, // hammer samples
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* r, // release samples
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* p) // pedal samples
    {
        DBG ("adding main, hammer and releaseResonance synths");
        mainSynth->addSoundSet (s);
        hammerSynth->addSoundSet (h);
        releaseResonanceSynth->addSoundSet (r);
        pedalSynth->addSoundSet (p);
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
    void valueTreePropertyChanged   (juce::ValueTree& t, const juce::Identifier&) {
        juce::String a  = t.getProperty(IDs::mainSampleSet, "");
        juce::String b  = t.getProperty(IDs::hammerSampleSet, "");
        juce::String c  = t.getProperty(IDs::releaseResonanceSampleSet, "");
        juce::String d  = t.getProperty(IDs::pedalSampleSet, "");
        addSoundSet (&(*ptrToSamples)[a],
                &(*ptrToSamples)[b],
                &(*ptrToSamples)[c],
                &(*ptrToSamples)[d]);

    }
    void valueTreeChildAdded        (juce::ValueTree&, juce::ValueTree&)        {}
    void valueTreeChildRemoved      (juce::ValueTree&, juce::ValueTree&, int)   {}
    void valueTreeChildOrderChanged (juce::ValueTree&, int, int)          {}
    void valueTreeParentChanged     (juce::ValueTree&)                    {}
    void valueTreeRedirected        (juce::ValueTree&)                    {}

private:
    //chowdsp::experimental::Directillator<float> oscillator;
    chowdsp::Gain<float> gain;
    juce::ScopedPointer<BufferDebugger> bufferDebugger;

    std::unique_ptr<BKSynthesiser> mainSynth;
    std::unique_ptr<BKSynthesiser> hammerSynth;
    std::unique_ptr<BKSynthesiser> releaseResonanceSynth;
    std::unique_ptr<BKSynthesiser> pedalSynth;

    //float releaseResonanceSynthGainMultiplier = 10.; // because these are very soft
    juce::Array<float> midiNoteTranspositions;
    juce::Array<float> getMidiNoteTranspositions();

    std::map<juce::String, juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>>* ptrToSamples;

    //chowdsp::ScopedCallbackList adsrCallbacks;  // need this?
    //chowdsp::ScopedCallbackList vtCallbacks;    // need this?
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirectProcessor)
};
