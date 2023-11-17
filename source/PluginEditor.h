#pragma once
#include "juce_graphics/juce_graphics.h"
#include "common/border_bounds_constrainer.h"
#include "PluginProcessor.h"
#include "BinaryData.h"
#include "interface/bitKlavierGuiInterface.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, public BitKlavierGuiInterface
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override { }
    void resized() override;
    void setScaleFactor(float newScale) override;

    void updateFullGui() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    BorderBoundsConstrainer constrainer_;
    std::unique_ptr<melatonin::Inspector> inspector;
    bool was_animating_;
    juce::TextButton inspectButton { "Inspect the UI" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
