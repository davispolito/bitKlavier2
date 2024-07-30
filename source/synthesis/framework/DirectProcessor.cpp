//
// Created by Davis Polito on 5/2/24.
//


#include "DirectProcessor.h"
#include "common.h"
DirectProcessor::DirectProcessor() : PluginBase(nullptr, directBusLayout())
{

}

void DirectProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };

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
    for(auto mi : midiMessages)
    {
        auto message = mi.getMessage();
        DBG(bitklavier::printMidi(message, "direct"));
    }
}