/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "Cable/Cable.h"

class SynthBase;
#if HEADLESS

class FullInterface { };
class juce::AudioDeviceManager { };

#endif

#include "synth_base.h"
class SampleLoadManager;
class UserPreferencesWrapper;
class FullInterface;
struct OpenGlWrapper;

struct SynthGuiData {
  SynthGuiData(SynthBase* synth_base);
  juce::ValueTree& tree;
  juce::UndoManager& um;
  SynthBase* synth;
};

class SynthGuiInterface {
  public:
    SynthGuiInterface(SynthBase* synth, bool use_gui = true);
    virtual ~SynthGuiInterface();

    virtual juce::AudioDeviceManager* getAudioDeviceManager() { return nullptr; }
    SynthBase* getSynth() { return synth_; }
    virtual void updateFullGui();
    virtual void updateGuiControl(const std::string& name, bitklavier::mono_float value);
    bitklavier::mono_float getControlValue(const std::string& name);


    void setFocus();
    void notifyChange();
    void notifyFresh();
    void openSaveDialog();
    void externalPresetLoaded(juce::File preset);
    void setGuiSize(float scale);

    void tryEnqueueProcessorInitQueue(juce::FixedSizeFunction<16, void()> callback);

    /* SynthBase Wrapper Functions */
    bool isMidiMapped(const std::string& name);
    bool loadFromFile(juce::File file, std::string& error);

    void addCableConnection(Cable* c);
    void resetSynthEngine();
    void setAuthor(const juce::String& author);
    void setPresetName(const juce::String& preset_name);

    juce::ValueTree& getValueTree();
    juce::String getComments();
    juce::File getActiveFile();
    juce::String getPresetName();
    bitklavier::SoundEngine* getEngine();
    //juce::AudioProcessorGraph::Node::Ptr addProcessor (PreparationSection* object);
    void enterCriticalSection();
    void exitCriticalSection();

    FullInterface* getGui() { return gui_.get(); }
    OpenGlWrapper* getOpenGlWrapper();
    UserPreferencesWrapper* userPreferences;
    SynthBase* synth_; //does this need to be there??
    SampleLoadManager* sampleLoadManager ;


  protected:


    std::unique_ptr<FullInterface> gui_;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthGuiInterface)
};



