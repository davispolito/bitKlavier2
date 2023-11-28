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
//class LogoButton;
//
//class LogoSection : public SynthSection {
//  public:
//    static constexpr float kLogoPaddingY = 2.0f;
//
//    class Listener {
//      public:
//        virtual ~Listener() { }
//
//        virtual void showAboutSection() = 0;
//    };
//
//    LogoSection();
//
//    void resized() override;
//    void paintBackground(Graphics& g) override;
//    void buttonClicked(Button* clicked_button) override;
//    void addListener(Listener* listener) { listeners_.push_back(listener); }
//
//  private:
//    std::vector<Listener*> listeners_;
//    std::unique_ptr<LogoButton> logo_button_;
//};

class HeaderSection : public SynthSection {
  public:
    class Listener {
      public:
        virtual ~Listener() { }

        //virtual void showAboutSection() = 0;
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

    void addListener(Listener* listener) { listeners_.push_back(listener); }

  private:
    std::vector<Listener*> listeners_;

    int tab_offset_;
    std::unique_ptr<PlainTextComponent> temporary_tab_;
    std::unique_ptr<OpenGlShapeButton> exit_temporary_button_;

    std::unique_ptr<SynthButton> click_me;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderSection)
};

