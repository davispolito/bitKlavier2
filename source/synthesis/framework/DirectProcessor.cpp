//
// Created by Davis Polito on 5/2/24.
//

#include "DirectProcessor.h"
#include "Synthesiser/Sample.h"
#include "common.h"
#include <chowdsp_serialization/chowdsp_serialization.h>
DirectProcessor::DirectProcessor (const juce::ValueTree& v) : PluginBase (v, nullptr, directBusLayout())
{
    for (int i = 0; i < 300; i++)
    {
        mainSynth.addVoice (new BKSamplerVoice());
        hammerSynth.addVoice (new BKSamplerVoice());
        releaseResonanceSynth.addVoice (new BKSamplerVoice());
        pedalSynth.addVoice (new BKSamplerVoice());
    }

    // mixer
    // connect these to sliders, as with ADSR
    mainSynth.setSynthGain (1.);
    hammerSynth.setSynthGain (0.05);
    releaseResonanceSynth.setSynthGain (2.);
    pedalSynth.setSynthGain (0.2);

    //    std::unique_ptr<XmlElement> xml = chowdsp::Serialization::serialize<chowdsp::XMLSerializer>(state);
    //    DBG(chowdsp::Serialization::serialize<chowdsp::XMLSerializer>(state)->toString());
    //chowdsp::Serialization::deserialize<chowdsp::XMLSerializer>(xml,state);
    adsrCallbacks += {

        state.addParameterListener (*state.params.attackParam,
            chowdsp::ParameterListenerThread::AudioThread,
            [this] {
                mainSynth.globalADSR.attack = state.params.attackParam->get() * .001f; //should fix, not do hard conversions between ms and seconds here
                DBG ("attack: " + juce::String (state.params.attackParam->get()));
            }),

        state.addParameterListener (*state.params.decayParam,
            chowdsp::ParameterListenerThread::AudioThread,
            [this] {
                mainSynth.globalADSR.decay = state.params.decayParam->get() * .001f;
                DBG ("decay: " + juce::String (state.params.decayParam->get()));
            }),

        state.addParameterListener (*state.params.sustainParam,
            chowdsp::ParameterListenerThread::AudioThread,
            [this] {
                mainSynth.globalADSR.sustain = state.params.sustainParam->get();
                DBG ("sustain: " + juce::String (state.params.sustainParam->get()));
            }),

        state.addParameterListener (*state.params.releaseParam,
            chowdsp::ParameterListenerThread::AudioThread,
            [this] {
                mainSynth.globalADSR.release = state.params.releaseParam->get() * .001f;
                DBG ("release: " + juce::String (state.params.releaseParam->get()));
            }),

        state.addParameterListener (*state.params.gainParam,
            chowdsp::ParameterListenerThread::AudioThread,
            [this] {
                mainSynth.setSynthGain (juce::Decibels::decibelsToGain (state.params.gainParam->get()));
                DBG ("direct gain: " + juce::String (state.params.gainParam->get()));
            })
    };

    // these synths play their stuff on noteOff rather than noteOn
    hammerSynth.isKeyReleaseSynth (true);
    releaseResonanceSynth.isKeyReleaseSynth (true);
    pedalSynth.isPedalSynth (true);

    // should be able to hard-wire these
    hammerSynth.globalADSR.attack = 0.001f;
    hammerSynth.globalADSR.decay = 0.0f;
    hammerSynth.globalADSR.sustain = 1.0f;
    hammerSynth.globalADSR.release = 0.05;

    releaseResonanceSynth.globalADSR.attack = 0.001f;
    releaseResonanceSynth.globalADSR.decay = 0.0f;
    releaseResonanceSynth.globalADSR.sustain = 1.0f;
    releaseResonanceSynth.globalADSR.release = 0.05;

    pedalSynth.globalADSR.attack = 0.001f;
    pedalSynth.globalADSR.decay = 0.0f;
    pedalSynth.globalADSR.sustain = 1.0f;
    pedalSynth.globalADSR.release = 0.05;
}

void DirectProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    mainSynth.setCurrentPlaybackSampleRate (sampleRate);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);

    hammerSynth.setCurrentPlaybackSampleRate (sampleRate);
    releaseResonanceSynth.setCurrentPlaybackSampleRate (sampleRate);
    pedalSynth.setCurrentPlaybackSampleRate (sampleRate);
}

//void DirectProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
//{
//    buffer.clear();
//
//
//
//    gain.setGainDecibels (*state.params.gainParam);
//    gain.process (buffer);
//}

bool DirectProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const
{
    //    // only supports mono and stereo (for now)
    //    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
    //        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    //        return false;
    //
    //    // input and output layout must be the same
    //    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    //        return false;

    return true;
}

void DirectProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
#if JUCE_MODULE_AVAILABLE_chowdsp_plugin_state
    state.getParameterListeners().callAudioThreadBroadcasters();
#endif
    for (auto mi : midiMessages)
    {
        auto message = mi.getMessage();

        //        DBG(bitklavier::printMidi(message, "direct"));
        //        state.params.doForAllParameters([this](auto& param, size_t) {
        //           param.printDebug();
        //           });
    }

    buffer.clear();

    if (mainSynth.getNumSounds() > 0)
        mainSynth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    if (hammerSynth.getNumSounds() > 0)
        hammerSynth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    if (releaseResonanceSynth.getNumSounds() > 0)
        releaseResonanceSynth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    if (pedalSynth.getNumSounds() > 0)
        pedalSynth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    //juce::dsp::AudioBlock<float> block(buffer);
    //melatonin::printSparkline(buffer);
}