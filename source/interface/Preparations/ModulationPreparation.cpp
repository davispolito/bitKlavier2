//
// Created by Davis Polito on 1/30/25.
//
/*********************************************************************************************/
/*                           Created by Davis Polito and Joshua Warner                       */
/*********************************************************************************************/

#include "ModulationPreparation.h"
#include "BKitems/BKItem.h"
#include "synth_slider.h"
#include "ModulationItem.h"
// Definition for the ModulationPreparation constructor.  It takes three parameters: a pointer to
// a Modulation Processor p, a juce::ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private ModulationPreparation member proc with an initialization list.
ModulationPreparation::ModulationPreparation (std::unique_ptr<bitklavier::ModulationProcessor> p,
                                      juce::ValueTree v, OpenGlWrapper& um) :
        PreparationSection(juce::String("Modulation"), v, um,p.get()),
        proc(*p.get()),
        _proc_ptr(std::move(p))
{

    item = std::make_unique<ModulationItem> (); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
                                        {item->getImageComponent()->init(_open_gl);
                                        });

    addAndMakeVisible (item.get());
    setSkinOverride (Skin::kModulation);
    juce::MemoryBlock data;

    DBG("add processor params");
}

std::shared_ptr<SynthSection> ModulationPreparation::getPrepPopup()
{
    DBG("prep popup");
    if(popup_view) {
        popup_view->destroyOpenGlComponents(_open_gl);
        popup_view->reset();
        return popup_view;
    }
    popup_view = std::make_shared<ModulationPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void ModulationPreparation::resized()
{
    PreparationSection::resized();
}

ModulationPreparation::~ModulationPreparation()
{
}

juce::AudioProcessor* ModulationPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> ModulationPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}


void ModulationPreparation::paintBackground(juce::Graphics &g)  {
    for (auto * port: objects)
        port->redoImage();
    PreparationSection::paintBackground(g);
}
/*************************************************************************************************/
/*                     NESTED CLASS: ModulationPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
ModulationPreparation::ModulationPopup::ModulationPopup(bitklavier::ModulationProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(open_gl)
{

//    auto& _params = proc.getState().params;
    setSkinOverride (Skin::kModulation);
    Skin default_skin;
//    view->setSkinValues(default_skin, false);
//    addSubSection(view.get());

}

ModulationPreparation::ModulationPopup::~ModulationPopup()
{
    if ((juce::OpenGLContext::getCurrentContext() == nullptr)) {
        open_gl->context.executeOnGLThread([this](juce::OpenGLContext &openGLContext) {
            destroyOpenGlComponents(*this->open_gl);
        }, true);
    }
}

void ModulationPreparation::ModulationPopup::initOpenGlComponents(OpenGlWrapper &open_gl) {

//    view->initOpenGlComponents(open_gl);
}

// DISPLAY SLIDERS ON-SCREEN
void ModulationPreparation::ModulationPopup::resized() {

    juce::Rectangle<int> bounds = getLocalBounds();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int section_height = getHeight();
    int section_width = getWidth();
//    view->setBounds(getLocalBounds());

    SynthSection::resized();

}

#include "ModulationPreparation.h"
