/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once



#include "synth_button.h"
#include "look_and_feel/fonts.h"
#include "paths.h"
//#include "open_gl_image_component.h"
//#include "open_gl_multi_quad.h"
#include "look_and_feel/shaders.h"

#include <functional>
#include <map>
#include <set>

namespace bitklavier
{
    namespace parameters_view_detail
    {
        class SliderParameterComponent;
    }
}


class ModulationButton;
class OpenGlComponent;
class PresetSelector;
class SynthSlider;
class PreparationSection;
class OpenGlBackground;
struct PopupItems {
  int id;
  std::string name;
  bool selected;
  std::vector<PopupItems> items;

  PopupItems() : id(0), selected(false) { }
  PopupItems(std::string name) : id(0), name(std::move(name)), selected(false) { }

  PopupItems(int id, std::string name, bool selected, std::vector<PopupItems> items) {
    this->id = id;
    this->selected = selected;
    this->name = std::move(name);
    this->items = std::move(items);
  }

  void addItem(int sub_id, const std::string& sub_name, bool sub_selected = false) {
    items.emplace_back(sub_id, sub_name, sub_selected, std::vector<PopupItems>());
  }
  void addItem(const PopupItems& item) { items.push_back(item); }
  int size() const { return static_cast<int>(items.size()); }
};

//class LoadingWheel : public OpenGlQuad {
//  public:
//    LoadingWheel() : OpenGlQuad(Shaders::kRotaryModulationFragment), tick_(0), complete_(false), complete_ticks_(0) {
//      setAlpha(1.0f);
//    }
//
//    void resized() override {
//      OpenGlQuad::resized();
//
//      juce::Colour color = findColour(Skin::kWidgetAccent1, true);
//      setColor(color);
//      setModColor(color);
//      setAltColor(color);
//    }
//
//    virtual void render(OpenGlWrapper& open_gl, bool animate) override {
//      static constexpr float kRotationMult = 0.05f;
//      static constexpr float kWidthFrequency = 0.025f;
//      static constexpr float kMinRads = 0.6f;
//      static constexpr float kMaxRads = 4.0f;
//      static constexpr float kRadRange = kMaxRads - kMinRads;
//      static constexpr float kCompleteSpeed = 0.15f;
//      static constexpr float kStartRads = -vital::kPi - 0.05f;
//
//      tick_++;
//      setStartPos(-tick_ * kRotationMult);
//
//      float width = (sinf(tick_ * kWidthFrequency) * 0.5f + 0.5f) * kRadRange + kMinRads;
//      if (complete_) {
//        complete_ticks_++;
//        width += kCompleteSpeed * complete_ticks_;
//      }
//
//      setShaderValue(0, kStartRads, 0);
//      setShaderValue(0, kStartRads + width, 1);
//      setShaderValue(0, kStartRads, 2);
//      setShaderValue(0, kStartRads + width, 3);
//
//      OpenGlQuad::render(open_gl, animate);
//    }
//
//    void completeRing() {
//      complete_ = true;
//    }
//
//  private:
//    int tick_;
//    bool complete_;
//    int complete_ticks_;
//};

//class AppLogo : public OpenGlImageComponent {
//  public:
//    AppLogo(juce::String name) : OpenGlImageComponent(std::move(name)) {
//      logo_letter_ = Paths::vitalV();
//      logo_ring_ = Paths::vitalRing();
//    }
//
//    void paint(juce::Graphics& g) override {
//      const juce::DropShadow shadow(findColour(Skin::kShadow, true), 10.0f,juce::Point<int>(0, 0));
//
//      logo_letter_.applyTransform(logo_letter_.getTransformToScaleToFit(getLocalBounds().toFloat(), true));
//      logo_ring_.applyTransform(logo_ring_.getTransformToScaleToFit(getLocalBounds().toFloat(), true));
//
//      shadow.drawForPath(g, logo_letter_);
//      shadow.drawForPath(g, logo_ring_);
//
//      juce::Colour letter_top_color = findColour(Skin::kWidgetSecondary1, true);
//      juce::Colour letter_bottom_color = findColour(Skin::kWidgetSecondary2, true);
//      juce::Colour ring_top_color = findColour(Skin::kWidgetPrimary1, true);
//      juce::Colour ring_bottom_color = findColour(Skin::kWidgetPrimary2, true);
//      juce::ColourGradient letter_gradient(letter_top_color, 0.0f, 12.0f, letter_bottom_color, 0.0f, 96.0f, false);
//      juce::ColourGradient ring_gradient(ring_top_color, 0.0f, 12.0f, ring_bottom_color, 0.0f, 96.0f, false);
//      g.setGradientFill(letter_gradient);
//      g.fillPath(logo_letter_);
//
//      g.setGradientFill(ring_gradient);
//      g.fillPath(logo_ring_);
//    }
//
//  private:
//    juce::Path logo_letter_;
//    juce::Path logo_ring_;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLogo)
//};


