//
// Created by Dan Trueman on 11/25/24.
//

#ifndef BITKLAVIER2_OPENGLRANGESLIDER_H
#define BITKLAVIER2_OPENGLRANGESLIDER_H

#include "OpenGlBKSliders.h"

/************************************************************************************/
/*                              CLASS: OpenGlRangeSlider                            */
/************************************************************************************/

class OpenGlRangeSlider : public OpenGlAutoImageComponent<BKRangeSlider>
{
public:
    OpenGlRangeSlider(chowdsp::FloatParameter *paramsMin, chowdsp::FloatParameter *paramsMax, chowdsp::ParameterListeners& listeners) :
                          OpenGlAutoImageComponent<BKRangeSlider>(
                              "Range",  // slider name
                             paramsMin->getNormalisableRange().start,     // min
                             paramsMax->getNormalisableRange().end,      // max
                                    0,        // default min
                                127,      // default max
                                1)        // increment
    {
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);



//        minSlider.setMinAndMaxValues(paramsMin->getNormalisableRange().start, paramsMin->getNormalisableRange().end);
//        maxSlider.setMinAndMaxValues(paramsMax->getNormalisableRange().start, paramsMax->getNormalisableRange().end);

//        setMinValue(paramsMin->getNormalisableRange().start, juce::dontSendNotification);
//        setMaxValue(paramsMax->getNormalisableRange().end, juce::dontSendNotification);

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
