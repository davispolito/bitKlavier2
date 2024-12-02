//
// Created by Dan Trueman on 11/25/24.
//

#ifndef BITKLAVIER2_OPENGL_RANGESLIDER_H
#define BITKLAVIER2_OPENGL_RANGESLIDER_H

#include "OpenGL_BKSliders.h"

/************************************************************************************/
/*                           CLASS: OpenGL_RangeSlider                              */
/************************************************************************************/

class OpenGL_RangeSlider : public OpenGlAutoImageComponent<BKRangeSlider>
{
public:
    OpenGL_RangeSlider (RangeSliderParams *params, chowdsp::ParameterListeners& listeners) :
                          OpenGlAutoImageComponent<BKRangeSlider>(
                                "Range",
                                0,
                                128,
                                0,
                                128,
                                1)
    {
        BKRangeSlider::setName(params->getName());
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);

        auto ptrMin = std::make_unique<chowdsp::SliderAttachment>(*(*params->getFloatParams())[0].get(),listeners,minSlider,nullptr);
        auto ptrMax = std::make_unique<chowdsp::SliderAttachment>(*(*params->getFloatParams())[1].get(),listeners,maxSlider,nullptr);
        auto ptrDisplay = std::make_unique<chowdsp::SliderAttachment>(*(*params->getFloatParams())[2].get(),listeners, *displaySlider.get(),nullptr);

        attachmentVec.emplace_back(std::move(ptrMin));
        attachmentVec.emplace_back(std::move(ptrMax));
        attachmentVec.emplace_back(std::move(ptrDisplay));

    }

    OpenGL_RangeSlider() :
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

#endif //BITKLAVIER2_OPENGL_RANGESLIDER_H