//class AppLogo : public OpenGlImageComponent {
//public:
//    AppLogo(juce::String name) : OpenGlImageComponent(std::move(name)) {
//
//    }
//
//    void paint(juce::Graphics& g) override {
//        const juce::DropShadow shadow(findColour(Skin::kShadow, true), 10.0f, juce::Point<int>(0, 0));
//
//        logo_letter_.applyTransform(logo_letter_.getTransformToScaleToFit(getLocalBounds().toFloat(), true));
//        logo_ring_.applyTransform(logo_ring_.getTransformToScaleToFit(getLocalBounds().toFloat(), true));
//
//        shadow.drawForPath(g, logo_letter_);
//        shadow.drawForPath(g, logo_ring_);
//
//        juce::Colour letter_top_color = findColour(Skin::kWidgetSecondary1, true);
//        juce::Colour letter_bottom_color = findColour(Skin::kWidgetSecondary2, true);
//        juce::Colour ring_top_color = findColour(Skin::kWidgetPrimary1, true);
//        juce::Colour ring_bottom_color = findColour(Skin::kWidgetPrimary2, true);
//        juce::ColourGradient letter_gradient(letter_top_color, 0.0f, 12.0f, letter_bottom_color, 0.0f, 96.0f, false);
//        juce::ColourGradient ring_gradient(ring_top_color, 0.0f, 12.0f, ring_bottom_color, 0.0f, 96.0f, false);
//        g.setGradientFill(letter_gradient);
//        g.fillPath(logo_letter_);
//
//        g.setGradientFill(ring_gradient);
//        g.fillPath(logo_ring_);
//    }
//
//private:
//    juce::Path logo_letter_;
//    juce::Path logo_ring_;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLogo)
//};

