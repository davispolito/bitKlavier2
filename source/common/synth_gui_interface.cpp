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

#include "synth_gui_interface.h"

#include "SampleLoadManager.h"
#include "UserPreferences.h"

#include "sound_engine.h"
#include "DirectProcessor.h"

SynthGuiData::SynthGuiData(SynthBase* synth_base) : synth(synth_base),
                                                     tree(synth_base->getValueTree()),
                                                     um(synth_base->getUndoManager())

{
    //tree = mainSynth->getValueTree();
//    um = synth_base->getUndoManager();
    //sampleLoadManager->loadSamples(0, true);
}
#if HEADLESS

SynthGuiInterface::SynthGuiInterface(SynthBase* mainSynth, bool use_gui) : synth_(mainSynth) { }
SynthGuiInterface::~SynthGuiInterface() { }
void SynthGuiInterface::updateFullGui() { }
void SynthGuiInterface::updateGuiControl(const std::string& name, float value) { }
float SynthGuiInterface::getControlValue(const std::string& name) { return 0.0f; }
void SynthGuiInterface::connectModulation(std::string source, std::string destination) { }
void SynthGuiInterface::connectModulation(bitklavier::ModulationConnection* connection) { }
void SynthGuiInterface::setModulationValues(const std::string& source, const std::string& destination,
                                            float amount, bool bipolar, bool stereo, bool bypass) { }
void SynthGuiInterface::disconnectModulation(std::string source, std::string destination) { }
void SynthGuiInterface::disconnectModulation(bitklavier::ModulationConnection* connection) { }
void SynthGuiInterface::setFocus() { }
void SynthGuiInterface::notifyChange() { }
void SynthGuiInterface::notifyFresh() { }
void SynthGuiInterface::openSaveDialog() { }
void SynthGuiInterface::externalPresetLoaded(juce::File preset) { }
void SynthGuiInterface::setGuiSize(float scale) { }

#else
#include <memory>
#include "../interface/look_and_feel/default_look_and_feel.h"
#include "../interface/fullInterface.h"


SynthGuiInterface::SynthGuiInterface(SynthBase* synth, bool use_gui) : synth_(synth), userPreferences(new UserPreferencesWrapper()),
                                                                       sampleLoadManager(new SampleLoadManager(userPreferences->userPreferences.get(),synth))
                                                                        {
  if (use_gui) {
    SynthGuiData synth_data(synth_);
    gui_ = std::make_unique<FullInterface>(&synth_data);
  }
    sampleLoadManager->preferences = userPreferences->userPreferences.get();
    //sampleLoadManager->loadSamples(0, true);
}

SynthGuiInterface::~SynthGuiInterface() { }

void SynthGuiInterface::updateFullGui() {
  if (gui_ == nullptr)
    return;

//  gui_->setAllValues(synth_->getControls());
  gui_->reset();
}
 OpenGlWrapper* SynthGuiInterface::getOpenGlWrapper() {
    return &gui_->open_gl_;
}

void SynthGuiInterface::updateGuiControl(const std::string& name, float value) {
    if (gui_ == nullptr)
        return;

//  gui_->setValue(name, value, NotificationType::dontSendNotification);
}
//void SynthGuiInterface::notifyModulationsChanged() {
//  gui_->modulationChanged();
//}

//void SynthGuiInterface::notifyModulationValueChanged(int index) {
//  gui_->modulationValueChanged(index);
//}
//
//void SynthGuiInterface::connectModulation(std::string source, std::string destination) {
//  bool created = synth_->connectModulation(source, destination);
//  if (created)
//    initModulationValues(source, destination);
//  notifyModulationsChanged();
//}

//void SynthGuiInterface::connectModulation(bitklavier::ModulationConnection* connection) {
//  synth_->connecotModulation(connection);
//  notifyModulationsChanged();
//}




bool SynthGuiInterface::loadFromFile(juce::File preset, std::string &error) {
    return getSynth()->loadFromFile(preset,error);
    //sampleLoadManager->loadSamples()
}


void SynthGuiInterface::setFocus() {
  if (gui_ == nullptr)
    return;

  gui_->setFocus();
}

void SynthGuiInterface::notifyChange() {
  if (gui_ == nullptr)
    return;

  gui_->notifyChange();
}

void SynthGuiInterface::notifyFresh() {
  if (gui_ == nullptr)
    return;

  gui_->notifyFresh();
}




void SynthGuiInterface::setGuiSize(float scale) {
  if (gui_ == nullptr)
    return;

  juce::Point<int> position = gui_->getScreenBounds().getCentre();
  const juce::Displays::Display& display = juce::Desktop::getInstance().getDisplays().findDisplayForPoint(position);

  juce::Rectangle<int> display_area = juce::Desktop::getInstance().getDisplays().getTotalBounds(true);
  juce::ComponentPeer* peer = gui_->getPeer();
  if (peer)
    peer->getFrameSize().subtractFrom(display_area);

  float window_size = scale / display.scale;
  window_size = std::min(window_size, display_area.getWidth() * 1.0f / bitklavier::kDefaultWindowWidth);
  window_size = std::min(window_size, display_area.getHeight() * 1.0f / bitklavier::kDefaultWindowHeight);
  //LoadSave::saveWindowSize(window_size);

  int width = std::round(window_size * bitklavier::kDefaultWindowWidth);
  int height = std::round(window_size * bitklavier::kDefaultWindowHeight);

  juce::Rectangle<int> bounds = gui_->getBounds();
  bounds.setWidth(width);
  bounds.setHeight(height);
  gui_->getParentComponent()->setBounds(bounds);
  gui_->redoBackground();
}

void SynthGuiInterface::createProcessor(const juce::ValueTree &v) {
     DirectProcessor d(v);
     auto proc = bitklavier::Processor::create("direct",v);
     DBG(proc->getName());
}
#endif
