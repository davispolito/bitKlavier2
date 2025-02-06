//
// Created by Davis Polito on 2/5/25.
//
#include "ModulationLineView.h"
#include "ModulationLine.h"
#include "ConstructionSite.h"
#include "open_gl_line.h"
ModulationLine::ModulationLine(ConstructionSite *site, ModulationLineView *lineView,
                               const juce::ValueTree &state) : site(site), lineView(lineView), state(state) {

    line = std::make_shared<OpenGlLine>(site->getComponentForPlugin(juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::fromVar(state.getProperty(IDs::src))),
                                        site->getComponentForPlugin(juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::fromVar(state.getProperty(IDs::dest))),
                                        lineView);
}