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

#include "synth_base.h"
#include "../synthesis/synth_engine/sound_engine.h"


SynthGuiData::SynthGuiData(SynthBase* synth_base) : synth(synth_base),
                                                     tree(synth_base->getValueTree()),
                                                     um(synth_base->getUndoManager())
{
    //tree = synth->getValueTree();
//    um = synth_base->getUndoManager();
}
#if HEADLESS

SynthGuiInterface::SynthGuiInterface(SynthBase* synth, bool use_gui) : synth_(synth) { }
SynthGuiInterface::~SynthGuiInterface() { }
void SynthGuiInterface::updateFullGui() { }
void SynthGuiInterface::updateGuiControl(const std::string& name, bitklavier::mono_float value) { }
bitklavier::mono_float SynthGuiInterface::getControlValue(const std::string& name) { return 0.0f; }
void SynthGuiInterface::connectModulation(std::string source, std::string destination) { }
void SynthGuiInterface::connectModulation(bitklavier::ModulationConnection* connection) { }
void SynthGuiInterface::setModulationValues(const std::string& source, const std::string& destination,
                                            bitklavier::mono_float amount, bool bipolar, bool stereo, bool bypass) { }
void SynthGuiInterface::disconnectModulation(std::string source, std::string destination) { }
void SynthGuiInterface::disconnectModulation(bitklavier::ModulationConnection* connection) { }
void SynthGuiInterface::setFocus() { }
void SynthGuiInterface::notifyChange() { }
void SynthGuiInterface::notifyFresh() { }
void SynthGuiInterface::openSaveDialog() { }
void SynthGuiInterface::externalPresetLoaded(File preset) { }
void SynthGuiInterface::setGuiSize(float scale) { }

#else
#include <memory>
#include "../interface/look_and_feel/default_look_and_feel.h"
#include "../interface/fullInterface.h"


SynthGuiInterface::SynthGuiInterface(SynthBase* synth, bool use_gui) : synth_(synth) {
  if (use_gui) {
    SynthGuiData synth_data(synth_);
    gui_ = std::make_unique<FullInterface>(&synth_data);
  }

}

SynthGuiInterface::~SynthGuiInterface() { }

void SynthGuiInterface::updateFullGui() {
  if (gui_ == nullptr)
    return;

//  gui_->setAllValues(synth_->getControls());
  gui_->reset();
}

void SynthGuiInterface::updateGuiControl(const std::string& name, bitklavier::mono_float value) {
  if (gui_ == nullptr)
    return;

//  gui_->setValue(name, value, NotificationType::dontSendNotification);
}

//bitklavier::mono_float SynthGuiInterface::getControlValue(const std::string& name) {
//  return synth_->getControls()[name]->value();
//}

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
//  synth_->connectModulation(connection);
//  notifyModulationsChanged();
//}







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
  const Displays::Display& display = Desktop::getInstance().getDisplays().findDisplayForPoint(position);

  Rectangle<int> display_area = Desktop::getInstance().getDisplays().getTotalBounds(true);
  ComponentPeer* peer = gui_->getPeer();
  if (peer)
    peer->getFrameSize().subtractFrom(display_area);

  float window_size = scale / display.scale;
  window_size = std::min(window_size, display_area.getWidth() * 1.0f / bitklavier::kDefaultWindowWidth);
  window_size = std::min(window_size, display_area.getHeight() * 1.0f / bitklavier::kDefaultWindowHeight);
  //LoadSave::saveWindowSize(window_size);

  int width = std::round(window_size * bitklavier::kDefaultWindowWidth);
  int height = std::round(window_size * bitklavier::kDefaultWindowHeight);

  Rectangle<int> bounds = gui_->getBounds();
  bounds.setWidth(width);
  bounds.setHeight(height);
  gui_->getParentComponent()->setBounds(bounds);
  gui_->redoBackground();
}
#endif
