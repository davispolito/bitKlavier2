//
// Created by Davis Polito on 5/2/24.
//



#include "DirectProcessor.h"

DirectProcessor::DirectProcessor() = default;

void DirectProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };

    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
}

void DirectProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    buffer.clear();



    gain.setGainDecibels (*state.params.gainParam);
    gain.process (buffer);
}
