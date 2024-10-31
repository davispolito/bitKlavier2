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

#include "header_section.h"

#include "fonts.h"
#include <memory>
#include "text_look_and_feel.h"
#include "load_save.h"
#include "SampleLoadManager.h"
LogoSection::LogoSection() : SynthSection("logo_section") {
#if !defined(NO_TEXT_ENTRY)
  logo_button_ = std::make_unique<LogoButton>("logo");

  addAndMakeVisible(logo_button_.get());
  addOpenGlComponent(logo_button_->getImageComponent());
  logo_button_->addListener(this);
#endif


  setSkinOverride(Skin::kLogo);
}

void LogoSection::resized() {
  int logo_padding_y = kLogoPaddingY;
  int logo_height = getHeight() - 2 * logo_padding_y;
  int logo_padding_x = (getWidth() - logo_height) / 2;
  if (logo_button_)
    logo_button_->setBounds(logo_padding_x, logo_padding_y, logo_height, logo_height);
}

void LogoSection::paintBackground(juce::Graphics& g) {
  if (logo_button_) {
    logo_button_->setRingColors(findColour(Skin::kWidgetPrimary1, true), findColour(Skin::kWidgetPrimary2, true));
    logo_button_->setLetterColors(findColour(Skin::kWidgetSecondary1, true), findColour(Skin::kWidgetSecondary2, true));
  }
}

void LogoSection::buttonClicked(juce::Button* clicked_button) {
  for (Listener* listener : listeners_)
    listener->showAboutSection();
}

HeaderSection::HeaderSection() : SynthSection("header_section"), tab_offset_(0), body_(new OpenGlQuad(Shaders::kRoundedRectangleFragment)) {
  //logo_section_ = std::make_unique<LogoSection>();
  //addSubSection(logo_section_.get());
  //logo_section_->addListener(this);

    addOpenGlComponent(body_);
    logo_section_ = std::make_unique<LogoSection>();
    logo_section_->setAlwaysOnTop(true);
    logo_section_->addListener(this);
    addSubSection(logo_section_.get());
    sampleSelector = std::make_unique<juce::ShapeButton>("Selector", juce::Colour(0xff666666),
                                  juce::Colour(0xffaaaaaa), juce::Colour(0xff888888));

    addAndMakeVisible(sampleSelector.get());
    sampleSelector->addListener(this);
    sampleSelector->setTriggeredOnMouseDown(true);
    sampleSelector->setShape(juce::Path(), true, true, true);

    currentSampleType = 0;
    sampleSelectText = std::make_shared<PlainTextComponent>("Sample Select Text", "---");
    addOpenGlComponent(sampleSelectText);

    printButton = std::make_unique<SynthButton>("header_print");
    addButton(printButton.get());
    printButton->setButtonText("PRINT");
    printButton->setLookAndFeel(TextLookAndFeel::instance());
    printButton->addListener(this);

    saveButton = std::make_unique<SynthButton>("header_save");
    addButton(saveButton.get());
    saveButton->setButtonText("save");
    saveButton->setLookAndFeel(TextLookAndFeel::instance());
    saveButton->addListener(this);

    loadButton = std::make_unique<SynthButton>("header_load");
    addButton(loadButton.get());
    loadButton->setButtonText("load");
    loadButton->setLookAndFeel(TextLookAndFeel::instance());
    loadButton->addListener(this);

  setAlwaysOnTop(true);
  setSkinOverride(Skin::kHeader);
}

void HeaderSection::paintBackground(juce::Graphics& g) {
  paintContainer(g);
    paintChildrenBackgrounds(g);
  g.setColour(findColour(Skin::kBody, true));
  int logo_section_width = 32.0 + getPadding();
 g.fillRect(0, 0, logo_section_width, getHeight());

  paintKnobShadows(g);



}

