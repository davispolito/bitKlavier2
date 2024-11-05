//
// Created by Davis Polito on 2/1/24.
//
#pragma once
#include "synth_section.h"

#include "melatonin_inspector/melatonin_inspector.h"
class SynthSlider;
class TestSection : public SynthSection
{
public:
    class Listener {
    public:
        virtual ~Listener() { }

        //virtual void showAboutSection() = 0;
    };

    TestSection( );

    void paintBackground(juce::Graphics& g) override;
    void resized() override;
//    void reset() override;
//
//    void buttonClicked(juce::Button* clicked_button) override;
//
//    void notifyChange();
//    void notifyFresh();
//    juce::ValueTree v;
    void addListener(Listener* listener) { listeners_.push_back(listener); }
private:
     juce::TextButton inspectButton { "Inspect the UI" };
    std::unique_ptr<melatonin::Inspector> inspector;
    std::unique_ptr<SynthButton> test_button;
    std::unique_ptr<SynthSlider> test_slider;
    std::unique_ptr<SynthSlider> test_slider2;
//    juce::Viewport constructionPort;
//    std::unique_ptr<ConstructionSite> constructionSite_;
    std::vector<Listener*> listeners_;

};
