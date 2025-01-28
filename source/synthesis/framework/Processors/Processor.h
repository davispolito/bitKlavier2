//
// Created by Davis Polito on 1/28/25.
//

#ifndef BITKLAVIER2_PROCESSOR_H
#define BITKLAVIER2_PROCESSOR_H
#include "Factory.h"
#include "juce_audio_processors/juce_audio_processors.h"

namespace bitklavier {


    class Processor : public juce::AudioProcessor,
                      public Factory<Processor, const juce::ValueTree &> {
    public:
        Processor(Key) : juce::AudioProcessor(){}

        virtual ~Processor() = default;
        virtual const juce::String getName() const;

        virtual bool acceptsMidi() const;

        virtual bool producesMidi() const;// override { return false; }
        virtual bool isMidiEffect() const;// override { return false; }

        virtual double getTailLengthSeconds() const;// override { return 0.0; }
        virtual bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const; //verride {};
        virtual void prepareToPlay (double sampleRate, int samplesPerBlock);// override {};
        virtual void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) ; //override{};
        virtual void processBlock (juce::AudioBuffer<double>&, juce::MidiBuffer&) ;//override {}
        virtual void processAudioBlock (juce::AudioBuffer<float>&) = 0;

        bool hasEditor() const override
        {
            return true;
        }

    private:
        //static juce::AudioProcessor::BusesProperties getDefaultBusLayout(){};
        //virtual void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&:)
    };
}
#endif //BITKLAVIER2_PROCESSOR_H
