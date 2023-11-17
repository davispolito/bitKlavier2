#include "PluginEditor.h"
#include "interface/FullInterface.h"
#include "interface/look_and_feel/default_look_and_feel.h"
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), was_animating_(true)
{
    juce::ignoreUnused (processorRef);
    setLookAndFeel(DefaultLookAndFeel::instance());
    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    constrainer_.setMinimumSize(bitklavier::kMinWindowWidth, bitklavier::kMinWindowHeight);
    double ratio = (1.0 * bitklavier::kDefaultWindowWidth) / bitklavier::kDefaultWindowHeight;
    constrainer_.setFixedAspectRatio(ratio);
    constrainer_.setGui(gui_.get());
    setConstrainer(&constrainer_);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

PluginEditor::~PluginEditor()
{
}

//void PluginEditor::paint (juce::Graphics& g)
//{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    auto area = getLocalBounds();
//    g.setColour (juce::Colours::white);
//    g.setFont (16.0f);
//    auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
//    g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
//}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.removeFromBottom(50);
    AudioProcessorEditor::resized();
    gui_->setBounds(getLocalBounds());
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
}







void PluginEditor::setScaleFactor(float newScale) {
    AudioProcessorEditor::setScaleFactor(newScale);
    gui_->redoBackground();
}

void PluginEditor::updateFullGui() {
    BitKlavierGuiInterface::updateFullGui();
    //synth_.updateHostDisplay();
}
