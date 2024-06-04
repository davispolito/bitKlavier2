//
// Created by Davis Polito on 2/13/24.
//

#include "DirectPreparation.h"
#include "BKitems/BKItem.h"
#include "synth_slider.h"
DirectPreparation::DirectPreparation (std::shared_ptr<PolygonalOscProcessor>  p,juce::ValueTree v, OpenGlWrapper& um) : PreparationSection("direct", v, um), proc(p)
{
    item = std::make_unique<DirectItem> ();
    addOpenGlComponent (item->getImageComponent());
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kDirect);

    //slider = std::make_unique<SynthSlider>("sliderino", p.getState().params.gainParam, p.getState());

    //addSlider(slider.get());
    _open_gl.init_comp.push_back(item->getImageComponent());
    _open_gl.initOpenGlComp.enqueue([this]
                                {item->getImageComponent()->init(_open_gl); });

//    _open_gl.initOpenGlComp.enqueue([this]
//                                    {slider->getImageComponent()->init(_open_gl); });
//    _open_gl.initOpenGlComp.enqueue([this]
//                                    {slider->getTextEditorComponent()->init(_open_gl); });
//    _open_gl.initOpenGlComp.enqueue([this]
//                                    {slider->getQuadComponent()->init(_open_gl); });
//    _open_gl.init_comp.push_back(slider->getImageComponent());
//    _open_gl.init_comp.push_back(slider->getQuadComponent());
//    _open_gl.init_comp.push_back(slider->getTextEditorComponent());

}






std::shared_ptr<SynthSection> DirectPreparation::getPrepPopup()
{
    popup_view = std::make_shared<DirectPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void DirectPreparation::resized()
{
    PreparationSection::resized();
    //slider->setBounds(0, 0, 100, 100);
}

DirectPreparation::~DirectPreparation()
{
}

std::shared_ptr<juce::AudioProcessor> DirectPreparation::getProcessor()
{
    return proc;
}

void DirectPreparation::DirectPopup::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {


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
DirectPreparation::DirectPopup::DirectPopup(std::shared_ptr<PolygonalOscProcessor> _proc, OpenGlWrapper &open_gl): proc(_proc), PreparationPopup(_open_gl)
{

    auto& _params = proc->getState().params;
    slider = std::make_unique<SynthSlider>("sliderino", _params.gainParam ,proc->getState());
    addSlider(slider.get());
    //addAndMakeVisible(slider.get());
//    addOpenGlComponent(slider->getImageComponent());
//    addOpenGlComponent(slider->getQuadComponent());

    slider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
}
void DirectPreparation::DirectPopup::redoImage()
{
    int mult = getPixelMultiple();
    int image_width = getWidth() * mult;
    int image_height = getHeight() * mult;
    //Image background_image(Image::ARGB,image_width, image_height, true);
    //Graphics g (background_image);

    //paintKnobShadows(g);
    //sliderShadows.setOwnImage(background_image);

}
void DirectPreparation::DirectPopup::initOpenGlComponents(OpenGlWrapper &open_gl) {

//    open_gl.initOpenGlComp.try_enqueue([this,&open_gl]
//                                    {slider->getTextEditorComponent()->init(open_gl); });
    open_gl.initOpenGlComp.try_enqueue([this,&open_gl]
                                    {slider->getQuadComponent()->init(open_gl); });
    open_gl.initOpenGlComp.try_enqueue([this, &open_gl]
                                       {slider->getImageComponent()->init(open_gl); });

}

void DirectPreparation::DirectPopup::resized() {
    Rectangle<int> bounds = getLocalBounds();



    slider->setBounds(100, 100,80,80);
//    Image image(Image::ARGB, 1, 1, false);
//    Graphics g(image);
//    paintKnobShadows(g);
    slider->redoImage();
//    Image image(Image::ARGB, 1, 1, false);
//    Graphics g(image);
//    paintOpenGlChildrenBackgrounds(g);
//    paintKnobShadows(g);
    redoImage();
}
