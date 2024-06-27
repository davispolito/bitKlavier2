//
// Created by Davis Polito on 6/26/24.
//

#ifndef BITKLAVIER2_PREPARATIONSELECTOR_H
#define BITKLAVIER2_PREPARATIONSELECTOR_H
#include <JuceHeader.h>
#include "PreparationSection.h"
class ConstructionSite;
class PreparationSelector : public LassoSource<PreparationSection*>
{
public:
    explicit PreparationSelector (const ConstructionSite&);
    
    void findLassoItemsInArea (Array<PreparationSection*>& results, const juce::Rectangle<int>& area) override;
    SelectedItemSet<PreparationSection*>& getLassoSelection() override { return selectedPreparationSet; }

private:
    const ConstructionSite& csite;
    SelectedItemSet<PreparationSection*> selectedPreparationSet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreparationSelector)
};

#endif //BITKLAVIER2_PREPARATIONSELECTOR_H
