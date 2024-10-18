//
// Created by Joshua Warner on 6/27/24.
//

#ifndef BITKLAVIER2_SYNCHRONICPREPARATION_H
#define BITKLAVIER2_SYNCHRONICPREPARATION_H


/************************************************************************************/
/*                 Created by Davis Polito and Joshua Warner                        */
/************************************************************************************/

#include "SynchronicProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"
#include "FullInterface.h"
/************************************************************************************/
/*                              CLASS: OpenGlSlider                                 */
/************************************************************************************/

class OpenGlSlider;

/************************************************************************************/
/*             CLASS: SynchronicPreparation, inherits from PreparationSection           */
/************************************************************************************/

class SynchronicPreparation : public PreparationSection {
public:

    // Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
    // a value tree, and a reference to an OpenGlWrapper object
    SynchronicPreparation(std::unique_ptr<SynchronicProcessor> proc, juce::ValueTree v, OpenGlWrapper& um);

    // Destructor method
    ~SynchronicPreparation();

    // Static function that returns a pointer to a SynchronicPreparation object
    static PreparationSection* createSynchronicSection(juce::ValueTree v, SynthGuiInterface* interface) {

        return new SynchronicPreparation(std::make_unique<SynchronicProcessor>(), v, interface->getGui()->open_gl_);
    }

    // Public function definitions for the SynchronicPreparation class, which override functions
    // in the PreparationSection base class
    std::shared_ptr<SynthSection> getPrepPopup() override;
    void resized() override;

    juce::AudioProcessor* getProcessor() override;
    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;
private:

    // Private member variable for the SynchronicPreparation class: proc is a pointer to a
    // SynchronicProcessor Object
    SynchronicProcessor & proc;
    std::unique_ptr<SynchronicProcessor> _proc_ptr;

    /************************************************************************************/
    /*             NESTED CLASS: SynchronicPopup, inherits from PreparationPopup            */
    /************************************************************************************/

    class SynchronicPopup : public PreparationPopup {
    public:

        // Constructor method that takes two arguments: a smart pointer to a SynchronicProcessor,
        // and a reference to an OpenGlWrapper
        SynchronicPopup (SynchronicProcessor& proc, OpenGlWrapper& open_gl);

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

        ~SynchronicPopup();


    private:

        // Private function definitions and member variables for the SynchronicPopup class
        void redoImage();
        SynchronicParams* params = nullptr;
        SynchronicProcessor& proc;

    };
};


#endif //BITKLAVIER2_SYNCHRONICPREPARATION_H
