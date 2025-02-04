//
// Created by Joshua Warner on 6/27/24.
//

/*********************************************************************************************/
/*                           Created by Davis Polito and Joshua Warner                       */
/*********************************************************************************************/

#include "SynchronicPreparation.h"
#include "BKitems/BKItem.h"

#include "synth_slider.h"

// Definition for the SynchronicPreparation constructor.  It takes three parameters: a pointer to
// a Synchronic Processor p, a juce::ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private SynchronicPreparation member proc with an initialization list.
SynchronicPreparation::SynchronicPreparation (std::unique_ptr<SynchronicProcessor> p,
                                            juce::ValueTree v, OpenGlWrapper& um) :
        PreparationSection(juce::String("synchronic"), v, um,p.get()),
        proc(*p.get()),
        _proc_ptr(std::move(p))
{

    item = std::make_unique<SynchronicItem> (); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
                                        {item->getImageComponent()->init(_open_gl); });
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kSynchronic);

}

std::shared_ptr<SynthSection> SynchronicPreparation::getPrepPopup()
{
    if(popup_view)
        popup_view->destroyOpenGlComponents(_open_gl);
    popup_view = std::make_shared<SynchronicPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void SynchronicPreparation::resized()
{
    PreparationSection::resized();
    //slider->setBounds(0, 0, 100, 100);
}

SynchronicPreparation::~SynchronicPreparation()
{
}

juce::AudioProcessor* SynchronicPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> SynchronicPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}

void SynchronicPreparation::SynchronicPopup::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {


    for (auto& sub_section : sub_sections_) {
        if (sub_section->isVisible() && !sub_section->isAlwaysOnTop())
            sub_section->renderOpenGlComponents(open_gl, animate);
    }

    for (auto& open_gl_component : open_gl_components_) {
        if (open_gl_component->isVisible() && !open_gl_component->isAlwaysOnTop()) {
            open_gl_component->render(open_gl, animate);
            GLenum gl =  juce::gl::glGetError();
            _ASSERT(gl == juce::gl::GL_NO_ERROR);
        }
    }


    for (auto& sub_section : sub_sections_) {
        if (sub_section->isVisible() && sub_section->isAlwaysOnTop())
            sub_section->renderOpenGlComponents(open_gl, animate);
    }

    for (auto& open_gl_component : open_gl_components_) {
        if (open_gl_component->isVisible() && open_gl_component->isAlwaysOnTop()) {
            open_gl_component->render(open_gl, animate);
            _ASSERT(juce::gl::glGetError() == juce::gl::GL_NO_ERROR);
        }
    }
    if (background_)
    {
        background_->render(open_gl);
    }
}

/*************************************************************************************************/
/*                     NESTED CLASS: SynchronicPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
SynchronicPreparation::SynchronicPopup::SynchronicPopup(SynchronicProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(open_gl)
{
    auto& _params = proc.getState().params;
}

SynchronicPreparation::SynchronicPopup::~SynchronicPopup()
{
    //TODO
    //constructor sends destroy message to opengl thread
}
void SynchronicPreparation::SynchronicPopup::redoImage()
{
    int mult = getPixelMultiple();
    int image_width = getWidth() * mult;
    int image_height = getHeight() * mult;
}
void SynchronicPreparation::SynchronicPopup::initOpenGlComponents(OpenGlWrapper &open_gl)
{


}

// DISPLAY SLIDERS ON-SCREEN
void SynchronicPreparation::SynchronicPopup::resized()
{


}
