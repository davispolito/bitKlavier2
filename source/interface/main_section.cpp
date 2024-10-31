//
// Created by Davis Polito on 2/1/24.
//

#include "main_section.h"
#include "synth_section.h"
#include "ConstructionSite.h"

MainSection::MainSection(juce::ValueTree v, juce::UndoManager &um, OpenGlWrapper & open_gl, SynthGuiData* data) : SynthSection("main_section"), um(um)
{
    constructionSite_ = std::make_unique<ConstructionSite>(v, um, open_gl, data);
    addMouseListener(constructionSite_.get(), true);
    //addAndMakeVisible(constructionSite_.get());
    //constructionPort.setViewedComponent(constructionSite_.get());
    constructionSite_->view = &constructionPort;
    //constructionPort.setScrollBarsShown(false, false, true, true);
    addSubSection(constructionSite_.get(), true);
    //addAndMakeVisible(constructionSite_.get());
    //addAndMakeVisible(constructionPort);
//    juce::ValueTree t(IDs::PREPARATION);
//
//    t.setProperty(IDs::type,bitklavier::BKPreparationType::PreparationTypeDirect, nullptr);
//    t.setProperty(IDs::x,255, nullptr);
//    t.setProperty(IDs::y,255, nullptr);
//    v.addChild(t,-1, nullptr);
    setSkinOverride(Skin::kNone);
}

void MainSection::paintBackground(juce::Graphics& g)
{
    paintContainer(g);

    g.setColour(findColour(Skin::kBody, true));
    paintChildrenBackgrounds(g);

    g.saveState();

    g.restoreState();


}

void MainSection::resized()
{
    constructionSite_->setColour(Skin::kBody,juce::Colours::antiquewhite);
    int height = getHeight();
    int width = getWidth();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int large_padding = findValue(Skin::kLargePadding);
    constructionSite_->setBounds(large_padding, 0,  width + 1500,  height + 1500);
    //constructionPort.setBounds(large_padding, 0,getDisplayScale()* width, getDisplayScale() * height);
    //constructionPort.setBounds(large_padding, 0,width, height);
    DBG (":");
    DBG("main section");
    DBG("display scale" + juce::String(getDisplayScale()));
    DBG("width" + juce::String(getWidth()));
    DBG("height" + juce::String(getHeight()));
    SynthSection::resized();
}