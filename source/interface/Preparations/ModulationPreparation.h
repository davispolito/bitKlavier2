//
// Created by Davis Polito on 1/30/25.
//

#ifndef BITKLAVIER2_MODULATIONPREPARATION_H
#define BITKLAVIER2_MODULATIONPREPARATION_H


/************************************************************************************/
/*                 Created by Davis Polito and Joshua Warner                        */
/************************************************************************************/

#include "ModulationProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"
//#include "ParameterView/ModulationParametersView.h"
#include "ModulationProcessor.h"
#include "FullInterface.h"

#include "envelope_section.h"

class ModulationModuleSection;
/************************************************************************************/
/*             CLASS: ModulationPreparation, inherits from PreparationSection           */
/************************************************************************************/

class ModulationPreparation : public PreparationSection
{
public:

    // Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
    // a value tree, and a reference to an OpenGlWrapper object
    ModulationPreparation(std::unique_ptr<bitklavier::ModulationProcessor> proc, juce::ValueTree v, OpenGlWrapper& um);

    // Destructor method
    ~ModulationPreparation();

    // Static function that returns a pointer to a ModulationPreparation object
    static PreparationSection* createModulationSection(juce::ValueTree v, SynthGuiInterface* interface) {

        return new ModulationPreparation(std::make_unique<bitklavier::ModulationProcessor>(v), v, interface->getGui()->open_gl_);
    }





    std::shared_ptr<SynthSection> getPrepPopup() override;
    void resized() override;
    void paintBackground(juce::Graphics &g);
    juce::AudioProcessor* getProcessor() override;
    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;

private:
    // Private member variable for the ModulationPreparation class: proc is a pointer to a
    // ModulationProcessor Object
    bitklavier::ModulationProcessor & proc;
    std::unique_ptr<bitklavier::ModulationProcessor> _proc_ptr;

    /************************************************************************************/
    /*             NESTED CLASS: ModulationPopup, inherits from PreparationPopup            */
    /************************************************************************************/

    class ModulationPopup : public PreparationPopup {
    public:

        // Constructor method that takes two arguments: a smart pointer to a ModulationProcessor,
        // and a reference to an OpenGlWrapper
        ModulationPopup (ModulationPreparation &,bitklavier::ModulationProcessor& proc, OpenGlWrapper& open_gl);

        // Public function definitions for the class, which override the base class methods for
        // initializing, rendering, resizing, and painting OpenGl components
        void initOpenGlComponents(OpenGlWrapper &open_gl) override;
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

        ~ModulationPopup();


    private:

        // Private function definitions and member variables for the ModulationPopup class
//        ModulationParams* params = nullptr;
        bitklavier::ModulationProcessor& proc;
        std::unique_ptr<ModulationModuleSection> view;
    };
    friend class ModulationPopup;
};



#endif //BITKLAVIER2_MODULATIONPREPARATION_H
