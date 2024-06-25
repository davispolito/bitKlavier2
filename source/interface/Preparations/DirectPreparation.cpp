/*********************************************************************************************/
/*                           Created by Davis Polito and Joshua Warner                       */
/*********************************************************************************************/

#include "DirectPreparation.h"
#include "BKitems/BKItem.h"

#include "synth_slider.h"

// Definition for the DirectPreparation constructor.  It takes three parameters: a pointer to
// a Direct Processor p, a ValueTree v, and a reference to an OpenGlWrapper object.  Initializes
// the base class members and private DirectPreparation member proc with an initialization list.
DirectPreparation::DirectPreparation (std::unique_ptr<DirectProcessor> p,
    juce::ValueTree v, OpenGlWrapper& um) :
                         PreparationSection(juce::String("direct"), v, um),
                         proc(*p.get()),
                         _proc_ptr(std::move(p))
{

    item = std::make_unique<DirectItem> (); // Initializes member variable `item` of PreparationSection class
    addOpenGlComponent (item->getImageComponent()); // Calls member function of SynthSection (parent class to PreparationSection)
    _open_gl.initOpenGlComp.try_enqueue([this]
        {item->getImageComponent()->init(_open_gl); });
    addAndMakeVisible (item.get());

    setSkinOverride (Skin::kDirect);

}

std::shared_ptr<SynthSection> DirectPreparation::getPrepPopup()
{
    if(popup_view)
        popup_view->destroyOpenGlComponents(_open_gl);
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

juce::AudioProcessor* DirectPreparation::getProcessor()
{
    return &proc;
}

std::unique_ptr<juce::AudioProcessor> DirectPreparation::getProcessorPtr()
{
    return std::move(_proc_ptr);
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

/*************************************************************************************************/
/*                     NESTED CLASS: DirectPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
DirectPreparation::DirectPopup::DirectPopup(DirectProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(_open_gl)
{

    auto& _params = proc.getState().params;

    /************************************** WHAT IS THIS? *****************************************/

    // slider = std::make_unique<SynthSlider>("sliderino", _params.gainParam ,proc->getState());
    // addSlider(slider.get());
    // addAndMakeVisible(slider.get());
    // addOpenGlComponent(slider->getImageComponent());
    // addOpenGlComponent(slider->getQuadComponent());
    // slider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    /****************************************   CHANGES    ******************************************/

    // GAIN PARAMETER SLIDER
    gainSlider = std::make_unique<SynthSlider>("gainSlider", _params.gainParam, proc.getState());
    addSlider(gainSlider.get());
    gainSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);


    // FREQ PARAMETER SLIDER
    sustainSlider = std::make_unique<SynthSlider>("sustainSlider", _params.sustainParam, proc.getState());
    SynthSection::addSlider(sustainSlider.get());
    sustainSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);


    // ORDER PARAMETER SLIDER
    attackSlider = std::make_unique<SynthSlider>("attackSlider", _params.attackParam, proc.getState());
    SynthSection::addSlider(attackSlider.get());
    attackSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);







    /*********************************************************************************************/
}

DirectPreparation::DirectPopup::~DirectPopup()
{
    //TODO
    //constructor sends destroy message to opengl thread
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

    // GAIN SLIDER
    open_gl.initOpenGlComp.try_enqueue([this,&open_gl]
                                    {gainSlider->getQuadComponent()->init(open_gl); });
    open_gl.initOpenGlComp.try_enqueue([this, &open_gl]
                                       {gainSlider->getImageComponent()->init(open_gl); });

    // ATTACK SLIDER
    open_gl.initOpenGlComp.try_enqueue([this,&open_gl]
        {attackSlider->getQuadComponent()->init(open_gl); });
    open_gl.initOpenGlComp.try_enqueue([this, &open_gl]
        {attackSlider->getImageComponent()->init(open_gl); });

    // SUSTAIN SLIDER
    open_gl.initOpenGlComp.try_enqueue([this,&open_gl]
        {sustainSlider->getQuadComponent()->init(open_gl); });
    open_gl.initOpenGlComp.try_enqueue([this, &open_gl]
        {sustainSlider->getImageComponent()->init(open_gl); });


}

// DISPLAY SLIDERS ON-SCREEN
void DirectPreparation::DirectPopup::resized() {

    int widget_margin = findValue(Skin::kWidgetMargin);
    int title_width = getTitleWidth();
    int section_height = getHeight();

    Rectangle<int> bounds = getLocalBounds().withLeft(title_width);
    Rectangle<int> knobs_area = getDividedAreaBuffered(bounds, 2, 1, widget_margin);
    Rectangle<int> settings_area = getDividedAreaUnbuffered(bounds, 4, 0, widget_margin);

    int widget_x = settings_area.getRight() + widget_margin;
    int widget_width = knobs_area.getX() - widget_x;

    int knob_y2 = section_height - widget_margin;
    // gainSlider->setBounds(settings_area.getX(), widget_margin, settings_area.getWidth(), section_height - 2 * widget_margin);
    // attackSlider->setBounds(settings_area.getX(), knob_y2 + widget_margin,
    //                         settings_area.getWidth(), section_height - 2 * widget_margin);
    gainSlider->setBounds(settings_area.getX(), widget_margin, 100, 100);
    attackSlider->setBounds(2 * settings_area.getRight(), widget_margin, 100, 100);
    //attackSlider->setBounds(settings_area.getX(), knob_y2 + widget_margin, // NOT VISIBLE
    //                        100, 100);

    int sustain_slider_height = (getHeight() - 3 * widget_margin) / 2;
    // sustainSlider->setBounds(widget_x, widget_margin, widget_width, sustain_slider_height);
    sustainSlider->setBounds(widget_x, widget_margin, 100, 100);

    sustainSlider->redoImage();
    gainSlider->redoImage();
    attackSlider->redoImage();
    redoImage();

    SynthSection::resized();


//
//    placeKnobsInArea(Rectangle<int>(knobs_area.getX(), 0, knobs_area.getWidth(), section_height),
//                     { drive_.get(), mix_.get() });
//
//    placeKnobsInArea(Rectangle<int>(knobs_area.getX(), knob_y2, knobs_area.getWidth(), section_height),
//                     { filter_cutoff_.get(), filter_resonance_.get(), filter_blend_.get() });
//    setFilterActive(filter_order_->getValue() != 0.0f && isActive());
//
//    SynthSection::resized();



/*******************************************************************************************/
/*                                      CODE THAT WORKS                                    */
/*******************************************************************************************/

//    int widget_margin = findValue(Skin::kWidgetMargin);
//    Rectangle<int> bounds = getLocalBounds();
//
//    // GAIN SLIDER
//    gainSlider->setBounds(100, 100,80,80);
//
//    // FREQ SLIDER
//    attackSlider->setBounds(170, 100,80,80);
//
//    // ORDER SLIDER
//    sustainSlider->setBounds(220, 100,80,80);
//
//    sustainSlider->redoImage();
//    gainSlider->redoImage();
//    attackSlider->redoImage();
//    redoImage();
/*******************************************************************************************/

    // Image image(Image::ARGB, 1, 1, false);
    // Graphics g(image);
    // paintKnobShadows(g);
    //gainSlider->redoImage();
    // Image image(Image::ARGB, 1, 1, false);
    // Graphics g(image);
    // paintOpenGlChildrenBackgrounds(g);
    // paintKnobShadows(g);
}
