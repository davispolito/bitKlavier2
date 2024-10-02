//
// Created by Joshua Warner on 6/27/24.
//

#ifndef BITKLAVIER2_NOSTALGICPREPARATION_H
#define BITKLAVIER2_NOSTALGICPREPARATION_H

/************************************************************************************/
/*                 Created by Davis Polito and Joshua Warner                        */
/************************************************************************************/

#include "NostalgicProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"
#include "FullInterface.h"
/************************************************************************************/
/*                              CLASS: OpenGlSlider                                 */
/************************************************************************************/

class OpenGlSlider;

/************************************************************************************/
/*             CLASS: NostalgicPreparation, inherits from PreparationSection           */
/************************************************************************************/

class NostalgicPreparation : public PreparationSection {
public:

    // Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
    // a value tree, and a reference to an OpenGlWrapper object
    NostalgicPreparation(std::unique_ptr<NostalgicProcessor> proc, juce::ValueTree v, OpenGlWrapper& um);

    // Destructor method
    ~NostalgicPreparation();

    // Static function that returns a pointer to a NostalgicPreparation object
    static PreparationSection* createNostalgicSection(ValueTree v, SynthGuiInterface* interface) {

        return new NostalgicPreparation(std::make_unique<NostalgicProcessor>(), v, interface->getGui()->open_gl_);
    }    // Public function definitions for the NostalgicPreparation class, which override functions
    // in the PreparationSection base class
    std::shared_ptr<SynthSection> getPrepPopup() override;
    void resized() override;

    juce::AudioProcessor* getProcessor() override;
    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;

private:

    // Private member variable for the NostalgicPreparation class: proc is a pointer to a
    // NostalgicProcessor Object
    NostalgicProcessor & proc;
    std::unique_ptr<NostalgicProcessor> _proc_ptr;

    /************************************************************************************/
    /*             NESTED CLASS: NostalgicPopup, inherits from PreparationPopup            */
    /************************************************************************************/

    class NostalgicPopup : public PreparationPopup {
    public:

        // Constructor method that takes two arguments: a smart pointer to a NostalgicProcessor,
        // and a reference to an OpenGlWrapper
        NostalgicPopup (NostalgicProcessor& proc, OpenGlWrapper& open_gl);

        // Public function definitions for the class, which override the base class methods for
        // initializing, rendering, resizing, and painting OpenGl components
        void initOpenGlComponents(OpenGlWrapper &open_gl) override;
        void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) override;
        void resized() override;
        void paintBackground(Graphics& g) override
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

        ~NostalgicPopup();


    private:

        // Private function definitions and member variables for the NostalgicPopup class
        void redoImage();
        NostalgicParams* params = nullptr;
        NostalgicProcessor& proc;

    };
};


#endif //BITKLAVIER2_NostalgicPREPARATION_H
