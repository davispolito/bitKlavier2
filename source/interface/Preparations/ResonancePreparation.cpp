/*********************************************************************************************/
/*                           Created by Davis Polito and Joshua Warner                       */
/*********************************************************************************************/

#include "ResonancePreparation.h"
#include "BKitems/BKItem.h"

#include "synth_slider.h"

// Definition for the ResonancePreparation constructor.  It takes three parameters: a pointer to
// a Resonance Processor p, a ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private ResonancePreparation member proc with an initialization list.
ResonancePreparation::ResonancePreparation (std::unique_ptr<ResonanceProcessor> p,
                                      juce::ValueTree v, OpenGlWrapper& um) :
        PreparationSection(juce::String("resonance"), v, um),
        proc(*p.get()),
        _proc_ptr(std::move(p))
{

    item = std::make_unique<ResonanceItem> (); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
                                        {item->getImageComponent()->init(_open_gl); });
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kResonance);

}

std::shared_ptr<SynthSection> ResonancePreparation::getPrepPopup()
{
    if(popup_view)
        popup_view->destroyOpenGlComponents(_open_gl);
    popup_view = std::make_shared<ResonancePopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void ResonancePreparation::resized()
{
    PreparationSection::resized();
    //slider->setBounds(0, 0, 100, 100);
}

ResonancePreparation::~ResonancePreparation()
{
}

juce::AudioProcessor* ResonancePreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> ResonancePreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}

void ResonancePreparation::ResonancePopup::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {


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
/*                     NESTED CLASS: ResonancePopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
ResonancePreparation::ResonancePopup::ResonancePopup(ResonanceProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(_open_gl)
{
    auto& _params = proc.getState().params;
}

ResonancePreparation::ResonancePopup::~ResonancePopup()
{
    //TODO
    //constructor sends destroy message to opengl thread
}
void ResonancePreparation::ResonancePopup::redoImage()
{
    int mult = getPixelMultiple();
    int image_width = getWidth() * mult;
    int image_height = getHeight() * mult;
}
void ResonancePreparation::ResonancePopup::initOpenGlComponents(OpenGlWrapper &open_gl)
{


}

// DISPLAY SLIDERS ON-SCREEN
void ResonancePreparation::ResonancePopup::resized()
{


}
