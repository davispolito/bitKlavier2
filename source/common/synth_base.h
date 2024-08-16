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


#include "midi_manager.h"
#include "../synthesis/framework/poly_utils.h"


#include <set>
#include <string>
#include "Connection.h"

class SynthGuiInterface;
template<typename T>
class BKSamplerSound;

class SynthBase : public MidiManager::Listener, public juce::ValueTree::Listener {
  public:
    static constexpr float kOutputWindowMinNote = 16.0f;
    static constexpr float kOutputWindowMaxNote = 128.0f;

    SynthBase();
    virtual ~SynthBase();


    void valueChanged(const std::string& name, bitklavier::mono_float value);
    void valueChangedThroughMidi(const std::string& name, bitklavier::mono_float value) override;
    void pitchWheelMidiChanged(bitklavier::mono_float value) override;
    void modWheelMidiChanged(bitklavier::mono_float value) override;
    void pitchWheelGuiChanged(bitklavier::mono_float value);
    void modWheelGuiChanged(bitklavier::mono_float value);
    void presetChangedThroughMidi(juce::File preset) override;
//    void valueChangedExternal(const std::string& name, bitklavier::mono_float value);
//    void valueChangedInternal(const std::string& name, bitklavier::mono_float value);


    bool isModSourceEnabled(const std::string& source);
    int getNumModulations(const std::string& destination);
    int getConnectionIndex(const std::string& source, const std::string& destination);







    int getSampleRate();
    void initEngine();


    void setMpeEnabled(bool enabled);

    virtual void setValueNotifyHost(const std::string& name, bitklavier::mono_float value) { }

//    void armMidiLearn(const std::string& name);
//    void cancelMidiLearn();
//    void clearMidiLearn(const std::string& name);
    bool isMidiMapped(const std::string& name);

    void setAuthor(const juce::String& author);
    void setComments(const juce::String& comments);
    void setStyle(const juce::String& comments);
    void setPresetName(const juce::String& preset_name);
    void setMacroName(int index, const juce::String& macro_name);
    juce::String getAuthor();
    juce::String getComments();
    juce::String getStyle();
    juce::String getPresetName();
    juce::String getMacroName(int index);


    bitklavier::SoundEngine* getEngine() { return engine_.get(); }
    juce::MidiKeyboardState* getKeyboardState() { return keyboard_state_.get(); }

    void notifyOversamplingChanged();
    void checkOversampling();
    virtual const juce::CriticalSection& getCriticalSection() = 0;
    virtual void pauseProcessing(bool pause) = 0;

    struct ValueChangedCallback : public juce::CallbackMessage {
      ValueChangedCallback(std::shared_ptr<SynthBase*> listener, std::string name, bitklavier::mono_float val) :
          listener(listener), control_name(std::move(name)), value(val) { }

      void messageCallback() override;

      std::weak_ptr<SynthBase*> listener;
      std::string control_name;
      bitklavier::mono_float value;
    };

    juce::AudioProcessorGraph::Node::Ptr addProcessor(std::unique_ptr<juce::AudioProcessor> processor);
    juce::AudioProcessorGraph::Node * getNodeForId(AudioProcessorGraph::NodeID id);
    void addConnection(AudioProcessorGraph::Connection& connect);

    juce::ValueTree& getValueTree();
    juce::UndoManager& getUndoManager();
    static constexpr size_t actionSize = 16; // sizeof ([this, i = index] { callMessageThreadBroadcaster (i); })
    using AudioThreadAction = juce::dsp::FixedSizeFunction<actionSize, void()>;
    moodycamel::ReaderWriterQueue<AudioThreadAction> processorInitQueue { 10 };
  protected:

    juce::ValueTree tree;
    juce::UndoManager um;
    virtual SynthGuiInterface* getGuiInterface() = 0;
//    OwnedArray<ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>> mainPianoSoundSet;
//    OwnedArray<ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>> resonanceReleaseSoundSet;
//    OwnedArray<ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>> hammerReleaseSoundSet;
//    OwnedArray<ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>> emptySoundSet;
//    OwnedArray<ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>> pedalReleaseSoundSet;




    void processAudio(juce::AudioSampleBuffer* buffer, int channels, int samples, int offset);
    void processAudioAndMidi(juce::AudioBuffer<float>& audio_buffer, juce::MidiBuffer& midi_buffer); // , int channels, int samples, int offset, int start_sample = 0, int end_sample = 0);
    void processAudioWithInput(juce::AudioSampleBuffer* buffer, const bitklavier::mono_float* input_buffer,
                               int channels, int samples, int offset);
    void writeAudio(juce::AudioSampleBuffer* buffer, int channels, int samples, int offset);
    void processMidi(juce::MidiBuffer& buffer, int start_sample = 0, int end_sample = 0);
    void processKeyboardEvents(juce::MidiBuffer& buffer, int num_samples);
    //void processModulationChanges();
    void updateMemoryOutput(int samples, const bitklavier::poly_float* audio);

    std::unique_ptr<bitklavier::SoundEngine> engine_;
    std::unique_ptr<MidiManager> midi_manager_;
    std::unique_ptr<juce::MidiKeyboardState> keyboard_state_;

    std::shared_ptr<SynthBase*> self_reference_;

    File active_file_;

    bitklavier::mono_float last_played_note_;
    int last_num_pressed_;
    bitklavier::mono_float memory_reset_period_;
    bitklavier::mono_float memory_input_offset_;
    int memory_index_;
    bool expired_;

    std::map<std::string, String> save_info_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthBase)
};

class HeadlessSynth : public SynthBase {
  public:
    virtual const CriticalSection& getCriticalSection() override {
      return critical_section_;
    }

    virtual void pauseProcessing(bool pause) override {
      if (pause)
        critical_section_.enter();
      else
        critical_section_.exit();
    }

  protected:
    virtual SynthGuiInterface* getGuiInterface() override { return nullptr; }

  private:
    CriticalSection critical_section_;
};
