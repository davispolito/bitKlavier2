//
// Created by Davis Polito on 5/2/24.
//


#include "DirectProcessor.h"
#include "common.h"
#include "Synthesiser/Sample.h"
#include <chowdsp_serialization/chowdsp_serialization.h>
DirectProcessor::DirectProcessor() : PluginBase(nullptr, directBusLayout())
{
    for (int i = 0; i < 300; i++)
    {
        synth.addVoice(new BKSamplerVoice());
    }

    std::unique_ptr<XmlElement> xml = chowdsp::Serialization::serialize<chowdsp::XMLSerializer>(state);
    DBG(chowdsp::Serialization::serialize<chowdsp::XMLSerializer>(state)->toString());
    chowdsp::Serialization::deserialize<chowdsp::XMLSerializer>(xml,state);
    adsrCallbacks += {state.addParameterListener(*state.params.attackParam,
                                                 chowdsp::ParameterListenerThread::AudioThread,
                                                 [this] {
                                                     synth.globalADSR.attack = state.params.attackParam->get();
                                                     DBG("attack: " + String(state.params.attackParam->get()));
                                                 }),
                      state.addParameterListener(*state.params.decayParam,
                                                 chowdsp::ParameterListenerThread::AudioThread,
                                                 [this] {
                                                     synth.globalADSR.decay = state.params.decayParam->get();
                                                     DBG("decay: " + String(state.params.decayParam->get()));
                                                 }),
                      state.addParameterListener(*state.params.sustainParam,
                                                 chowdsp::ParameterListenerThread::AudioThread,
                                                 [this] {
                                                     synth.globalADSR.sustain = state.params.sustainParam->get();
                                                     DBG("sustain: " + String(state.params.sustainParam->get()));
                                                 }),
                      state.addParameterListener(*state.params.releaseParam,
                                                 chowdsp::ParameterListenerThread::AudioThread,
                                                 [this] {
                                                     synth.globalADSR.release = state.params.releaseParam->get();
                                                     DBG("release: " + String(state.params.releaseParam->get()));
                                                 })
    };


}

void DirectProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    synth.setCurrentPlaybackSampleRate(sampleRate);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
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
    for(auto mi : midiMessages)
    {
        auto message = mi.getMessage();

//        DBG(bitklavier::printMidi(message, "direct"));
//        state.params.doForAllParameters([this](auto& param, size_t) {
//           param.printDebug();
//           });

    }
    buffer.clear();
    synth.renderNextBlock (buffer, midiMessages,
                           0, buffer.getNumSamples());
    //juce::dsp::AudioBlock<float> block(buffer);
    //melatonin::printSparkline(buffer);
}