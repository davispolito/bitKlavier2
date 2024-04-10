//
// Created by Davis Polito on 12/15/23.
//

#ifndef BITKLAVIER2_PREPARATIONMODULE_H
#define BITKLAVIER2_PREPARATIONMODULE_H

#include "juce_data_structures/juce_data_structures.h"
struct ModuleData {
    juce::ValueTree& prepTree;
    juce::ValueTree& controls;
    juce::ValueTree& inputs;
    juce::ValueTree& outputs;
};

class PreparationModule
{
public:
};

#endif //BITKLAVIER2_PREPARATIONMODULE_H
