//
// Created by Joshua Warner on 6/27/24.
//

#ifndef BITKLAVIER2_BlendronicPREPARATION_H
#define BITKLAVIER2_BlendronicPREPARATION_H

/************************************************************************************/
/*                 Created by Davis Polito and Joshua Warner                        */
/************************************************************************************/

#include "BlendronicProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"
#include "FullInterface.h"
/************************************************************************************/
/*                              CLASS: OpenGlSlider                                 */
/************************************************************************************/

class OpenGlSlider;

/************************************************************************************/
/*             CLASS: BlendronicPreparation, inherits from PreparationSection           */
/************************************************************************************/

class BlendronicPreparation : public PreparationSection {
public:

    // Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
    // a value tree, and a reference to an OpenGlWrapper object
    BlendronicPreparation(std::unique_ptr<BlendronicProcessor> proc, juce::ValueTree v, OpenGlWrapper& um);

    // Destructor method
    ~BlendronicPreparation();

    // Static function that returns a pointer to a BlendronicPreparation object
    static PreparationSection* createBlendronicSection(juce::ValueTree v, SynthGuiInterface* interface) {

        return new BlendronicPreparation(std::make_unique<BlendronicProcessor>(), v, interface->getGui()->open_gl_);
    }    // Public function definitions for the BlendronicPreparation class, which override functions
    // in the PreparationSection base class
    std::shared_ptr<SynthSection> getPrepPopup() override;
    void resized() override;

    juce::AudioProcessor* getProcessor() override;
    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;
private:

    // Private member variable for the BlendronicPreparation class: proc is a pointer to a
    // BlendronicProcessor Object
    BlendronicProcessor & proc;
    std::unique_ptr<BlendronicProcessor> _proc_ptr;

    /************************************************************************************/
    /*             NESTED CLASS: BlendronicPopup, inherits from PreparationPopup            */
    /************************************************************************************/

    class BlendronicPopup : public PreparationPopup {
    public:

        // Constructor method that takes two arguments: a smart pointer to a BlendronicProcessor,
        // and a reference to an OpenGlWrapper
        BlendronicPopup (BlendronicProcessor& proc, OpenGlWrapper& open_gl);

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

        ~BlendronicPopup();


    private:

        // Private function definitions and member variables for the BlendronicPopup class
        void redoImage();
        BlendronicParams* params = nullptr;
        BlendronicProcessor& proc;

    };
};



#endif //BITKLAVIER2_BlendronicPREPARATION_H
