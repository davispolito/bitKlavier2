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

#include "synth_base.h"
#include "synth_gui_interface.h"
#include "melatonin_audio_sparklines/melatonin_audio_sparklines.h"

#include "startup.h"
#include "../synthesis/synth_engine/sound_engine.h"

#include "Identifiers.h"
#include "Synthesiser/Sample.h"
#include "load_save.h"
SynthBase::SynthBase(juce::AudioDeviceManager * deviceManager) : expired_(false), manager(deviceManager) {

  self_reference_ = std::make_shared<SynthBase*>();
  *self_reference_ = this;

  engine_ = std::make_unique<bitklavier::SoundEngine>();






  keyboard_state_ = std::make_unique<juce::MidiKeyboardState>();
  juce::ValueTree v;
  midi_manager_ = std::make_unique<MidiManager>( keyboard_state_.get(),manager, v ,this);






  Startup::doStartupChecks();
  tree = juce::ValueTree(IDs::GALLERY);
  tree.addListener(this);
}

SynthBase::~SynthBase() {
  tree.removeListener(this);
}

//void SynthBase::valueChanged(const std::string& name, bitklavier::mono_float value) {
//
//}
//
//void SynthBase::valueChangedInternal(const std::string& name, bitklavier::mono_float value) {
//  valueChanged(name, value);
//  setValueNotifyHost(name, value);
//}

//void SynthBase::valueChangedThroughMidi(const std::string& name, bitklavier::mono_float value) {
//
//  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, name, value);
//  setValueNotifyHost(name, value);
//  callback->post();
//}

void SynthBase::pitchWheelMidiChanged(bitklavier::mono_float value) {
  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, "pitch_wheel", value);
  callback->post();
}

void SynthBase::modWheelMidiChanged(bitklavier::mono_float value) {
  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, "mod_wheel", value);
  callback->post();
}

void SynthBase::pitchWheelGuiChanged(bitklavier::mono_float value) {
  engine_->setZonedPitchWheel(value, 0, bitklavier::kNumMidiChannels - 1);
}

void SynthBase::modWheelGuiChanged(bitklavier::mono_float value) {
  engine_->setModWheelAllChannels(value);
}

void SynthBase::presetChangedThroughMidi(juce::File preset) {
  SynthGuiInterface* gui_interface = getGuiInterface();
  if (gui_interface) {
    gui_interface->updateFullGui();
    gui_interface->notifyFresh();
  }
}
//
//void SynthBase::valueChangedExternal(const std::string& name, bitklavier::mono_float value) {
//  valueChanged(name, value);
//  if (name == "mod_wheel")
//    engine_->setModWheelAllChannels(value);
//  else if (name == "pitch_wheel")
//    engine_->setZonedPitchWheel(value, 0, bitklavier::kNumMidiChannels - 1);
//
//  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, name, value);
//  callback->post();
//}














void SynthBase::initEngine()
{
  checkOversampling();
}












void SynthBase::setMpeEnabled(bool enabled) {
  midi_manager_->setMpeEnabled(enabled);
}


juce::AudioProcessorGraph::Node::Ptr SynthBase::addProcessor(std::unique_ptr<juce::AudioProcessor> processor , juce::AudioProcessorGraph::NodeID id)
{
    processor->prepareToPlay(engine_->getSampleRate(), engine_->getBufferSize());
    return engine_->addNode(std::move(processor) , id);
}

juce::AudioProcessorGraph::Node * SynthBase::getNodeForId(juce::AudioProcessorGraph::NodeID id)
{
return engine_->processorGraph->getNodeForId(id);
}

void SynthBase::addConnection(juce::AudioProcessorGraph::Connection &connect)
{
    engine_->processorGraph->addConnection(connect);
}
bool SynthBase::loadFromValueTree(const juce::ValueTree& state)
{
    pauseProcessing(true);
    engine_->allSoundsOff();
    tree.copyPropertiesAndChildrenFrom(state, nullptr);
    pauseProcessing(false);
    DBG("unpause processing");
    if (tree.isValid())
        return true;
    return false;
}

bool SynthBase::loadFromFile(juce::File preset, std::string& error) {
    DBG("laoding from file");
    if (!preset.exists())
        return false;

    auto xml = juce::parseXML(preset);
    if(xml == nullptr)
    {
        error = "Error loading preset";
        return false;
    }
    auto parsed_value_tree = juce::ValueTree::fromXml(*xml);
    if(!parsed_value_tree.isValid()) {

        error = "Error converting XML to juce::ValueTree";
        return false;
    }
    if(!loadFromValueTree(parsed_value_tree))
    {
        error = "Error Initializing juce::ValueTree";
        return false;
    }

    //setPresetName(preset.getFileNameWithoutExtension());

    SynthGuiInterface* gui_interface = getGuiInterface();
    if (gui_interface) {
        gui_interface->updateFullGui();
        gui_interface->notifyFresh();
    }

    return true;
}
bool SynthBase::saveToFile(juce::File preset) {
    preset = preset.withFileExtension(juce::String(bitklavier::kPresetExtension));

    juce::File parent = preset.getParentDirectory();
    if (!parent.exists()) {
        if (!parent.createDirectory().wasOk() || !parent.hasWriteAccess())
            return false;
    }

    setPresetName(preset.getFileNameWithoutExtension());

    SynthGuiInterface* gui_interface = getGuiInterface();
    if (gui_interface)
        gui_interface->notifyFresh();

//    if (preset.replaceWithText(saveToJson().dump())) {
//        active_file_ = preset;
//        return true;
//    }
    return false;
}

