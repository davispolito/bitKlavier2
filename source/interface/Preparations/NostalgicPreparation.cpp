//
// Created by Davis Polito on 4/22/24.
//

#include "NostalgicPreparation.h"
#include "BKitems/BKItem.h"


// Definition for the NostalgicPreparation constructor.  It takes three parameters: a pointer to
// a Nostalgic Processor p, a ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private NostalgicPreparation member proc with an initialization list.
NostalgicPreparation::NostalgicPreparation (std::unique_ptr<NostalgicProcessor> p,
                                      juce::ValueTree v, OpenGlWrapper& um) :
        PreparationSection(juce::String("Nostalgic"), v, um),
        proc(*p.get()),
        _proc_ptr(std::move(p))
{

    item = std::make_unique<BKItem> (bitklavier::BKPreparationType::PreparationTypeNostalgic,*this); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
                                        {item->getImageComponent()->init(_open_gl); });
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kNostalgic);

}

std::shared_ptr<SynthSection> NostalgicPreparation::getPrepPopup()
{
    if(popup_view)
        popup_view->destroyOpenGlComponents(_open_gl);
    popup_view = std::make_shared<NostalgicPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void NostalgicPreparation::resized()
{
    PreparationSection::resized();
}

NostalgicPreparation::~NostalgicPreparation()
{
}

juce::AudioProcessor* NostalgicPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> NostalgicPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}

void NostalgicPreparation::NostalgicPopup::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {


    for (auto& sub_section : sub_sections_) {
        if (sub_section->isVisible() && !sub_section->isAlwaysOnTop())
            sub_section->renderOpenGlComponents(open_gl, animate);
    }

    for (auto& open_gl_component : open_gl_components_) {
        if (open_gl_component->isVisible() && !open_gl_component->isAlwaysOnTop()) {
            open_gl_component->render(open_gl, animate);
            GLenum gl =  juce::gl::glGetError();
            //DBG(String(gl));
            BITKLAVIER_ASSERT(gl == juce::gl::GL_NO_ERROR);
        }
    }

    for (auto& sub_section : sub_sections_) {
        if (sub_section->isVisible() && sub_section->isAlwaysOnTop())
            sub_section->renderOpenGlComponents(open_gl, animate);
    }

    for (auto& open_gl_component : open_gl_components_) {
        if (open_gl_component->isVisible() && open_gl_component->isAlwaysOnTop()) {
            open_gl_component->render(open_gl, animate);
            BITKLAVIER_ASSERT(juce::gl::glGetError() == juce::gl::GL_NO_ERROR);
        }
    }
    if (background_)
    {
        background_->render(open_gl);
    }
}

/*************************************************************************************************/
/*                     NESTED CLASS: NostalgicPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
NostalgicPreparation::NostalgicPopup::NostalgicPopup(NostalgicProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(_open_gl)
{
    auto& _params = proc.getState().params;
}

NostalgicPreparation::NostalgicPopup::~NostalgicPopup()
{
    //TODO
    //constructor sends destroy message to opengl thread
}
void NostalgicPreparation::NostalgicPopup::redoImage()
{
    int mult = getPixelMultiple();
    int image_width = getWidth() * mult;
    int image_height = getHeight() * mult;
}
void NostalgicPreparation::NostalgicPopup::initOpenGlComponents(OpenGlWrapper &open_gl)
{


}

// DISPLAY SLIDERS ON-SCREEN
void NostalgicPreparation::NostalgicPopup::resized()
{


}
