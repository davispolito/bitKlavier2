//
// Created by Davis Polito on 1/31/25.
//

#include "ModulationLineView.h"
#include "ConstructionSite.h"

ModulationLineView::ModulationLineView(ConstructionSite &site) : SynthSection("modlineview"), site(site)
{
    setInterceptsMouseClicks(false, false);
    setAlwaysOnTop(true);

}

void ModulationLineView::renderOpenGlComponents(OpenGlWrapper &open_gl, bool animate) {

    for(auto line: lines)
    {
        line->render(open_gl,false);
    }
}

//probably want to move this to construction site leaving here for now
void ModulationLineView::preparationDragged(juce::Component *comp, const juce::MouseEvent &e) {
    current_source_ = comp;
    mouse_drag_position_ = getLocalPoint(current_source_, e.getPosition());
    juce::Rectangle<int> global_bounds = getLocalArea(current_source_, current_source_->getLocalBounds());
    juce::Point<int> global_start = global_bounds.getCentre();
}
void ModulationLineView::preparationDropped(const juce::MouseEvent& e, juce::Point<int>)
{
    mouse_drag_position_ = getLocalPoint(current_source_, e.getPosition());
    auto comp =  getComponentAt(mouse_drag_position_.x, mouse_drag_position_.y);
    if(comp)
    {

        DBG(comp->getName());
    }
    site.mouse_drag_position_ = mouse_drag_position_;
}


void ModulationLineView::modulationDropped(const juce::ValueTree &source, const juce::ValueTree &destination)
{
    auto sourceId =juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::fromVar(source.getProperty(IDs::nodeID,-1));
    auto destId =juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::fromVar( destination.getProperty(IDs::nodeID,-1));

    auto* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->addModulationNodeConnection(sourceId, destId);
}
void ModulationLineView::_update()
{

}
