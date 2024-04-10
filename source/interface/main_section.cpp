//
// Created by Davis Polito on 2/1/24.
//

#include "main_section.h"
#include "ConstructionSite.h"

MainSection::MainSection(juce::ValueTree v, juce::UndoManager &um, OpenGlWrapper & open_gl) : SynthSection("main_section"), v(v), um(um)
{
    constructionSite_ = std::make_unique<ConstructionSite>(v, um, open_gl);
    //addAndMakeVisible(constructionSite_.get());
    //constructionPort.setViewedComponent(constructionSite_.get());
    //constructionPort.setLookAndFeel()
    //constructionPort.setScrollBarsShown(false, false, true, true);
    addSubSection(constructionSite_.get());
    //addAndMakeVisible(constructionSite_.get());
    //addAndMakeVisible(constructionPort);
//    ValueTree t(IDs::PREPARATION);
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
    constructionSite_->setColour(Skin::kBody,Colours::antiquewhite);
    int height = getHeight();
    int width = getWidth();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int large_padding = findValue(Skin::kLargePadding);
//
    constructionSite_->setBounds(large_padding, 0,width, height);
    //constructionPort.setBounds(large_padding, 0,width, height);
    SynthSection::resized();
}