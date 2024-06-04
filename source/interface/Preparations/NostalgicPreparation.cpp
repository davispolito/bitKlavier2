//
// Created by Davis Polito on 4/22/24.
//

#include "NostalgicPreparation.h"
#include "BKitems/BKItem.h"



NostalgicPreparation::NostalgicPreparation (std::shared_ptr<juce::AudioProcessor> a,juce::ValueTree v, OpenGlWrapper& um) : PreparationSection("nostalgic", v, um)
{
    item = std::make_unique<NostalgicItem> ();
    addOpenGlComponent (item->getImageComponent());
    addAndMakeVisible (item.get());
    
    setSkinOverride (Skin::kNostalgic);
    _open_gl.init_comp.push_back(item->getImageComponent());
    _open_gl.initOpenGlComp.enqueue([this]
                                    {item->getImageComponent()->init(_open_gl); });

}
NostalgicPreparation::~NostalgicPreparation()
{
}