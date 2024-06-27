//
// Created by Joshua Warner on 6/27/24.
//

#include "ResonanceProcessor.h"

ResonanceProcessor::ResonanceProcessor() = default;

void ResonanceProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };

    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
}

void ResonanceProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

}


