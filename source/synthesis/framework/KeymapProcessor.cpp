//
// Created by Davis Polito on 6/19/24.
//

#include "KeymapProcessor.h"
#include "common.h"
KeymapProcessor::KeymapProcessor(const ValueTree& v, AudioDeviceManager* manager) : PluginBase(v, nullptr, keymapBusLayout()), _midi(std::make_unique<MidiManager>(&keyboard_state,manager,v))
{

}

void KeymapProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    _midi->midi_collector_.reset(sampleRate);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
}


void KeymapProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    midiMessages.clear();
    int num_samples = buffer.getNumSamples();
    MidiBuffer midi_messages;
    _midi->removeNextBlockOfMessages(midiMessages, num_samples);
    _midi->replaceKeyboardMessages(midiMessages, num_samples);
    for(auto mi : midiMessages)
    {
        auto message = mi.getMessage();
        DBG(bitklavier::printMidi(message, "kmap"));
    }
}