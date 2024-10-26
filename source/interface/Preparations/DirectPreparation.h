/************************************************************************************/
/*                 Created by Davis Polito and Joshua Warner                        */
/************************************************************************************/

#ifndef BITKLAVIER2_DIRECTPREPARATION_H
#define BITKLAVIER2_DIRECTPREPARATION_H

#include "DirectProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"
#include "ParameterView/ParametersView.h"
#include "FullInterface.h"
#include "BKSliders.h"
/************************************************************************************/
/*                              CLASS: OpenGlSlider                                 */
/************************************************************************************/

class OpenGlTranspositionSlider : public OpenGlAutoImageComponent<BKStackedSlider>
{
public:
    OpenGlTranspositionSlider(const juce::ValueTree& v) :
               OpenGlAutoImageComponent<BKStackedSlider>(
                   "Transpositions",   // slider name
                   -12,                // min
                   12,                 // max
                   -12,                // default min
                   12,                 // default max
                   0,                  // default val
                   0.01)               // increment
    //-12, 12, -12, 12, 0, 0.01
    {             // increment
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);
    }

    OpenGlTranspositionSlider() :
                                                           OpenGlAutoImageComponent<BKStackedSlider>(
                                                               "Transpositions",    // slider name
                                                               -12,                 // min
                                                               12,                  // max
                                                               -12,                 // default min
                                                               12,                  // default max
                                                               0,                   // default val
                                                               0.01)                // increment
    {             // increment
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);
    }

    virtual void resized() override
    {
        OpenGlAutoImageComponent<BKStackedSlider>::resized();
        if (isShowing())
            redoImage();
    }
};

/*
 * class OpenGlMidiSelector : public OpenGlAutoImageComponent<MidiInputSelectorComponentListBox> {
public:
    OpenGlMidiSelector(const juce::ValueTree& v) :
            OpenGlAutoImageComponent<MidiInputSelectorComponentListBox>(v) {
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);
    }

virtual void resized() override {
    OpenGlAutoImageComponent<MidiInputSelectorComponentListBox>::resized();
    if (isShowing())
        redoImage();
}

private:
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlMidiSelector)
};
 */

/************************************************************************************/
/*             CLASS: DirectPreparation, inherits from PreparationSection           */
/************************************************************************************/

class DirectPreparation : public PreparationSection
                            {
public:

    // Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
    // a value tree, and a reference to an OpenGlWrapper object
    DirectPreparation(std::unique_ptr<DirectProcessor> proc, juce::ValueTree v, OpenGlWrapper& um);

    // Destructor method
    ~DirectPreparation();

    // Static function that returns a pointer to a DirectPreparation object
    static PreparationSection* createDirectSection(juce::ValueTree v, SynthGuiInterface* interface) {

        return new DirectPreparation(std::make_unique<DirectProcessor>(v), v, interface->getGui()->open_gl_);
    }

//    void portClicked(const juce::Point<int>& pos
//                     ) override
//    {
//        for(auto listener : listeners_)
//        {
//            listener->portClicked(pos, node);
//        }
//    }
    // Public function definitions for the DirectPreparation class, which override functions
    // in the PreparationSection base class
    void addSoundSet(juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* s) override
    {
        proc.addSoundSet(s);
    }

    void addSoundSet(
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* s,
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* h,
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* r,
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* p) override
    {
        proc.addSoundSet(s, h, r, p);
    }

    std::shared_ptr<SynthSection> getPrepPopup() override;
    void resized() override;
    void paintBackground(juce::Graphics &g);
    juce::AudioProcessor* getProcessor() override;
    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;
private:

    // Private member variable for the DirectPreparation class: proc is a pointer to a
    // DirectProcessor Object
    DirectProcessor & proc;
    std::unique_ptr<DirectProcessor> _proc_ptr;

    /************************************************************************************/
    /*             NESTED CLASS: DirectPopup, inherits from PreparationPopup            */
    /************************************************************************************/

    class DirectPopup : public PreparationPopup {
    public:

        // Constructor method that takes two arguments: a smart pointer to a DirectProcessor,
        // and a reference to an OpenGlWrapper
        DirectPopup (DirectProcessor& proc, OpenGlWrapper& open_gl);

        // Public function definitions for the class, which override the base class methods for
        // initializing, rendering, resizing, and painting OpenGl components
        void initOpenGlComponents(OpenGlWrapper &open_gl) override;
        void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) override;
        void resized() override;
        void paintBackground(juce::Graphics& g) override
        {
             SynthSection::paintContainer(g);
             paintHeadingText(g);
             paintBorder(g);
             paintKnobShadows(g);
             paintChildrenBackgrounds(g);
        }

        int getViewPosition() {
            int view_height = getHeight();
            return view_height; //std::max(0, std::min<int>(selections_.size() * getRowHeight() - view_height, view_position_));
        }

        ~DirectPopup();


    private:

        // Private function definitions and member variables for the DirectPopup class
        void redoImage();
        DirectParams* params = nullptr;
        DirectProcessor& proc;
        std::unique_ptr<OpenGlTranspositionSlider> transpositionSlider;


        bitklavier::ParametersView view;
//        // Parameter sliders for the Direct Popup
//        std::unique_ptr<SynthSlider> gainSlider;
//        std::unique_ptr<SynthSlider> hammerSlider;
//        std::unique_ptr<SynthSlider> velocitySlider;
//        std::unique_ptr<SynthSlider> resonanceSlider;
//        std::unique_ptr<SynthSlider> attackSlider;
//        std::unique_ptr<SynthSlider> decaySlider;
//        std::unique_ptr<SynthSlider> sustainSlider;
//        std::unique_ptr<SynthSlider> releaseSlider;
////        std::unique_ptr<SynthSlider> transpositionsSlider;
////        std::unique_ptr<SynthSlider> blendronicSendSlider;


        OpenGlImage sliderShadows;

    };
};



#endif // BITKLAVIER2_DIRECTPREPARATION_H