void HeaderSection::resized() {
  static constexpr float kTextHeightRatio = 0.3f;
  static constexpr float kPaddingLeft = 0.25f;
    juce::Colour body_text = findColour(Skin::kBodyText, true);
    sampleSelectText->setColor(body_text);
//  oscilloscope_->setColour(Skin::kBody, findColour(Skin::kBackground, true));
//  spectrogram_->setColour(Skin::kBody, findColour(Skin::kBackground, true));
  int height = getHeight();
  int width = getWidth();

    body_->setBounds(getLocalBounds());
    body_->setRounding(findValue(Skin::kBodyRounding));
    body_->setColor(findColour(Skin::kBody, true));
  int widget_margin = findValue(Skin::kWidgetMargin);
  int large_padding = findValue(Skin::kLargePadding);

    int logo_width = findValue(Skin::kModulationButtonWidth);
    int label_height = findValue(Skin::kLabelBackgroundHeight);
    logo_section_->setBounds(0, 0, logo_width , height );
    sampleSelector->setBounds(logo_width  + widget_margin, 10, 100, label_height);
    sampleSelectText->setBounds(sampleSelector->getBounds());
    float label_text_height = findValue(Skin::kLabelHeight);
    sampleSelectText->setTextSize(label_text_height);
    printButton->setBounds(sampleSelector->getX() + 100, sampleSelector->getY(), 80, 20);
    loadButton->setBounds(printButton->getX() + 100, sampleSelector->getY(),80, 20);
    saveButton->setBounds(loadButton->getX() + 100, sampleSelector->getY(), 80, 20);
  //int logo_width = findValue(Skin::kModulationButtonWidth);
 // logo_section_->setBounds(large_padding, 0, logo_width, height);
  //inspectButton->setBounds(large_padding, 0, 100, height);

  int preset_selector_width = width / 3;
  int preset_selector_height = height * 0.6f;
  int preset_selector_buffer = (height - preset_selector_height) * 0.5f;
  int preset_selector_x = (getWidth() - preset_selector_width + 2 * preset_selector_height) / 2;
 // synth_preset_selector_->setBounds(preset_selector_x, preset_selector_buffer,
//                                    preset_selector_width, preset_selector_height);
  int component_padding = 12 * size_ratio_;
//  int volume_width = (width - synth_preset_selector_->getRight() - 2 * component_padding) / 2;
//  int oscilloscope_width = volume_width - widget_margin;
//  int oscilloscope_x = width - oscilloscope_width - large_padding;
//  volume_section_->setBounds(synth_preset_selector_->getRight() + component_padding, 0, volume_width, height);
//  oscilloscope_->setBounds(oscilloscope_x, widget_margin, oscilloscope_width, height - 2 * widget_margin);
//  view_spectrogram_->setBounds(oscilloscope_->getBounds());
//  spectrogram_->setBounds(oscilloscope_->getBounds());

  int tabs_width = preset_selector_x - component_padding - tab_offset_;
//  tab_selector_->setBounds(tab_offset_, 0, tabs_width, height);
//  exit_temporary_button_->setBounds(tab_offset_, 0, height, height);

//  float temporary_height = getHeight() * kTextHeightRatio;
//  temporary_tab_->setTextSize(temporary_height);

//  int temporary_x = exit_temporary_button_->getRight() + getHeight() * kPaddingLeft;
  //int temporary_width = synth_preset_selector_->getX() - temporary_x;
//  temporary_tab_->setBounds(temporary_x, 0, temporary_width, getHeight());

  SynthSection::resized();
}

void HeaderSection::reset() {
//  if (preset_selector_)
//    //synth_preset_selector_->resetText();
}

//void HeaderSection::setAllValues() {
//  //SynthSection::setAllValues(controls);
////  bool view_spectrogram = view_spectrogram_->getToggleState();
////  oscilloscope_->setVisible(!view_spectrogram);
////  spectrogram_->setVisible(view_spectrogram);
//}
namespace string_constants{

