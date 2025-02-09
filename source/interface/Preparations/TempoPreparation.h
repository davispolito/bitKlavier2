//
// Created by Joshua Warner on 6/27/24.
//

#ifndef BITKLAVIER2_TEMPOPREPARATION_H
#define BITKLAVIER2_TEMPOPREPARATION_H


/************************************************************************************/
/*                 Created by Davis Polito and Joshua Warner                        */
/************************************************************************************/

#include "TempoProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"
#include "FullInterface.h"
/************************************************************************************/
/*                              CLASS: OpenGlSlider                                 */
/************************************************************************************/

class OpenGlSlider;

/************************************************************************************/
/*             CLASS: TempoPreparation, inherits from PreparationSection           */
/************************************************************************************/

class TempoPreparation : public PreparationSection {
public:

    // Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
    // a value tree, and a reference to an OpenGlWrapper object
    TempoPreparation(std::unique_ptr<TempoProcessor> proc, juce::ValueTree v, OpenGlWrapper& um);

    // Destructor method
    ~TempoPreparation();

    // Static function that returns a pointer to a TempoPreparation object
    static PreparationSection* createTempoSection(juce::ValueTree v, SynthGuiInterface* interface) {

        return new TempoPreparation(std::make_unique<TempoProcessor>(), v, interface->getGui()->open_gl_);
    }

    // Public function definitions for the TempoPreparation class, which override functions
    // in the PreparationSection base class
    std::shared_ptr<SynthSection> getPrepPopup() override;
    void resized() override;

    juce::AudioProcessor* getProcessor() override;
    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;
private:

    // Private member variable for the TempoPreparation class: proc is a pointer to a
    // TempoProcessor Object
    TempoProcessor & proc;
    std::unique_ptr<TempoProcessor> _proc_ptr;

    /************************************************************************************/
    /*             NESTED CLASS: TempoPopup, inherits from PreparationPopup            */
    /************************************************************************************/

    class TempoPopup : public PreparationPopup {
    public:

        // Constructor method that takes two arguments: a smart pointer to a TempoProcessor,
        // and a reference to an OpenGlWrapper
        TempoPopup (TempoProcessor& proc, OpenGlWrapper& open_gl);

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

        ~TempoPopup();


    private:

        // Private function definitions and member variables for the TempoPopup class
        void redoImage();
        TempoParams* params = nullptr;
        TempoProcessor& proc;

    };
};

#endif //BITKLAVIER2_TEMPOPREPARATION_H