class SynthSection : public juce::Component, public juce::Slider::Listener,
                     public juce::Button::Listener, public SynthButton::ButtonListener {
  public:
    static constexpr int kDefaultPowerButtonOffset = 0;
    static constexpr float kPowerButtonPaddingPercent = 0.29f;
    static constexpr float kTransposeHeightPercent = 0.5f;
    static constexpr float kTuneHeightPercent = 0.4f;
    static constexpr float kJointModulationRadiusPercent = 0.1f;
    static constexpr float kJointModulationExtensionPercent = 0.6666f;
    static constexpr float kPitchLabelPercent = 0.33f;
    static constexpr float kJointLabelHeightPercent = 0.4f;
    static constexpr double kTransposeMouseSensitivity = 0.2;
    static constexpr float kJointLabelBorderRatioX = 0.05f;

    static constexpr int kDefaultBodyRounding = 4;
    static constexpr int kDefaultLabelHeight = 10;
    static constexpr int kDefaultLabelBackgroundHeight = 16;
    static constexpr int kDefaultLabelBackgroundWidth = 56;
    static constexpr int kDefaultLabelBackgroundRounding = 4;
    static constexpr int kDefaultPadding = 2;
    static constexpr int kDefaultPopupMenuWidth = 150;
    static constexpr int kDefaultDualPopupMenuWidth = 340;
    static constexpr int kDefaultStandardKnobSize = 32;
    static constexpr int kDefaultKnobThickness = 2;
    static constexpr float kDefaultKnobModulationAmountThickness = 2.0f;
    static constexpr int kDefaultKnobModulationMeterSize = 43;
    static constexpr int kDefaultKnobModulationMeterThickness = 4;
    static constexpr int kDefaultModulationButtonWidth = 64;
    static constexpr int kDefaultModFontSize = 10;
    static constexpr int kDefaultKnobSectionHeight = 64;
    static constexpr int kDefaultSliderWidth = 24;
    static constexpr int kDefaultTextWidth = 80;
    static constexpr int kDefaultTextHeight = 24;
    static constexpr int kDefaultWidgetMargin = 6;
    static constexpr float kDefaultWidgetFillFade = 0.3f;
    static constexpr float kDefaultWidgetLineWidth = 4.0f;
    static constexpr float kDefaultWidgetFillCenter = 0.0f;

    class OffOverlay : public OpenGlQuad {
      public:
        OffOverlay() : OpenGlQuad(Shaders::kColorFragment) { }

        void paintBackground(juce::Graphics& g) override { }
    };

    SynthSection(const juce::String& name);
    SynthSection(const juce::String& name, OpenGlWrapper* open_gl);
    virtual ~SynthSection() = default;

    void setParent(const SynthSection* parent) { parent_ = parent; }
    float findValue(Skin::ValueId value_id) const;

    virtual void reset();
    virtual void resized() override;
    virtual void paint(juce::Graphics& g) override;
    virtual void paintSidewaysHeadingText(juce::Graphics& g);
    virtual void paintHeadingText(juce::Graphics& g);
    virtual void paintBackground(juce::Graphics& g);
    virtual void repaintBackground();
    void showPopupBrowser(SynthSection* owner, juce::Rectangle<int> bounds, std::vector<juce::File> directories,
                          juce::String extensions, std::string passthrough_name, std::string additional_folders_name);
//    void updatePopupBrowser(SynthSection* owner);
//
    void showPopupSelector(juce::Component* source,juce::Point<int> position, const PopupItems& options,
                           std::function<void(int)> callback, std::function<void()> cancel = { });
//    void showDualPopupSelector(juce::Component* source,juce::Point<int> position, int width,
//                               const PopupItems& options, std::function<void(int)> callback);
    void showPopupDisplay(juce::Component* source, const std::string& text,
                          juce::BubbleComponent::BubblePlacement placement, bool primary);
    void hidePopupDisplay(bool primary);

    virtual void loadFile(const juce::File& file) { }
    virtual juce::File getCurrentFile() { return juce::File(); }
    virtual std::string getFileName() { return ""; }
    virtual std::string getFileAuthor() { return ""; }
    virtual void paintContainer(juce::Graphics& g);
    virtual void paintBody(juce::Graphics& g, juce::Rectangle<int> bounds);
    virtual void paintBorder(juce::Graphics& g, juce::Rectangle<int> bounds);
    virtual void paintBody(juce::Graphics& g);
    virtual void paintBorder(juce::Graphics& g);
    int getComponentShadowWidth();
    virtual void paintTabShadow(juce::Graphics& g);
    void paintTabShadow(juce::Graphics& g, juce::Rectangle<int> bounds);
    virtual void paintBackgroundShadow(juce::Graphics& g) { }
    virtual void setSizeRatio(float ratio);
    void paintKnobShadows(juce::Graphics& g);
    juce::Font getLabelFont();
    void setLabelFont(juce::Graphics& g);
    void drawLabelConnectionForComponents(juce::Graphics& g, juce::Component* left, juce::Component* right);
    void drawLabelBackground(juce::Graphics& g, juce::Rectangle<int> bounds, bool text_component = false);
    void drawLabelBackgroundForComponent(juce::Graphics& g, juce::Component* component);
    juce::Rectangle<int> getDividedAreaBuffered(juce::Rectangle<int> full_area, int num_sections, int section, int buffer);
    juce::Rectangle<int> getDividedAreaUnbuffered(juce::Rectangle<int> full_area, int num_sections, int section, int buffer);
    juce::Rectangle<int> getLabelBackgroundBounds(juce::Rectangle<int> bounds, bool text_component = false);
    juce::Rectangle<int> getLabelBackgroundBounds(juce::Component* component, bool text_component = false) {
      return getLabelBackgroundBounds(component->getBounds(), text_component);
    }
    void drawLabel(juce::Graphics& g, juce::String text, juce::Rectangle<int> component_bounds, bool text_component = false);
    void drawLabelForComponent(juce::Graphics& g, juce::String text, juce::Component* component, bool text_component = false) {
      drawLabel(g, std::move(text), component->getBounds(), text_component);
    }
    void drawTextBelowComponent(juce::Graphics& g, juce::String text, juce::Component* component, int space, int padding = 0);

    virtual void paintChildrenShadows(juce::Graphics& g);
    void paintChildrenBackgrounds(juce::Graphics& g);
    void paintOpenGlChildrenBackgrounds(juce::Graphics& g);
    void paintChildBackground(juce::Graphics& g, SynthSection* child);
    void paintChildShadow(juce::Graphics& g, SynthSection* child);
    void paintOpenGlBackground(juce::Graphics& g, OpenGlComponent* child);
    void drawTextComponentBackground(juce::Graphics& g, juce::Rectangle<int> bounds, bool extend_to_label);
    void drawTempoDivider(juce::Graphics& g, juce::Component* sync);
    virtual void initOpenGlComponents(OpenGlWrapper& open_gl);
    virtual void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate);
    virtual void destroyOpenGlComponents(OpenGlWrapper& open_gl);
    void destroyOpenGlComponent(OpenGlComponent & open_gl_component, OpenGlWrapper& open_gl);

    virtual void sliderValueChanged(juce::Slider* moved_slider) override;
    virtual void buttonClicked(juce::Button* clicked_button) override;
    virtual void guiChanged(SynthButton* button) override;

    std::map<std::string, SynthSlider*> getAllSliders() { return all_sliders_; }
    std::map<std::string, juce::ToggleButton*> getAllButtons() { return all_buttons_; }

    std::vector<juce::Component*> getAllSlidersVec() { return all_sliders_v; }
    float getKnobSectionHeight();

    virtual void setActive(bool active);
    bool isActive() const { return active_; }
    virtual void animate(bool animate);


    void addModulationButton(ModulationButton* button, bool show = true);
    void addSubSection(SynthSection* section, bool show = true);
    void removeSubSection(SynthSection* section);
    virtual void setScrollWheelEnabled(bool enabled);
    juce::ToggleButton* activator() const { return activator_; }
    void showPrepPopup(PreparationSection* prep);
    float getTitleWidth();
    float getPadding();
    float getPowerButtonOffset() const { return size_ratio_ * kDefaultPowerButtonOffset; }
