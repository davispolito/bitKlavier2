//
// Created by Joshua Warner on 6/27/24.
//

#ifndef BITKLAVIER2_RESONANCEPREPARATION_H
#define BITKLAVIER2_RESONANCEPREPARATION_H

/************************************************************************************/
/*                 Created by Davis Polito and Joshua Warner                        */
/************************************************************************************/

#include "ResonanceProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"
#include "FullInterface.h"
/************************************************************************************/
/*                              CLASS: OpenGlSlider                                 */
/************************************************************************************/

class OpenGlSlider;

/************************************************************************************/
/*             CLASS: ResonancePreparation, inherits from PreparationSection           */
/************************************************************************************/

class ResonancePreparation : public PreparationSection {
public:

    // Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
    // a value tree, and a reference to an OpenGlWrapper object
    ResonancePreparation(std::unique_ptr<ResonanceProcessor> proc, juce::ValueTree v, OpenGlWrapper& um);

    // Destructor method
    ~ResonancePreparation();

    // Static function that returns a pointer to a ResonancePreparation object
    static PreparationSection* createResonanceSection(ValueTree v, SynthGuiInterface* interface) {

        return new ResonancePreparation(std::make_unique<ResonanceProcessor>(), v, interface->getGui()->open_gl_);
    }

    // Public function definitions for the ResonancePreparation class, which override functions
    // in the PreparationSection base class
    std::shared_ptr<SynthSection> getPrepPopup() override;
    void resized() override;

    juce::AudioProcessor* getProcessor() override;
    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;
private:

    // Private member variable for the ResonancePreparation class: proc is a pointer to a
    // ResonanceProcessor Object
    ResonanceProcessor & proc;
    std::unique_ptr<ResonanceProcessor> _proc_ptr;

    /************************************************************************************/
    /*             NESTED CLASS: ResonancePopup, inherits from PreparationPopup            */
    /************************************************************************************/

    class ResonancePopup : public PreparationPopup {
    public:

        // Constructor method that takes two arguments: a smart pointer to a ResonanceProcessor,
        // and a reference to an OpenGlWrapper
        ResonancePopup (ResonanceProcessor& proc, OpenGlWrapper& open_gl);

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

        ~ResonancePopup();


    private:

        // Private function definitions and member variables for the ResonancePopup class
        void redoImage();
        ResonanceParams* params = nullptr;
        ResonanceProcessor& proc;

    };
};




#endif //BITKLAVIER2_RESONANCEPREPARATION_H
