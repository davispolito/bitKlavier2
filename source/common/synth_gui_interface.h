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
    virtual void updateGuiControl(const std::string& name, float value);
    float getControlValue(const std::string& name);


    void setFocus();
    void notifyChange();
    void notifyFresh();
    void openSaveDialog();
    void externalPresetLoaded(juce::File preset);
    void setGuiSize(float scale);
    bool loadFromFile(juce::File preset, std::string& error);
    FullInterface* getGui() { return gui_.get(); }
    OpenGlWrapper* getOpenGlWrapper();
    std::shared_ptr<UserPreferencesWrapper> userPreferences;
    SynthBase* synth_;
    std::unique_ptr<SampleLoadManager> sampleLoadManager ;
  protected:


    std::unique_ptr<FullInterface> gui_;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthGuiInterface)
};

