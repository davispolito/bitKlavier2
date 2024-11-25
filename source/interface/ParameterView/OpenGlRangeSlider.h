//
// Created by Dan Trueman on 11/25/24.
//

#ifndef BITKLAVIER2_OPENGLRANGESLIDER_H
#define BITKLAVIER2_OPENGLRANGESLIDER_H

#include "BKSliders.h"
#include "TransposeParams.h" // redo this for Range slider
#include "open_gl_component.h"
#include "juce_data_structures/juce_data_structures.h"

/************************************************************************************/
/*                              CLASS: OpenGlRangeSlider                            */
/************************************************************************************/

class OpenGlRangeSlider : public OpenGlAutoImageComponent<BKRangeSlider>
{
public:
    OpenGlRangeSlider(TransposeParams *params, chowdsp::ParameterListeners& listeners) :
                          OpenGlAutoImageComponent<BKRangeSlider>(
                              "Range",  // slider name
                              0,        // min
                              127,      // max
                              0,        // default min
                              127,      // default max
                              1)        // increment
    {
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);

//        int i = 0;
//        for(auto slider : dataSliders) {
//            //chowdsp::SemitonesParameter::Ptr param = params->addNewSliderParam();
//            auto ptr = std::make_unique<chowdsp::SliderAttachment>(*(*params->getFloatParams())[i++].get(),listeners,*slider,nullptr);
//            attachmentVec.emplace_back(std::move(ptr));
        //}

    }

    OpenGlRangeSlider() :
                          OpenGlAutoImageComponent<BKRangeSlider>(
                              "Range",  // slider name
                              0,        // min
                              127,      // max
                              0,        // default min
                              127,      // default max
                              1)        // increment
    {
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);
    }

    virtual void resized() override
    {
        OpenGlAutoImageComponent<BKRangeSlider>::resized();
        // if (isShowing())
        redoImage();
    }
    virtual void mouseDrag(const juce::MouseEvent &e) override
    {
        OpenGlAutoImageComponent<BKRangeSlider>::mouseDrag(e);
        redoImage();
    }
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKRangeSlider>::textEditorReturnKeyPressed(textEditor);
        redoImage();
    }
    void textEditorFocusLost(juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKRangeSlider>::textEditorFocusLost(textEditor);
        redoImage();
    }
    void textEditorEscapeKeyPressed (juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKRangeSlider>::textEditorEscapeKeyPressed(textEditor);
        redoImage();
    }
    void textEditorTextChanged(juce::TextEditor& textEditor) override
    {
        OpenGlAutoImageComponent<BKRangeSlider>::textEditorTextChanged(textEditor);
        redoImage();
    }
    std::vector<std::unique_ptr<chowdsp::SliderAttachment>> attachmentVec;
};

#endif //BITKLAVIER2_OPENGLRANGESLIDER_H
