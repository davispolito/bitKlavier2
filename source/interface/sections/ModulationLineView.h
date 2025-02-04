//
// Created by Davis Polito on 1/31/25.
//

#ifndef BITKLAVIER2_MODULATIONLINEVIEW_H
#define BITKLAVIER2_MODULATIONLINEVIEW_H
#include "PreparationSection.h"
#include "open_gl_line.h"
class ConstructionSite;
class ModulationLineView : public PreparationSection::Listener,
                            public SynthSection
{
public:
    explicit ModulationLineView(ConstructionSite &site);

    ConstructionSite &site;

    void renderOpenGlComponents(OpenGlWrapper &open_gl, bool animate) override;

    std::vector<OpenGlLine*> lines;
    juce::Point<int> mouse_drag_position_;
    juce::Component* current_source_;
   //preparation seciton listerner
    void beginConnectorDrag(juce::AudioProcessorGraph::NodeAndChannel source,
                                    juce::AudioProcessorGraph::NodeAndChannel dest,
                                    const juce::MouseEvent& e) override {}
    void dragConnector(const juce::MouseEvent& e) override {}
    void endDraggingConnector(const juce::MouseEvent& e) override {}
    void preparationDropped(const juce::MouseEvent& e, juce::Point<int>) override;
    void preparationDragged(juce::Component*, const juce::MouseEvent&e) override;
    void modulationDropped(const juce::ValueTree& source, const juce::ValueTree& dest) override;
    void _update() override;
};

#endif //BITKLAVIER2_MODULATIONLINEVIEW_H
