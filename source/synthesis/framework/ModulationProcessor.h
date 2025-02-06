//
// Created by Davis Polito on 1/30/25.
//

#ifndef BITKLAVIER2_MODULATIONPROCESSOR_H
#define BITKLAVIER2_MODULATIONPROCESSOR_H
#include <juce_audio_processors/juce_audio_processors.h>
namespace bitklavier {

    class ModulationProcessor : public juce::AudioProcessor {
    public:
        ModulationProcessor(juce::ValueTree& vt) :
        juce::AudioProcessor(BusesProperties().withInput("disabled",juce::AudioChannelSet::mono(),false)
        .withOutput("disabled",juce::AudioChannelSet::mono(),false)
        .withOutput("Modulation",juce::AudioChannelSet::discreteChannels(1),true)
        .withInput( "Modulation",juce::AudioChannelSet::discreteChannels(1),true))
        {

        }

        bool acceptsMidi() const override {
            return true;
        }

        bool producesMidi() const override { return false; }

        bool isMidiEffect() const override { return false; }

        const juce::String getName() const override {
            return "modulation";
        }

        void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override {

        }

        void releaseResources() override
        {

        }
        double getTailLengthSeconds() const override
        {

        }


        void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override
        {
        }

        juce::AudioProcessorEditor * createEditor() override
        {
           return  nullptr;
        }
        int getNumPrograms() override
        {
            return 1;
        }

        int getCurrentProgram() override
        {
            return 1;

        }

        void setCurrentProgram(int index) override
        {

        }

        void changeProgramName(int index, const juce::String &newName) override
        {

        }

        void getStateInformation(juce::MemoryBlock &destData) override
        {

        }

        void setStateInformation(const void *data, int sizeInBytes) override
        {

        }

        bool hasEditor() const override
        {
            return true;
        }
        const juce::String getProgramName(int index) override
        {
            return "";
        }

    };

} // bitklavier

#endif //BITKLAVIER2_MODULATIONPROCESSOR_H
