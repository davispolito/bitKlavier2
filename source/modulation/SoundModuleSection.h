//
// Created by Davis Polito on 11/19/24.
//

#ifndef BITKLAVIER_SOUNDMODULESECTION_H
#define BITKLAVIER_SOUNDMODULESECTION_H
#include "ModuleListInterface.h"
class ModulationSection;
class ProcessorBase;
class ModulationManager;
class SoundModuleSection : public ModulesInterface<ModulationSection>
{
public:
    explicit SoundModuleSection(juce::ValueTree &, ModulationManager* m);
    virtual ~SoundModuleSection();
    ModulationSection* createNewObject(const juce::ValueTree& v) override;
    void deleteObject (ModulationSection* at) override;


    // void reset() override;
//    void newObjectAdded (ModuleSection*) override;
    void objectRemoved (ModulationSection*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }
    void valueTreeParentChanged (juce::ValueTree&) override{};
//    void valueTreeRedirected (juce::ValueTree&) override;
    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::modulationproc);
    }
    void setEffectPositions() override;

    PopupItems createPopupMenu() override;
    void handlePopupResult(int result) override;
    Factory<ProcessorBase> factory;
    std::map<std::string, SynthSlider*> getAllSliders() override;

};

#endif //BITKLAVIER_SOUNDMODULESECTION_H
