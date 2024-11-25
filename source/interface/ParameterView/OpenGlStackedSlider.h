//
// Created by Davis Polito on 11/13/24.
//

#ifndef BITKLAVIER2_OPENGLSTACKEDSLIDER_H
#define BITKLAVIER2_OPENGLSTACKEDSLIDER_H
#include "BKSliders.h"
#include "TransposeParams.h"
#include "open_gl_component.h"
#include "juce_data_structures/juce_data_structures.h"

/************************************************************************************/
/*                         CLASS: OpenGlStackedSlider                         */
/************************************************************************************/

class OpenGlStackedSlider : public OpenGlAutoImageComponent<BKStackedSlider>
{
public:
    OpenGlStackedSlider (TransposeParams *params, chowdsp::ParameterListeners& listeners) :
            OpenGlAutoImageComponent<BKStackedSlider>(
                    "StackedSlider",    // slider name
                    -12,                // min
                    12,                 // max
                    -12,                // default min
                    12,                 // default max
                    0,                  // default val
                    0.01)               // increment
    {
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

    OpenGlStackedSlider() :
            OpenGlAutoImageComponent<BKStackedSlider>(
                    "Transpositions",    // slider name
                    -12,                 // min
                    12,                  // max
                    -12,                 // default min
                    12,                  // default max
                    0,                   // default val
                    0.01)                // increment
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

#endif //BITKLAVIER2_OPENGLSTACKEDSLIDER_H