//    float getKnobSectionHeight();
    float getSliderWidth();
//    float getSliderOverlap();
//    float getSliderOverlapWithSpace();
    float getTextComponentHeight();
    float getStandardKnobSize();
    float getTotalKnobHeight() { return getStandardKnobSize(); }
    float getTextSectionYOffset();
    float getModButtonWidth();
    float getModFontSize();
    float getWidgetMargin();
    float getWidgetRounding();
    float getSizeRatio() const { return size_ratio_; }
    int getPopupWidth() const { return kDefaultPopupMenuWidth * size_ratio_; }
    int getDualPopupWidth() const { return kDefaultDualPopupMenuWidth * size_ratio_; }
    virtual void setSkinValues(const Skin& skin, bool top_level);
    void setSkinValues(std::map<Skin::ValueId, float> values) { value_lookup_ = std::move(values); }
    void setSkinOverride(Skin::SectionOverride skin_override) { skin_override_ = skin_override; }

    void addSlider(SynthSlider* slider, bool show = true, bool listen = true);
    OpenGlWrapper *open_gl;
    float getDisplayScale() const;
    void addOpenGlComponent(std::shared_ptr<OpenGlComponent> open_gl_component, bool to_beginning = false, bool makeVisible = true);
    void addButton(OpenGlToggleButton* button, bool show = true);
   void  placeKnobsInArea(juce::Rectangle<int> area, std::vector<std::unique_ptr<bitklavier::parameters_view_detail::SliderParameterComponent>>& knobs);
  protected:
    void setSliderHasHzAlternateDisplay(SynthSlider* slider);
    void setSidewaysHeading(bool sideways) { sideways_heading_ = sideways; }
    void addToggleButton(juce::ToggleButton* button, bool show);

    void addButton(OpenGlShapeButton* button, bool show = true);


    void addBackgroundComponent(OpenGlBackground* open_gl_component, bool to_beginning = false);
    void setActivator(SynthButton* activator);
    void createOffOverlay();
    void setPresetSelector(PresetSelector* preset_selector, bool half = false) {
      preset_selector_ = preset_selector;
      preset_selector_half_width_ = half;
    }
    void paintJointControlSliderBackground(juce::Graphics& g, int x, int y, int width, int height);
    void paintJointControlBackground(juce::Graphics& g, int x, int y, int width, int height);
    void paintJointControl(juce::Graphics& g, int x, int y, int width, int height, const std::string& name);
    void placeJointControls(int x, int y, int width, int height,
                            SynthSlider* left, SynthSlider* right, juce::Component* widget = nullptr);
    void placeTempoControls(int x, int y, int width, int height, SynthSlider* tempo, SynthSlider* sync);
    void placeRotaryOption(juce::Component* option, SynthSlider* rotary);
    void placeKnobsInArea(juce::Rectangle<int> area,  std::vector<std::unique_ptr<juce::Component>> &knobs);
    void placeKnobsInArea(juce::Rectangle<int> area,  std::vector<juce::Component*> knobs);
    void lockCriticalSection();
    void unlockCriticalSection();
    juce::Rectangle<int> getPresetBrowserBounds();
    int getTitleTextRight();
    juce::Rectangle<int> getPowerButtonBounds();
    juce::Rectangle<int> getTitleBounds();

    virtual int getPixelMultiple() const;

    std::vector<SynthSection*> sub_sections_;
    std::vector<std::shared_ptr<OpenGlComponent>> open_gl_components_;
    OpenGlBackground* background_;
    std::map<std::string, SynthSlider*> slider_lookup_;
    std::map<std::string, juce::Button*> button_lookup_;

    std::map<std::string, SynthSlider*> all_sliders_;
    std::vector<juce::Component*> all_sliders_v;
    std::map<std::string, juce::ToggleButton*> all_buttons_;
    std::map<Skin::ValueId, float> value_lookup_;

    const SynthSection* parent_;
    SynthButton* activator_;
    PresetSelector* preset_selector_;
    bool preset_selector_half_width_;
    std::shared_ptr<OffOverlay> off_overlay_;
    Skin::SectionOverride skin_override_;
    OpenGlComponent* objectToDelete;
    float size_ratio_;
    bool active_;
    bool sideways_heading_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSection)
};

