//
// Created by Davis Polito on 11/13/24.
//

#ifndef BITKLAVIER2_OPENGL_STACKEDSLIDER_H
#define BITKLAVIER2_OPENGL_STACKEDSLIDER_H

#include "OpenGL_BKSliders.h"

/************************************************************************************/
/*                         CLASS: OpenGL_StackedSlider                         */
/************************************************************************************/

class OpenGL_StackedSlider : public OpenGlAutoImageComponent<BKStackedSlider>
{
public:
    OpenGL_StackedSlider (TransposeParams *params, chowdsp::ParameterListeners& listeners) :
                            OpenGlAutoImageComponent<BKStackedSlider>(
                                    "StackedSlider",
                                    -12,
                                    12,
                                    -12,
                                    12,
                                    0,
                                    0.01)
    {
        BKStackedSlider::setName(params->getName());
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);
        int i = 0;
        for(auto slider : dataSliders) {
            //chowdsp::SemitonesParameter::Ptr param = params->addNewSliderParam();
            auto ptr = std::make_unique<chowdsp::SliderAttachment>(*(*params->getFloatParams())[i++].get(),listeners,*slider,nullptr);
            attachmentVec.emplace_back(std::move(ptr));
        }
    }

    OpenGL_StackedSlider() :
            OpenGlAutoImageComponent<BKStackedSlider>(
                    "Transpositions",
                    -12,
                    12,
                    -12,
                    12,
                    0,
                    0.01)
    {
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);
    }

    virtual void resized() override
    {
        OpenGlAutoImageComponent<BKStackedSlider>::resized();
        // if (isShowing())
        redoImage();
    }
    virtual void mouseDrag(const juce::MouseEvent &e) override
    {
        OpenGlAutoImageComponent<BKStackedSlider>::mouseDrag(e);
        redoImage();
    }
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKStackedSlider>::textEditorReturnKeyPressed(textEditor);
        redoImage();
    }
    void textEditorFocusLost(juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKStackedSlider>::textEditorFocusLost(textEditor);
        redoImage();
    }
    void textEditorEscapeKeyPressed (juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKStackedSlider>::textEditorEscapeKeyPressed(textEditor);
        redoImage();
    }
    void textEditorTextChanged(juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKStackedSlider>::textEditorTextChanged(textEditor);
        redoImage();
    }

    std::vector<std::unique_ptr<chowdsp::SliderAttachment>> attachmentVec;
};



#endif //BITKLAVIER2_OPENGL_STACKEDSLIDER_H
