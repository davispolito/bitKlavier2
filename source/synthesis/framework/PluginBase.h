//
// Created by Davis Polito on 10/1/24.
//

#ifndef BITKLAVIER2_PLUGINBASE_H
#define BITKLAVIER2_PLUGINBASE_H

#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <chowdsp_parameters/chowdsp_parameters.h>
#include "Processor.h"
//#include "Factory.h"

namespace bitklavier {
    /**
  * Base class for plugin processors.
  *
  * Derived classes must override `prepareToPlay` and `releaseResources`
  * (from `juce::AudioProcessor`), as well as `processAudioBlock`, and
  * `addParameters`.
 */

    template<class PluginStateType>

class PluginBase : public bitklavier::Processor::Registrar<PluginBase<PluginStateType>> {
    public:
        explicit PluginBase(const juce::ValueTree &v = {});
//                            const juce::AudioProcessor::BusesProperties &layout = getDefaultBusLayout());

//        ~PluginBase() override = default;


//        bool acceptsMidi() const override {
//            return false;
//        }
//
//        bool producesMidi() const override { return false; }
//
//        bool isMidiEffect() const override { return false; }
//
//        double getTailLengthSeconds() const override { return 0.0; }

        int getNumPrograms() override;

        int getCurrentProgram() override;

        void setCurrentProgram(int) override;
//        virtual const juce::String getName();
        const juce::String getProgramName(int) override{return juce::String();}

        void changeProgramName(int, const juce::String &) override;

        bool isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout &layouts) const override; //{return true;};

        void prepareToPlay(double sampleRate, int samplesPerBlock) override;//{};

//        void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;//{};
//
//        void processBlock(juce::AudioBuffer<double> &, juce::MidiBuffer &) override {}
//        void releaseResources() override {}
//        void processAudioBlock (juce::AudioBuffer<float>& buffer) override {};

        bool hasEditor() const override {
            return true;
        }
        juce::AudioProcessorEditor* createEditor() override { return nullptr; }

        void getStateInformation(juce::MemoryBlock &data) override;

        void setStateInformation(const void *data, int sizeInBytes) override;

        auto &getState() {
            return state;
        }

        const auto &getState() const {
            return state;
        }

//        virtual juce::String getWrapperTypeString() const;
//
//        bool supportsParameterModulation() const;

        juce::ValueTree v;
        static std::string name;
    protected:

        PluginStateType state;


    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginBase)
    };

//    template<class P>
//    juce::AudioProcessor::BusesProperties PluginBase<P>::getDefaultBusLayout() {
//        return BusesProperties()
//                .withInput("Input", juce::AudioChannelSet::stereo(), true)
//                .withOutput("Output", juce::AudioChannelSet::stereo(), true);
//    }

#if JUCE_MODULE_AVAILABLE_chowdsp_plugin_state

    template<class State>
    PluginBase<State>::PluginBase(const juce::ValueTree &v)
//                                  const juce::AudioProcessor::BusesProperties &layout)
            :
              state(*this, nullptr),
              v(v) {
        DBG(v.toXmlString());
        if (v.getChild(0).isValid())
            chowdsp::Serialization::deserialize<chowdsp::XMLSerializer>(v.createXml(), state);
    }

#else
    template <class Processor>
PluginBase<Processor>::PluginBase (juce::UndoManager* um, const juce::AudioProcessor::BusesProperties& layout) : juce::AudioProcessor (layout),
                                                                                                                 vts (*this, um, juce::Identifier ("Parameters"), createParameterLayout())
{
}

template <class Processor>
juce::AudioProcessorValueTreeState::ParameterLayout PluginBase<Processor>::createParameterLayout()
{
    juce::Parameters params;

    static_assert (HasAddParameters<Processor>, "Processor class MUST contain a static addParameters function!");
    Processor::addParameters (params);

    return { params.begin(), params.end() };
}
#endif

    template<class P>
    int PluginBase<P>::getNumPrograms() {
        return 1;
    }

    template<class P>
    int PluginBase<P>::getCurrentProgram() {
        return 1;
    }

    template<class P>
    void PluginBase<P>::setCurrentProgram(int index) {
        return;
    }

//    template<class P>
//    const juce::String PluginBase<P>::getProgramName(
//            int index) // NOLINT(readability-const-return-type): Needs to return a const juce::String for override compatibility
//    {
//        return juce::String("");
//    }
//
    template<class P>
    void PluginBase<P>::changeProgramName(int index, const juce::String &newName) {
        return;
    }

    template<class P>
    bool PluginBase<P>::isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout &layouts) const {
        // only supports mono and stereo (for now)
        if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

        // input and output layout must be the same
        if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
            return false;

        return true;
    }

//    template<class P>
//    void PluginBase<P>::prepareToPlay(double sampleRate, int samplesPerBlock) {
//        bitklavier::Processor::setRateAndBufferSizeDetails(sampleRate, samplesPerBlock);
//#if JUCE_MODULE_AVAILABLE_foleys_gui_magic
//        magicState.prepareToPlay (sampleRate, samplesPerBlock);
//#endif
//    }
//
//    template<class P>
//    void PluginBase<P>::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &) {
//        juce::ScopedNoDenormals noDenormals;
//
//#if JUCE_MODULE_AVAILABLE_chowdsp_plugin_state
//        state.getParameterListeners().callAudioThreadBroadcasters();
//#endif
//
//        processAudioBlock(buffer);
//    }

// magic number to identify memory blocks that we've stored as XML
    const juce::uint32 magicXmlNumber = 0x21324356;


    template<class State>
    void PluginBase<State>::getStateInformation(juce::MemoryBlock &data) {
//    {
//        juce::MemoryOutputStream out (data, false);
//        out.writeInt (magicXmlNumber);
//        out.writeInt (0);
//        xml.writeTo (out, juce::XmlElement::TextFormat().singleLine());
//        out.writeByte (0);
//    }
        state.serialize(data);
    }

    template<class State>
    void PluginBase<State>::setStateInformation(const void *data, int sizeInBytes) {
        state.deserialize(juce::MemoryBlock{data, (size_t) sizeInBytes});
    }



} // bitklavier

#endif //BITKLAVIER2_PLUGINBASE_H
