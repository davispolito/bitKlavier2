//
// Created by Davis Polito on 2/13/24.
//

#ifndef BITKLAVIER2_DIRECTPREPARATION_H
#define BITKLAVIER2_DIRECTPREPARATION_H
#include "PreparationSection.h"

#include "PolygonalOscProcessor.h"
#include "popup_browser.h"
class OpenGlSlider;
class DirectPreparation : public PreparationSection {
public:
    DirectPreparation(std::shared_ptr<PolygonalOscProcessor> proc, ValueTree v, OpenGlWrapper &um);

    ~DirectPreparation();

    static PreparationSection *createDirectSection(ValueTree v, OpenGlWrapper &um) {

        return new DirectPreparation(std::make_shared<PolygonalOscProcessor>(), v, um);
    }

    class DirectPopup : public PreparationPopup
    {
    public:
        DirectPopup (std::shared_ptr<PolygonalOscProcessor> proc, OpenGlWrapper &open_gl);

        void initOpenGlComponents(OpenGlWrapper &open_gl) override;
        void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) override;
        void resized() override;
        void paintBackground(Graphics& g) override{

//            paintContainer(g);
//            paintHeadingText(g);
//            paintBorder(g);
//            paintKnobShadows(g);
//            paintChildrenBackgrounds(g);

        }

        int getViewPosition() {
            int view_height = getHeight();
            return view_height;//std::max(0, std::min<int>(selections_.size() * getRowHeight() - view_height, view_position_));
        }
    private:
        void redoImage();
        PolygonOscParams* params = nullptr;
        std::shared_ptr<PolygonalOscProcessor> proc;
        std::unique_ptr<SynthSlider> slider;
        OpenGlImage sliderShadows;
    };

    std::shared_ptr<SynthSection> getPrepPopup() override;

    void resized() override;
    std::shared_ptr<juce::AudioProcessor> getProcessor() override;
private:


    std::shared_ptr<PolygonalOscProcessor> proc;

};

#endif //BITKLAVIER2_DIRECTPREPARATION_H
