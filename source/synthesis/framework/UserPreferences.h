//
// Created by Davis Polito on 8/12/24.
//

#ifndef BITKLAVIER2_USERPREFERENCES_H
#define BITKLAVIER2_USERPREFERENCES_H

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <tracktion_engine.h>
#include "Identifiers.h"
struct UserPreferences : private tracktion::engine::ValueTreeAllEventListener
{
public:
    juce::File file;
    juce::ValueTree tree;

    UserPreferences()
    {
        juce::PropertiesFile::Options options;
        options.applicationName = "bitklavier";
        options.folderName = "bitklavier";
        options.filenameSuffix = ".settings";
        options.osxLibrarySubFolder = "Application Support";
        file = options.getDefaultFile();

        tree = juce::ValueTree::fromXml(file.loadFileAsString());
        if (!tree.isValid())
            tree = juce::ValueTree("Preferences");
        tree.addListener(this);
        juce::String path_to_samples = "~/Library/Application Support/bitklavier/samples";

        tree.setProperty("default_sample_path", path_to_samples, nullptr);
        juce::ValueTree a (IDs::midiPrefs);
        if(!tree.getChildWithName(IDs::midiInput).isValid())
            tree.appendChild(a, nullptr);
    }

    void valueTreeChanged() override { file.replaceWithText(tree.toXmlString()); }
};


class UserPreferencesWrapper : private juce::ValueTree::Listener
{
public:

    juce::SharedResourcePointer<UserPreferences> userPreferences;
    juce::ValueTree tree = userPreferences->tree;
    //juce::CachedValue<juce::String> skin { tree, "skin", nullptr, "Light" };
    std::function<void(juce::String)> onSkinChange;

    UserPreferencesWrapper()
    {
        tree.addListener(this);
    }

private:
    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override
    {
        //skin.forceUpdateOfCachedValue();
        //juce::NullCheckedInvocation::invoke(onSkinChange, skin);
    }
};
namespace ProjectInfo
{
    const char* const  projectName    = "bitKlavier2 Demo";
    const char* const  companyName    = "bitKlavier2 Company";
    const char* const  versionString  = "0.0.1";
    const int          versionNumber  =  0x1;
}
#endif //BITKLAVIER2_USERPREFERENCES_H
