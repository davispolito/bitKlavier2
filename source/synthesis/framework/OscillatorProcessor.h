//
// Created by Davis Polito on 5/2/24.
//

#ifndef BITKLAVIER2_OSCILLATORPROCESSOR_H
#define BITKLAVIER2_OSCILLATORPROCESSOR_H
#include "ProcessorBase.h"
#include "juce_dsp/juce_dsp.h"
#include "juce_data_structures/juce_data_structures.h"
class OscillatorProcessor  : public ProcessorBase
{
public:

    OscillatorProcessor(juce::ValueTree v, juce::UndoManager &um)
    {
        oscillator.setFrequency (440.0f);
        oscillator.initialise ([] (float x) { return std::sin (x); });
    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
        oscillator.prepare (spec);
    }
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        oscillator.process (context);
    }
    void reset() override
    {
        oscillator.reset();
    }
    const juce::String getName() const override { return "Oscillator"; }

private:
    juce::dsp::Oscillator<float> oscillator;
};
#endif //BITKLAVIER2_OSCILLATORPROCESSOR_H
