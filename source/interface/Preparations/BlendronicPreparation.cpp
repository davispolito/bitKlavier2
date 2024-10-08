/*********************************************************************************************/
/*                           Created by Davis Polito and Joshua Warner                       */
/*********************************************************************************************/

#include "BlendronicPreparation.h"
#include "BKitems/BKItem.h"

#include "synth_slider.h"

// Definition for the BlendronicPreparation constructor.  It takes three parameters: a pointer to
// a Blendronic Processor p, a ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private BlendronicPreparation member proc with an initialization list.
BlendronicPreparation::BlendronicPreparation (std::unique_ptr<BlendronicProcessor> p,
                                            juce::ValueTree v, OpenGlWrapper& um) :
        PreparationSection(juce::String("blendronic"), v, um,p.get()),
        proc(*p.get()),
        _proc_ptr(std::move(p))
{

    item = std::make_unique<BlendronicItem> (); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
                                        {item->getImageComponent()->init(_open_gl); });
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kBlendronic);

}

std::shared_ptr<SynthSection> BlendronicPreparation::getPrepPopup()
{
    if(popup_view)
        popup_view->destroyOpenGlComponents(_open_gl);
    popup_view = std::make_shared<BlendronicPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void BlendronicPreparation::resized()
{
    PreparationSection::resized();
    //slider->setBounds(0, 0, 100, 100);
}

BlendronicPreparation::~BlendronicPreparation()
{
}

juce::AudioProcessor* BlendronicPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> BlendronicPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
}

void BlendronicPreparation::BlendronicPopup::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {


    for (auto& sub_section : sub_sections_) {
        if (sub_section->isVisible() && !sub_section->isAlwaysOnTop())
            sub_section->renderOpenGlComponents(open_gl, animate);
    }

    for (auto& open_gl_component : open_gl_components_) {
        if (open_gl_component->isVisible() && !open_gl_component->isAlwaysOnTop()) {
            open_gl_component->render(open_gl, animate);
            GLenum gl =  juce::gl::glGetError();
            //DBG(String(gl));
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
/*                     NESTED CLASS: BlendronicPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
BlendronicPreparation::BlendronicPopup::BlendronicPopup(BlendronicProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(_open_gl)
{
    auto& _params = proc.getState().params;
}

BlendronicPreparation::BlendronicPopup::~BlendronicPopup()
{
    //TODO
    //constructor sends destroy message to opengl thread
}

void BlendronicPreparation::BlendronicPopup::redoImage()
{
    int mult = getPixelMultiple();
    int image_width = getWidth() * mult;
    int image_height = getHeight() * mult;
}

void BlendronicPreparation::BlendronicPopup::initOpenGlComponents(OpenGlWrapper &open_gl)
{


}

// DISPLAY SLIDERS ON-SCREEN
void BlendronicPreparation::BlendronicPopup::resized()
{


}
