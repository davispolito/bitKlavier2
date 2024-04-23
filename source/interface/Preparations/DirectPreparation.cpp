//
// Created by Davis Polito on 2/13/24.
//

#include "DirectPreparation.h"
#include "BKitems/BKItem.h"
DirectPreparation::DirectPreparation (juce::ValueTree v, juce::UndoManager& um) : PreparationSection("direct", v, um)
{
    item = std::make_unique<DirectItem> ();
    addOpenGlComponent (item->getImageComponent());
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kDirect);


}
DirectPreparation::~DirectPreparation()
{
}
