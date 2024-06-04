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



#include "synth_section.h"
#include "open_gl_background.h"

class LogoButton : public Button {
public:
    LogoButton(const String& name) : Button(name) {
        image_component_.setComponent(this);
        auto paths = Paths::logoPaths();
        layer_1_ = paths.getUnchecked(0);
        layer_2_ = paths.getUnchecked(1);
        layer_3_ = paths.getUnchecked(2);
        layer_4_ = paths.getUnchecked(3);
        layer_5_ = paths.getUnchecked(4);

    }



    void resized() override {
        const DropShadow shadow(Colours::white, 5,juce::Point<int>(0, 0));

        if (shadow_.getWidth() == getWidth() && shadow_.getHeight() == getHeight())
            return;

        Rectangle<float> bounds = getLocalBounds().toFloat();
        //letter_.applyTransform(letter_.getTransformToScaleToFit(bounds, true));
        //ring_.applyTransform(ring_.getTransformToScaleToFit(bounds, true));
        layer_1_.applyTransform(layer_1_.getTransformToScaleToFit(bounds,true));

        shadow_ = Image(Image::SingleChannel, getWidth(), getHeight(), true);

        Graphics shadow_g(shadow_);
        //shadow.drawForPath(shadow_g, letter_);
        //shadow.drawForPath(shadow_g, ring_);
        shadow.drawForPath(shadow_g, layer_1_);
        redoImage();
    }

    void paintButton(Graphics& g, bool hover, bool down) override {
        Rectangle<float> bounds = getLocalBounds().toFloat();
        //letter_.applyTransform(letter_.getTransformToScaleToFit(bounds, true));
        //ring_.applyTransform(ring_.getTransformToScaleToFit(bounds, true));
        layer_1_.applyTransform(layer_1_.getTransformToScaleToFit(bounds,true));
        layer_2_.applyTransform(layer_2_.getTransformToScaleToFit(bounds,true));
        layer_3_.applyTransform(layer_3_.getTransformToScaleToFit(bounds,true));
        layer_4_.applyTransform(layer_4_.getTransformToScaleToFit(bounds,true));
        layer_5_.applyTransform(layer_5_.getTransformToScaleToFit(bounds,true));
        g.setColour(findColour(Skin::kShadow, true));
        g.drawImageAt(shadow_, 0, 0, true);

        ColourGradient letter_gradient(letter_top_color_, 0.0f, 0.0f, letter_bottom_color_, 0.0f, getHeight(), false);
        ColourGradient ring_gradient(ring_top_color_, 0.0f, 0.0f, ring_bottom_color_, 0.0f, getHeight(), false);
        g.setColour(Colours::brown);
        g.fillPath(layer_1_);

        g.setColour(Colours::black);
        g.fillPath(layer_2_);

        g.setColour(Colours::white);
        g.fillPath(layer_3_);

        g.setColour(Colours::goldenrod);
        g.fillPath(layer_4_);
        g.fillPath(layer_5_);

        if (hover) {
            g.setColour(findColour(Skin::kLightenScreen, true));
            g.fillEllipse(getLocalBounds().toFloat());
        }
        else if (down) {
            g.setColour(findColour(Skin::kOverlayScreen, true));
            g.fillEllipse(getLocalBounds().toFloat());
        }
    }

    void setLetterColors(Colour top, Colour bottom) {
        letter_top_color_ = top;
        letter_bottom_color_ = bottom;
        redoImage();
    }

    void setRingColors(Colour top, Colour bottom) {
        ring_top_color_ = top;
        ring_bottom_color_ = bottom;
        redoImage();
    }

    void mouseEnter(const MouseEvent& e) override {
        Button::mouseEnter(e);
        image_component_.setColor(Colour(0xffdddddd));
    }

    void mouseExit(const MouseEvent& e) override {
        Button::mouseExit(e);
        image_component_.setColor(Colours::white);
    }

    OpenGlImageComponent* getImageComponent() { return &image_component_; }
    void redoImage() { image_component_.redrawImage(true); }

private:
    OpenGlImageComponent image_component_;

    juce::Path layer_1_;
    juce::Path layer_2_;
    juce::Path layer_3_;
    juce::Path layer_4_;
    juce::Path layer_5_;

    Image shadow_;

    Colour letter_top_color_;
    Colour letter_bottom_color_;

    Colour ring_top_color_;
    Colour ring_bottom_color_;
};

class LogoSection : public SynthSection {
  public:
    static constexpr float kLogoPaddingY = 2.0f;

    class Listener {
      public:
        virtual ~Listener() { }

        virtual void showAboutSection() = 0;
    };

    LogoSection();

    void resized() override;
    void paintBackground(Graphics& g) override;
    void buttonClicked(Button* clicked_button) override;
    void addListener(Listener* listener) { listeners_.push_back(listener); }

  private:
    std::vector<Listener*> listeners_;
    std::unique_ptr<LogoButton> logo_button_;
};

class HeaderSection : public SynthSection, public LogoSection::Listener {
  public:
    class Listener {
      public:
        virtual ~Listener() { }

        virtual void showAboutSection() = 0;
    };

    HeaderSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;

    void buttonClicked(Button* clicked_button) override;
    void sliderValueChanged(Slider* slider) override;


    void setTemporaryTab(String name);

//    void showAboutSection() override {
//      for (Listener* listener : listeners_)
//        listener->showAboutSection();
//    }

    void setTabOffset(int offset) { tab_offset_ = offset; repaint(); }
    void notifyChange();
    void notifyFresh();
    void showAboutSection() override {

        for (Listener* listener : listeners_)
            listener->showAboutSection();
    }
    void addListener(Listener* listener) { listeners_.push_back(listener); }

  private:
    std::vector<Listener*> listeners_;

    int tab_offset_;
    std::unique_ptr<PlainTextComponent> temporary_tab_;
    std::unique_ptr<OpenGlShapeButton> exit_temporary_button_;
    OpenGlQuad body_;
    std::unique_ptr<LogoSection> logo_section_;
    //Image background_image_;
    //std::unique_ptr<SynthButton> click_me;
   // juce::TextButton inspectButton { "Inspect the UI" };
    //std::unique_ptr<melatonin::Inspector> inspector;
    //std::unique_ptr<OpenGlToggleButton> inspectButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderSection)
};

