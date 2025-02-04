/*********************************************************************************************/
/*                           Created by Davis Polito and Joshua Warner                       */
/*********************************************************************************************/

#include "DirectPreparation.h"
#include "BKitems/BKItem.h"
#include "synth_slider.h"

// Definition for the DirectPreparation constructor.  It takes three parameters: a pointer to
// a Direct Processor p, a juce::ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private DirectPreparation member proc with an initialization list.
DirectPreparation::DirectPreparation (std::unique_ptr<DirectProcessor> p,
    juce::ValueTree v, OpenGlWrapper& um) :
                         PreparationSection(juce::String("direct"), v, um,p.get()),
                         proc(*p.get()),
                         _proc_ptr(std::move(p))
{

    item = std::make_unique<DirectItem> (); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
        {item->getImageComponent()->init(_open_gl);
        });

    addAndMakeVisible (item.get());
    setSkinOverride (Skin::kDirect);
    juce::MemoryBlock data;

}

std::shared_ptr<SynthSection> DirectPreparation::getPrepPopup()
{
    if(popup_view) {
        popup_view->destroyOpenGlComponents(_open_gl);
        popup_view->reset();
        return popup_view;
    }
    popup_view = std::make_shared<DirectPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void DirectPreparation::resized()
{
    PreparationSection::resized();
}

DirectPreparation::~DirectPreparation()
{
}

juce::AudioProcessor* DirectPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> DirectPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}


void DirectPreparation::paintBackground(juce::Graphics &g)  {
    for (auto * port: objects)
        port->redoImage();
    PreparationSection::paintBackground(g);
}
/*************************************************************************************************/
/*                     NESTED CLASS: DirectPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
DirectPreparation::DirectPopup::DirectPopup(DirectProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(open_gl), view(std::make_unique<DirectParametersView>(proc.getState(), proc.getState().params, &open_gl))
{

    auto& _params = proc.getState().params;
    setSkinOverride (Skin::kDirect);
    Skin default_skin;
    view->setSkinValues(default_skin, false);
    addSubSection(view.get());

}

DirectPreparation::DirectPopup::~DirectPopup()
{
    if ((juce::OpenGLContext::getCurrentContext() == nullptr)) {
        open_gl->context.executeOnGLThread([this](juce::OpenGLContext &openGLContext) {
            destroyOpenGlComponents(*this->open_gl);
        }, true);
    }
}

void DirectPreparation::DirectPopup::initOpenGlComponents(OpenGlWrapper &open_gl) {

    view->initOpenGlComponents(open_gl);
}

// DISPLAY SLIDERS ON-SCREEN
void DirectPreparation::DirectPopup::resized() {

    juce::Rectangle<int> bounds = getLocalBounds();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int section_height = getHeight();
    int section_width = getWidth();
    view->setBounds(getLocalBounds());

    SynthSection::resized();

}
