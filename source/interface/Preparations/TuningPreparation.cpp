//
// Created by Joshua Warner on 6/27/24.
//


/*********************************************************************************************/
/*                           Created by Davis Polito and Joshua Warner                       */
/*********************************************************************************************/

#include "TuningPreparation.h"
#include "BKitems/BKItem.h"

#include "synth_slider.h"

// Definition for the TuningPreparation constructor.  It takes three parameters: a pointer to
// a Tuning Processor p, a juce::ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private TuningPreparation member proc with an initialization list.
TuningPreparation::TuningPreparation (std::unique_ptr<TuningProcessor> p,
                                    juce::ValueTree v, OpenGlWrapper& um) :
        PreparationSection(juce::String("tuning"), v, um,p.get()),
        proc(*p.get()),
        _proc_ptr(std::move(p))
{

    item = std::make_unique<TuningItem> (); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
                                        {item->getImageComponent()->init(_open_gl); });
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kTuning);

}

std::shared_ptr<SynthSection> TuningPreparation::getPrepPopup()
{
    if(popup_view)
        popup_view->destroyOpenGlComponents(_open_gl);
    popup_view = std::make_shared<TuningPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void TuningPreparation::resized()
{
    PreparationSection::resized();
    //slider->setBounds(0, 0, 100, 100);
}

TuningPreparation::~TuningPreparation()
{
}

juce::AudioProcessor* TuningPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> TuningPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}

void TuningPreparation::TuningPopup::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {


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
/*                     NESTED CLASS: TuningPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
TuningPreparation::TuningPopup::TuningPopup(TuningProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(open_gl)
{
    auto& _params = proc.getState().params;
}

TuningPreparation::TuningPopup::~TuningPopup()
{
    //TODO
    //constructor sends destroy message to opengl thread
}
void TuningPreparation::TuningPopup::redoImage()
{
    int mult = getPixelMultiple();
    int image_width = getWidth() * mult;
    int image_height = getHeight() * mult;
}
void TuningPreparation::TuningPopup::initOpenGlComponents(OpenGlWrapper &open_gl)
{


}

// DISPLAY SLIDERS ON-SCREEN
void TuningPreparation::TuningPopup::resized()
{


}

