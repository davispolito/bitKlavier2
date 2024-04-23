//
// Created by Davis Polito on 4/22/24.
//

#include "NostalgicPreparation.h"
#include "BKitems/BKItem.h"
NostalgicPreparation::NostalgicPreparation (juce::ValueTree v, juce::UndoManager& um) : PreparationSection("nostalgic", v, um)
{
    item = std::make_unique<NostalgicItem> ();
    addOpenGlComponent (item->getImageComponent());
    addAndMakeVisible (item.get());
    
    setSkinOverride (Skin::kNostalgic);


}
NostalgicPreparation::~NostalgicPreparation()
{
}