bool SynthBase::saveToActiveFile() {
    if (!active_file_.exists() || !active_file_.hasWriteAccess())
        return false;

    return saveToFile(active_file_);
}


void SynthBase::processAudio(juce::AudioSampleBuffer* buffer, int channels, int samples, int offset) {
  if (expired_)
    return;
  AudioThreadAction action;
  while (processorInitQueue.try_dequeue (action))
      action();
  engine_->process(samples, *buffer);
  writeAudio(buffer, channels, samples, offset);
}
void SynthBase::processAudioAndMidi(juce::AudioBuffer<float>& audio_buffer, juce::MidiBuffer& midi_buffer) //, int channels, int samples, int offset, int start_sample = 0, int end_sample = 0)
{

    if (expired_)
        return;
    AudioThreadAction action;
    while (processorInitQueue.try_dequeue (action))
        action();

    engine_->processorGraph->processBlock(audio_buffer, midi_buffer);

    //melatonin::printSparkline(audio_buffer);


}
void SynthBase::processAudioWithInput(juce::AudioSampleBuffer* buffer, const bitklavier::mono_float* input_buffer,
                                      int channels, int samples, int offset) {
  if (expired_)
    return;

  engine_->processWithInput(input_buffer, samples);
  writeAudio(buffer, channels, samples, offset);
}

void SynthBase::writeAudio(juce::AudioSampleBuffer* buffer, int channels, int samples, int offset) {
  //const bitklavier::mono_float* engine_output = (const bitklavier::mono_float*)engine_->output(0)->buffer;
  /* get output of engine here */
  for (int channel = 0; channel < channels; ++channel) {
    float* channel_data = buffer->getWritePointer(channel, offset);
    //this line actually sends audio to the JUCE AudioSamplerBuffer to get audio out of the plugin
    for (int i = 0; i < samples; ++i) {
      //channel_data[i] = engine_output[bitklavier::poly_float::kSize * i + channel];
      _ASSERT(std::isfinite(channel_data[i]));
    }
  }
 /*this line would send audio out to draw and get info from */
  //updateMemoryOutput(samples, engine_->output(0)->buffer);
}

void SynthBase::processMidi(juce::MidiBuffer& midi_messages, int start_sample, int end_sample) {
  bool process_all = end_sample == 0;
  for (const juce::MidiMessageMetadata message : midi_messages) {
    int midi_sample = message.samplePosition;
    if (process_all || (midi_sample >= start_sample && midi_sample < end_sample))
      midi_manager_->processMidiMessage(message.getMessage(), midi_sample - start_sample);
  }
}

void SynthBase::processKeyboardEvents(juce::MidiBuffer& buffer, int num_samples) {
  midi_manager_->replaceKeyboardMessages(buffer, num_samples);
}


//void SynthBase::cancelMidiLearn() {
//  midi_manager_->cancelMidiLearn();
//}
//
//void SynthBase::clearMidiLearn(const std::string& name) {
//  midi_manager_->clearMidiLearn(name);
//}

void SynthBase::valueChanged(const std::string& name, bitklavier::mono_float value) {
//  controls_[name]->set(value);
}



void SynthBase::valueChangedThroughMidi(const std::string& name, bitklavier::mono_float value) {
//  controls_[name]->set(value);
//  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, name, value);
//  setValueNotifyHost(name, value);
//  callback->post();
}

int SynthBase::getSampleRate() {
  return engine_->getSampleRate();
}

bool SynthBase::isMidiMapped(const std::string& name) {
  return midi_manager_->isMidiMapped(name);
}

void SynthBase::setAuthor(const juce::String& author) {
  save_info_["author"] = author;
}

void SynthBase::setComments(const juce::String& comments) {
  save_info_["comments"] = comments;
}

void SynthBase::setStyle(const juce::String& style) {
  save_info_["style"] = style;
}

void SynthBase::setPresetName(const juce::String& preset_name) {
  save_info_["preset_name"] = preset_name;
}

void SynthBase::setMacroName(int index, const juce::String& macro_name) {
  save_info_["macro" + std::to_string(index + 1)] = macro_name;
}

juce::String SynthBase::getAuthor() {
  return save_info_["author"];
}

juce::String SynthBase::getComments() {
  return save_info_["comments"];
}

juce::String SynthBase::getStyle() {
  return save_info_["style"];
}

juce::String SynthBase::getPresetName() {
  return save_info_["preset_name"];
}






void SynthBase::notifyOversamplingChanged() {
  pauseProcessing(true);
  engine_->allSoundsOff();
  checkOversampling();
  pauseProcessing(false);
}

void SynthBase::checkOversampling() {
  return engine_->checkOversampling();
}

void SynthBase::ValueChangedCallback::messageCallback() {
  if (auto synth_base = listener.lock()) {
    SynthGuiInterface* gui_interface = (*synth_base)->getGuiInterface();
    if (gui_interface) {
      gui_interface->updateGuiControl(control_name, value);
      if (control_name != "pitch_wheel")
        gui_interface->notifyChange();
    }
  }
}

juce::ValueTree& SynthBase::getValueTree()
{
  return tree;
}

juce::UndoManager& SynthBase::getUndoManager()
{
  return um;
}
