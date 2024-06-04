//
// Created by Davis Polito on 4/22/24.
//

#ifndef BITKLAVIER2_NOSTALGICPREPARATION_H
#define BITKLAVIER2_NOSTALGICPREPARATION_H
#include "PreparationSection.h"
struct FPParams1 : chowdsp::ParamHolder
{
    FPParams1()
    {
        add (dummy);
    }
    chowdsp::PercentParameter::Ptr dummy { "dummy", "Dummy", 0.5f };
};

using State = chowdsp::PluginStateImpl<FPParams1>;

struct DummyPlugin : chowdsp::PluginBase<State>
{
    juce::UndoManager undoManager { 1000 };
    DummyPlugin() : chowdsp::PluginBase<chowdsp::PluginStateImpl<FPParams1>> (&undoManager)
    {
    }

    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>&) override {}
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
};
class NostalgicPreparation : public PreparationSection
{
public:
    NostalgicPreparation( std::shared_ptr<juce::AudioProcessor> p, ValueTree v, OpenGlWrapper& um);
    ~NostalgicPreparation();

    static PreparationSection* createNostalgicSection( ValueTree v,  OpenGlWrapper &um)
    {
        return new NostalgicPreparation(std::make_shared<DummyPlugin>(), v, um);
    }
private:

};

#endif //BITKLAVIER2_NOSTALGICPREPARATION_H
