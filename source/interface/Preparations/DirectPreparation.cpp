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
    //port = std::make_unique<BKPort> ();
    _open_gl.initOpenGlComp.try_enqueue([this]
                                                  {item->getImageComponent()->init(_open_gl);
                                                      });

    addAndMakeVisible (item.get());
    setSkinOverride (Skin::kDirect);
    juce::MemoryBlock data;
//    proc.getStateInformation(data);
//    auto xml = juce::parseXML(data.toString());
////auto xml = juce::AudioProcessor::getXmlF(data.getData(), (int)data.getSize());
//    if (!state.getChild(0).isValid())
//        state.addChild(juce::ValueTree::fromXml(*xml),0,nullptr);
//    DBG(state.toXmlString());
    DBG("add processor params");
}

std::shared_ptr<SynthSection> DirectPreparation::getPrepPopup()
{
    DBG("prep popup");
    if(popup_view) {
//        popup_view->destroyOpenGlComponents(_open_gl);
//        popup_view->reset();
        return popup_view;
    }
    popup_view = std::make_shared<DirectPopup>(proc, _open_gl);
    popup_view->initOpenGlComponents(_open_gl);
    return popup_view;
}


void DirectPreparation::resized()
{

    PreparationSection::resized();
//    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
//    int item_padding_y = kItemPaddingY * size_ratio_;
//    int item_height = getHeight() - 2 * item_padding_y;
//    int item_padding_x = kItemPaddingX * size_ratio_;
//    int item_width = getWidth() - 2 * item_padding_x;
//    auto newBounds = getBoundsInParent();
////    constrainer.checkBounds (newBounds, getBoundsInParent(),
////        getParentComponent()->getLocalBounds(),
////        false, false, false, false);
//    DBG("item_padding x " + juce::String(item_padding_x) + "item_padding y" + juce::String(item_padding_y));
//
//    auto portDim = proportionOfWidth(0.1f);
//
//    //port->setBounds(getRight(), getY() + getHeight()/2, 100, 100);
//    SynthSection::resized();
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
            //DBG(juce::String(gl));
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

void DirectPreparation::paintBackground(juce::Graphics &g)  {
    for (auto * port: objects)
        port->redoImage();
    PreparationSection::paintBackground(g);
}
/*************************************************************************************************/
/*                     NESTED CLASS: DirectPopup, inherits from PreparationPopup                 */
/*************************************************************************************************/
DirectPreparation::DirectPopup::DirectPopup(DirectProcessor& _proc, OpenGlWrapper &open_gl):  proc(_proc), PreparationPopup(_open_gl), view(proc.getState(), proc.getState().params, &open_gl)
{

    auto& _params = proc.getState().params;
    setSkinOverride (Skin::kDirect);
    Skin default_skin;
    view.setSkinValues(default_skin, false);
    addSubSection(&view);
//    view.init_();

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
    //juce::Image background_image(juce::Image::ARGB,image_width, image_height, true);
    //juce::Graphics g (background_image);

    //paintKnobShadows(g);
    //sliderShadows.setOwnImage(background_image);

}
void DirectPreparation::DirectPopup::initOpenGlComponents(OpenGlWrapper &open_gl) {

    view.initOpenGlComponents(open_gl);
}

// DISPLAY SLIDERS ON-SCREEN
void DirectPreparation::DirectPopup::resized() {

    // DYNAMICALLY POSITIONS SLIDER WITHIN THE DIRECT POPUP

    // Retrieves the size specifications of the current popup
    juce::Rectangle<int> bounds = getLocalBounds();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int section_height = getHeight();
    int section_width = getWidth();
    view.setBounds(getLocalBounds());
//    // Relatively defines slider size
//    int sliderWidth = (section_width - (4 * widget_margin))  / 3;
//    int sliderHeight = (section_height - (4 * widget_margin))  / 3;
//
//    // Relatively defines column locations
//    int column1 = bounds.getX() + widget_margin;
//    int column1andAHalf = bounds.getX() + (sliderWidth / 2) + widget_margin;
//    int column2 = bounds.getX() + sliderWidth + (2 * widget_margin);
//    int column2andAHalf = bounds.getX() + (3 * sliderWidth / 2) + (2 * widget_margin);
//    int column3 = bounds.getX() + (2 * sliderWidth) + (3 * widget_margin);
//
//    // Relatively defines row locations
//    int row1 = bounds.getY() + widget_margin;
//    int row2 = bounds.getY() + sliderHeight + (2 * widget_margin);
//    int row3 = bounds.getY() + (2 * sliderHeight) + (3 * widget_margin);
//
//    // POSITIONS GAIN SLIDER
//    gainSlider->setBounds(column1, row1, sliderWidth, sliderHeight);
//
//    // POSITIONS HAMMER SLIDER
//    hammerSlider->setBounds(column2, row1, sliderWidth, sliderHeight);
//
//    // POSITIONS VELOCITY SLIDER
//    velocitySlider->setBounds(column3, row1, sliderWidth, sliderHeight);
//
//    // POSITIONS RESONANCE SLIDER
//    resonanceSlider->setBounds(column1, row2, sliderWidth, sliderHeight);
//
//    // POSITIONS ATTACK SLIDER
//    attackSlider->setBounds(column2, row2, sliderWidth, sliderHeight);
//
//    // POSITIONS DECAY SLIDER
//    decaySlider->setBounds(column3, row2, sliderWidth, sliderHeight);
//
//    // POSITIONS SUSTAIN SLIDER
//    sustainSlider->setBounds(column1andAHalf, row3, sliderWidth, sliderHeight);
//
//    // POSITIONS RELEASE SLIDER
//    releaseSlider->setBounds(column2andAHalf, row3, sliderWidth, sliderHeight);
//
////    // POSITIONS TRANSPOSITIONS SLIDER
////    transpositionsSlider->setBounds(column3, row3, sliderWidth, sliderHeight);
//
////    // POSITIONS BLENDRONIC SEND SLIDER
////    blendronicSendSlider->setBounds();
//
//    gainSlider->redoImage();
//    hammerSlider->redoImage();
//    velocitySlider->redoImage();
//    resonanceSlider->redoImage();
//    attackSlider->redoImage();
//    decaySlider->redoImage();
//    sustainSlider->redoImage();
//    releaseSlider->redoImage();

//    transpositionsSlider->redoImage();
//    blendronicSendSlider->redoImage();

    redoImage();
    SynthSection::resized();



//    placeKnobsInArea(juce::Rectangle<int>(knobs_area.getX(), 0, knobs_area.getWidth(), section_height),
//                     { drive_.get(), mix_.get() });
//
//    placeKnobsInArea(juce::Rectangle<int>(knobs_area.getX(), knob_y2, knobs_area.getWidth(), section_height),
//                     { filter_cutoff_.get(), filter_resonance_.get(), filter_blend_.get() });
//    setFilterActive(filter_order_->getValue() != 0.0f && isActive());
//
//    SynthSection::resized();



    // juce::Image image(juce::Image::ARGB, 1, 1, false);
    // juce::Graphics g(image);
    // paintKnobShadows(g);
    //gainSlider->redoImage();
    // juce::Image image(juce::Image::ARGB, 1, 1, false);
    // juce::Graphics g(image);
    // paintOpenGlChildrenBackgrounds(g);
    // paintKnobShadows(g);
}
