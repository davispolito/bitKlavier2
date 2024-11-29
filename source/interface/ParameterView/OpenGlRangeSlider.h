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
    OpenGlRangeSlider(RangeSliderParams *params, chowdsp::ParameterListeners& listeners) :
                          OpenGlAutoImageComponent<BKRangeSlider>(
                              "Range",  // slider name
                                    0,     // min
                                128,      // max
                                0,        // default min
                                128,      // default max
                                1)        // increment
    {
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);

        auto ptrMin = std::make_unique<chowdsp::SliderAttachment>(*(*params->getFloatParams())[0],listeners,minSlider,nullptr);
        auto ptrMax = std::make_unique<chowdsp::SliderAttachment>(*(*params->getFloatParams())[1],listeners,maxSlider,nullptr);

        attachmentVec.emplace_back(std::move(ptrMin));
        attachmentVec.emplace_back(std::move(ptrMax));
    }

    OpenGlRangeSlider() :
                          OpenGlAutoImageComponent<BKRangeSlider>(
                              "Range",
                              0,
                              127,
                              0,
                              127,
                              1)
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
