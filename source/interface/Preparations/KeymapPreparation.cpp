//
// Created by Davis Polito on 6/24/24.
//

#include "KeymapPreparation.h"
#include "FullInterface.h"
KeymapPreparation::KeymapPreparation (std::unique_ptr<KeymapProcessor> p,
                                      juce::ValueTree v, OpenGlWrapper& um) :
        PreparationSection(juce::String("keymap"), v, um),
        proc(*p.get()),
        _proc_ptr(std::move(p))
{

    item = std::make_unique<BKItem> (bitklavier::BKPreparationType::PreparationTypeKeymap, *this); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
                                        {item->getImageComponent()->init(_open_gl); });
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kKeymap);

//    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
//    if (parent) {
//        AudioDeviceManager* device_manager = parent->getAudioDeviceManager();
//        if (device_manager)
//                    (*proc._midi.get(), proc._midi->enabledMidiInputs, *device_manager);
//            addAndMakeVisible(midi_selector_.get());
//            addOpenGlComponent(midi_selector_->getImageComponent());
//        }
//    }

}

KeymapPreparation::~KeymapPreparation()
{

}

std::shared_ptr<SynthSection> KeymapPreparation::getPrepPopup()
{
    if(popup_view)
        popup_view->destroyOpenGlComponents(_open_gl);
    popup_view = std::make_shared<KeymapPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}

juce::AudioProcessor* KeymapPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> KeymapPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}



KeymapPreparation::KeymapPopup::KeymapPopup(KeymapProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), opengl(open_gl),
PreparationPopup(_open_gl)
 {

    auto &_params = proc.getState().params;

}
void KeymapPreparation::KeymapPopup::resized()
{
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent && midi_selector_ == nullptr) {
        AudioDeviceManager* device_manager = parent->getAudioDeviceManager();
        if (device_manager) {
            midi_selector_ = std::make_unique<OpenGlMidiSelector>
                    (*proc._midi.get(), proc._midi->enabledMidiInputs, *device_manager);
            addAndMakeVisible(midi_selector_.get());
            addOpenGlComponent(midi_selector_->getImageComponent());
            parent->getGui()->open_gl_.initOpenGlComp.try_enqueue([this]
                                               {SynthGuiInterface* parent = this->findParentComponentOfClass<SynthGuiInterface>();
                midi_selector_->getImageComponent()->init(parent->getGui()->open_gl_); });
        }
    }
    if (midi_selector_) {
        midi_selector_->setBounds(10, 10, 200, 200);
        midi_selector_->redoImage();
        midi_selector_->setRowHeight(22);
        Colour background = findColour(Skin::kPopupBackground, true);
        setColorRecursively(midi_selector_.get(), ListBox::backgroundColourId, background);
        //setColorRecursively(midi_selector_.get(), ComboBox::backgroundColourId, background);
        //setColorRecursively(midi_selector_.get(), PopupMenu::backgroundColourId, background);
        //setColorRecursively(midi_selector_.get(), BubbleComponent::backgroundColourId, background);

        Colour text = findColour(Skin::kBodyText, true);
        setColorRecursively(midi_selector_.get(), ListBox::textColourId, text);
        setColorRecursively(midi_selector_.get(), ComboBox::textColourId, text);

        //setColorRecursively(midi_selector_.get(), TextEditor::highlightColourId, Colours::transparentBlack);
        setColorRecursively(midi_selector_.get(), ListBox::outlineColourId, Colours::transparentBlack);
        //setColorRecursively(midi_selector_.get(), ComboBox::outlineColourId, Colours::transparentBlack);
    }
}

void KeymapPreparation::KeymapPopup::redoImage()
{
    int mult = getPixelMultiple();
    int image_width = getWidth() * mult;
    int image_height = getHeight() * mult;
    //Image background_image(Image::ARGB,image_width, image_height, true);
    //Graphics g (background_image);

    //paintKnobShadows(g);
    //sliderShadows.setOwnImage(background_image);

}
void KeymapPreparation::KeymapPopup::initOpenGlComponents(OpenGlWrapper &open_gl) {
//    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
//    if (parent && midi_selector_ == nullptr) {
//        AudioDeviceManager* device_manager = parent->getAudioDeviceManager();
//        if (device_manager) {
//            midi_selector_ = std::make_unique<OpenGlMidiSelector>
//                    (*proc._midi.get(), proc._midi->enabledMidiInputs, *device_manager);
//            addAndMakeVisible(midi_selector_.get());
//            addOpenGlComponent(midi_selector_->getImageComponent());
//        }
//    }


}

KeymapPreparation::KeymapPopup::~KeymapPopup()
{
    //TODO
    //constructor sends destroy message to opengl thread
}

