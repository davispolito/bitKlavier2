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

#include "synth_base.h"
#include <JuceHeader.h>
#if HEADLESS

class FullInterface { };
class AudioDeviceManager { };

#endif
#include "templates/Factory.h"
#include "tracktion_engine.h"
#include "SampleLoadManager.h"
#include "UserPreferences.h"
class FullInterface;


struct SynthGuiData {
  SynthGuiData(SynthBase* synth_base);
  juce::ValueTree& tree;
  UndoManager& um;
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
    void externalPresetLoaded(File preset);
    void setGuiSize(float scale);
    FullInterface* getGui() { return gui_.get(); }
    UserPreferencesWrapper userPreferences;
    SynthBase* synth_;
    SampleLoadManager sampleLoadManager ;
  protected:


    std::unique_ptr<FullInterface> gui_;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthGuiInterface)
};

