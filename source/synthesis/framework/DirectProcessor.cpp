//
// Created by Davis Polito on 5/2/24.
//

#include "DirectProcessor.h"
#include "Synthesiser/Sample.h"
#include "common.h"
#include <chowdsp_serialization/chowdsp_serialization.h>
DirectProcessor::DirectProcessor (const juce::ValueTree& v) : PluginBase (v, nullptr, directBusLayout()),
mainSynth(new BKSynthesiser(state.params.env, state.params.gainParam)),
hammerSynth(new BKSynthesiser(state.params.env,state.params.hammerParam)),
releaseResonanceSynth(new BKSynthesiser(state.params.env,state.params.releaseResonanceParam)),
pedalSynth(new BKSynthesiser(state.params.env,state.params.pedalParam))
{
    for (int i = 0; i < 300; i++)
    {
        mainSynth->addVoice (new BKSamplerVoice());
        hammerSynth->addVoice (new BKSamplerVoice());
        releaseResonanceSynth->addVoice (new BKSamplerVoice());
        pedalSynth->addVoice (new BKSamplerVoice());
    }
//
//<<<<<<< HEAD
//    // mixer
//    // connect these to sliders, as with ADSR
//    mainSynth->setSynthGain (1.);
//    hammerSynth->setSynthGain (0.05);
//    releaseResonanceSynth->setSynthGain (1.0 * releaseResonanceSynthGainMultiplier);
//    pedalSynth->setSynthGain (0.4);
//
//    //    std::unique_ptr<XmlElement> xml = chowdsp::Serialization::serialize<chowdsp::XMLSerializer>(state);
//    //    DBG(chowdsp::Serialization::serialize<chowdsp::XMLSerializer>(state)->toString());
//    //chowdsp::Serialization::deserialize<chowdsp::XMLSerializer>(xml,state);
//    adsrCallbacks += {
//
//        state.addParameterListener (*state.params.gainParam,
//             chowdsp::ParameterListenerThread::AudioThread,
//             [this] {
//                 mainSynth->setSynthGain (juce::Decibels::decibelsToGain (state.params.gainParam->get()));
//                 DBG ("direct gain: " + juce::String (state.params.gainParam->get()));
//             }),
//
//        state.addParameterListener (*state.params.hammerParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                hammerSynth->setSynthGain (juce::Decibels::decibelsToGain (state.params.hammerParam->get()));
//                DBG ("hammer gain: " + juce::String (state.params.hammerParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.releaseResonanceParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                releaseResonanceSynth->setSynthGain (juce::Decibels::decibelsToGain (state.params.releaseResonanceParam->get()) * releaseResonanceSynthGainMultiplier);
//                DBG ("release resonance gain: " + juce::String (state.params.releaseResonanceParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.pedalParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                pedalSynth->setSynthGain (juce::Decibels::decibelsToGain (state.params.pedalParam->get()));
//                DBG ("pedal gain: " + juce::String (state.params.pedalParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.env.attackParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                mainSynth->globalADSR.attack = state.params.env.attackParam->get() * .001f; //should fix, not do hard conversions between ms and seconds here
//                DBG ("attack: " + juce::String (state.params.env.attackParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.env.attackPowerParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                mainSynth->globalADSR.attackPower = state.params.env.attackPowerParam->get() * -1.;
//                DBG ("attack power: " + juce::String (state.params.env.attackPowerParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.env.decayParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                mainSynth->globalADSR.decay = state.params.env.decayParam->get() * .001f;
//                DBG ("decay: " + juce::String (state.params.env.decayParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.env.decayPowerParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                mainSynth->globalADSR.decayPower = state.params.env.decayPowerParam->get() * -1.;
//                DBG ("decay power: " + juce::String (state.params.env.decayPowerParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.env.sustainParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                mainSynth->globalADSR.sustain = state.params.env.sustainParam->get();
//                DBG ("sustain: " + juce::String (state.params.env.sustainParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.env.releaseParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                mainSynth->globalADSR.release = state.params.env.releaseParam->get() * .001f;
//                DBG ("release: " + juce::String (state.params.env.releaseParam->get()));
//            }),
//
//        state.addParameterListener (*state.params.env.releasePowerParam,
//            chowdsp::ParameterListenerThread::AudioThread,
//            [this] {
//                mainSynth->globalADSR.releasePower = state.params.env.releasePowerParam->get() * -1.;
//                DBG ("attack power: " + juce::String (state.params.env.releasePowerParam->get()));
//            }),
//
//    };
//=======
//>>>>>>> feature/adsropt

    // these synths play their stuff on noteOff rather than noteOn
    hammerSynth->isKeyReleaseSynth (true);
    releaseResonanceSynth->isKeyReleaseSynth (true);
    pedalSynth->isPedalSynth (true);
    bufferDebugger = new BufferDebugger();

}

void DirectProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    mainSynth->setCurrentPlaybackSampleRate (sampleRate);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);

    hammerSynth->setCurrentPlaybackSampleRate (sampleRate);
    releaseResonanceSynth->setCurrentPlaybackSampleRate (sampleRate);
    pedalSynth->setCurrentPlaybackSampleRate (sampleRate);
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

    buffer.clear(); // always top of the chain as an instrument source; doesn't take audio in

    if (mainSynth->hasSamples() )
        mainSynth->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    if (hammerSynth->hasSamples())
        hammerSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    if (releaseResonanceSynth->hasSamples())
        releaseResonanceSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    if (pedalSynth->hasSamples())
        pedalSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

//DBG ("attack: " + juce::String (state.params.env.attackParam->get()));
    //juce::dsp::AudioBlock<float> block(buffer);
    //melatonin::printSparkline(buffer);
    bufferDebugger->capture("direct", buffer.getReadPointer(0), buffer.getNumSamples(), -1.f, 1.f);
}