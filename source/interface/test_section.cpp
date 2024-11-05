//
// Created by Davis Polito on 2/1/24.
//

#include "test_section.h"
#include "synth_section.h"
#include "synth_slider.h"
TestSection::TestSection() : SynthSection("Test_section")
{
    test_slider = std::make_unique<SynthSlider>("dummy_SLIDER");
    addAndMakeVisible(test_slider.get());
    addSlider(test_slider.get());
    test_slider->setAlwaysOnTop(true);
    //
    test_slider->parentHierarchyChanged();
    test_slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    test_slider2 = std::make_unique<SynthSlider>("dummy_SLIDER2");
    addAndMakeVisible(test_slider2.get());
    addSlider(test_slider2.get());
    test_slider2->setAlwaysOnTop(true);
    //
    test_slider2->parentHierarchyChanged();
    test_slider2->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
//    test_button = std::make_unique<SynthButton>("tes");
//    addButton(test_button.get());

//    test_button->setButtonText("test");
//    test_button->setLookAndFeel(TextLookAndFeel::instance());
//    addAndMakeVisible(test_boutton.get());
   //addAndMakeVisible(&inspectButton);
   //addOpenGlComponent(inspectButton->getGlComponent());

   // this chunk of code instantiates and opens the melatonin inspector
   inspectButton.onClick = [&] {
       if (!inspector)
       {
           inspector = std::make_unique<melatonin::Inspector> (*this);
           inspector->onClose = [this]() { inspector.reset(); };
       }

       inspector->setVisible (true);
   };
   inspectButton.triggerClick();
    setSkinOverride(Skin::kNone);
}

void TestSection::paintBackground(juce::Graphics& g)
{
    SynthSection::paintBackground(g);
    paintContainer(g);

    g.setColour(findColour(Skin::kBody, true));
    paintChildrenBackgrounds(g);

    g.saveState();

    g.restoreState();

    paintKnobShadows(g);
    //drawLabelForComponent(g, test_slider->getName(), test_slider.get());
    //drawLabelForComponent(g, test_button->getName(), test_button.get());
}

void TestSection::resized()
{

    int width = getWidth();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int large_padding = findValue(Skin::kLargePadding);
    test_slider->setBounds(100,100,100, 100);
    test_slider2->setBounds(200,200, getWidth()/40, getHeight()/20);
    //test_button->setBounds(100,100,100,100);
    DBG (":");
    DBG("Test section");
    DBG("display scale" + juce::String(getDisplayScale()));
    DBG("width" + juce::String(getWidth()));
    DBG("height" + juce::String(getHeight()));
    SynthSection::resized();
}