//
// Created by Joshua Warner on 6/27/24.
//

#include "BlendronicProcessor.h"

BlendronicProcessor::BlendronicProcessor() = default;

void BlendronicProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };

    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
}


void BlendronicProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

}


