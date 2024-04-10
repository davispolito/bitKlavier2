//
// Created by Davis Polito on 11/8/23.
//

#ifndef BITKLAVIER2_FULLINTERFACE_H
#define BITKLAVIER2_FULLINTERFACE_H

#include <juce_opengl/juce_opengl.h>
#include "open_gl_background.h"
#include "header_section.h"
#include "main_section.h"
#include "synth_section.h"

struct SynthGuiData;
class HeaderSection;
class MainSection;
namespace bitklavier{
    constexpr int kMinWindowWidth = 350;
    constexpr int kMinWindowHeight = 205;
    constexpr int kDefaultWindowWidth = 1400;
    constexpr int kDefaultWindowHeight = 820;
}
class FullInterface : public SynthSection, public juce::OpenGLRenderer, public HeaderSection::Listener,
                      public MainSection::Listener, DragAndDropContainer
{

public :
    static constexpr double kMinOpenGlVersion = 1.4;
    FullInterface(SynthGuiData *synth_gui_data);
     ~FullInterface() override;

     void paintBackground(Graphics& g) override;

    void parentHierarchyChanged() override {
        SynthSection::parentHierarchyChanged();
        checkShouldReposition();
    }
    void copySkinValues(const Skin& skin);
    void reloadSkin(const Skin& skin);

    void repaintChildBackground(SynthSection* child);
    void repaintSynthesisSection();
    void repaintOpenGlBackground(OpenGlComponent* component);
    void redoBackground();
    void checkShouldReposition(bool resize = true);
    void resized() override;
    void animate(bool animate) override;
    void reset() override;

    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

    void setFocus();
    void notifyChange();
    void notifyFresh();
    float getResizingScale() const { return width_ * 1.0f / resized_width_; }
    //float getPixelScaling() const override { return display_scale_; }
    int getPixelMultiple() const override { return pixel_multiple_; }
   // std::unique_ptr<HeaderSection> header_;
    void showFullScreenSection(SynthSection* full_screen);
    void enableRedoBackground(bool enable) {
        enable_redo_background_ = enable;
        if (enable)
            resized();
    }

    SynthGuiData* data;
    SynthSection* full_screen_section_;
private :
    std::unique_ptr<MainSection> main_;
    std::unique_ptr<HeaderSection> header_;
    int width_;
    int resized_width_;
    bool animate_;
    bool unsupported_{};
    float last_render_scale_;
    bool enable_redo_background_{};
    float display_scale_;
    int pixel_multiple_;
    CriticalSection open_gl_critical_section_;
    OpenGLContext open_gl_context_;
    std::unique_ptr<Shaders> shaders_;
    OpenGlWrapper open_gl_;
    Image background_image_;
    OpenGlBackground background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FullInterface)
};

#endif //BITKLAVIER2_FULLINTERFACE_H