    // i think the sample library menu should be autopopulated by subfolders in the samples folder
    // and also by whatever other folders the user specifies in preferences for storing samples
    // including SoundFonts
    static const std::vector<std::string> cBKSampleLoadTypes = {
            "Piano (Default)",
            "Library 2",
            "Library 3",
            "Library 4"
    };

};
void HeaderSection::buttonClicked(juce::Button* clicked_button) {
  if (clicked_button == exit_temporary_button_.get()) {
//    for (Listener* listener : listeners_)
//      listener->clearTemporaryTab(tab_selector_->getValue());
//    setTemporaryTab("");
  }
//  else if (clicked_button == view_spectrogram_.get()) {
//    bool view_spectrogram = view_spectrogram_->getToggleState();
//    oscilloscope_->setVisible(!view_spectrogram);
//    spectrogram_->setVisible(view_spectrogram);
//    SynthSection::buttonClicked(clicked_button);
//  }
  else if (clicked_button == sampleSelector.get())
    {
      PopupItems options;
      for(int i = 0; i < bitklavier::utils::BKSampleLoadType::BKNumSampleLoadTypes; i++)
      {
            options.addItem(i, string_constants::cBKSampleLoadTypes[i]);
      }

      juce::Point<int> position(sampleSelector->getX(), sampleSelector->getBottom());
      showPopupSelector(this,position, options, [=](int selection){

          SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
          parent->sampleLoadManager->loadSamples(selection, true);
          sampleSelectText->setText(string_constants::cBKSampleLoadTypes[selection]);
          resized();
      });

    }
  else if (clicked_button == printButton.get())
    {
      SynthGuiInterface* interface = findParentComponentOfClass<SynthGuiInterface>();
      interface->getSynth()->getValueTree().getChild(0).setProperty("sync", 1, nullptr);
      DBG(interface->getSynth()->getValueTree().toXmlString());
    }
  else if (clicked_button == loadButton.get())
    {
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        juce::File active_file = parent->getSynth()->getActiveFile();
        filechooser = std::make_unique<juce::FileChooser> ("Open Preset", active_file, juce::String("*.") + bitklavier::kPresetExtension);

        auto flags = juce::FileBrowserComponent::openMode
                     | juce::FileBrowserComponent::canSelectFiles;
        filechooser->launchAsync(flags, [this] (const juce::FileChooser& fc) {
            if (fc.getResult() == juce::File{})
            {
               return ;
            }
            SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();
            std::string error;
                    juce::File choice = fc.getResult();
        if (!_parent->getSynth()->loadFromFile(choice, error)) {
//            std::string name = ProjectInfo::projectName;
//            error = "There was an error open the preset. " + error;
            //juce::AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "PRESET ERROR, ""Error opening preset", error);
            DBG("errrrrr");
            DBG(error);
        }
//        else
//            parent->externalPresetLoaded(choice);

        });

    }
    else if (clicked_button == saveButton.get())
    {

    }
    else
        SynthSection::buttonClicked(clicked_button);
}

void HeaderSection::sliderValueChanged(juce::Slider* slider) {
//  if (slider == tab_selector_.get()) {
//    int index = tab_selector_->getValue();
//    for (Listener* listener : listeners_)
//      listener->tabSelected(index);
//  }
//  else
    SynthSection::sliderValueChanged(slider);
}

//void HeaderSection::setPresetBrowserVisibility(bool visible) {
//  for (Listener* listener : listeners_)
//    listener->setPresetBrowserVisibility(visible, tab_selector_->getValue());
//}
//
//void HeaderSection::setBankExporterVisibility(bool visible) {
//  for (Listener* listener : listeners_)
//    listener->setBankExporterVisibility(visible, tab_selector_->getValue());
//}
//
//void HeaderSection::deleteRequested(juce::File preset) {
//  for (Listener* listener : listeners_)
//    listener->deleteRequested(preset);
//}
//
//void HeaderSection::bankImported() {
//  for (Listener* listener : listeners_)
//    listener->bankImported();
//}
//
//void HeaderSection::save(juce::File preset) {
//  synth_preset_selector_->resetText();
//  synth_preset_selector_->setModified(false);
//}
//
//void HeaderSection::setTemporaryTab(juce::String name) {
//  temporary_tab_->setText(name);
//  tab_selector_->setVisible(name.isEmpty());
//  exit_temporary_button_->setVisible(!name.isEmpty());
//  repaint();
//  repaintBackground();
//}
//
//void HeaderSection::setOscilloscopeMemory(const vital::poly_float* memory) {
//  oscilloscope_->setOscilloscopeMemory(memory);
//}
//
//void HeaderSection::setAudioMemory(const vital::StereoMemory* memory) {
//  spectrogram_->setAudioMemory(memory);
//}

//void HeaderSection::notifyChange() {
//  synth_preset_selector_->setModified(true);
//}
//
//void HeaderSection::notifyFresh() {
//  synth_preset_selector_->resetText();
//  synth_preset_selector_->setModified(false);
//}
