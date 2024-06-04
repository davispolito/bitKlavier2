//
// Created by Davis Polito on 5/2/24.
//



#include "PolygonalOscProcessor.h"

PolygonalOscProcessor::PolygonalOscProcessor() = default;

void PolygonalOscProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    oscillator.prepare (spec);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
}

void PolygonalOscProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    buffer.clear();

    oscillator.setFrequency (*state.params.freqParam);
    oscillator.setOrder (*state.params.orderParam);
    oscillator.setTeeth (*state.params.teethParam);
    oscillator.processBlock (buffer);

    gain.setGainDecibels (*state.params.gainParam);
    gain.process (buffer);
}
