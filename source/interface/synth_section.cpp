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

#include "synth_section.h"

#include "look_and_feel/skin.h"
#include "look_and_feel/fonts.h"
#include "FullInterface.h"
#include "open_gl_component.h"
#include "synth_gui_interface.h"
#include "synth_slider.h"
#include "ParameterView/ParametersView.h"
SynthSection::SynthSection(const juce::String& name) : juce::Component(name), parent_(nullptr), activator_(nullptr),
                                                 preset_selector_(nullptr), preset_selector_half_width_(false),
                                                 skin_override_(Skin::kNone), size_ratio_(1.0f),
                                                 active_(true), sideways_heading_(true), background_(nullptr) {
  setWantsKeyboardFocus(true);
}

SynthSection::SynthSection(const juce::String& name, OpenGlWrapper* open_gl)  : juce::Component(name), parent_(nullptr), activator_(nullptr),
preset_selector_(nullptr), preset_selector_half_width_(false),
skin_override_(Skin::kNone), size_ratio_(1.0f),
active_(true), sideways_heading_(true), background_(nullptr), open_gl(open_gl) {
    setWantsKeyboardFocus(true);
}


float SynthSection::findValue(Skin::ValueId value_id) const {
  if (value_lookup_.count(value_id)) {
      if (Skin::shouldScaleValue(value_id))
          return size_ratio_ * value_lookup_.at(value_id);
      return value_lookup_.at(value_id);
  }
  if (parent_)
      return parent_->findValue(value_id);

  return 0.0f;

}

void SynthSection::reset() {
  for (auto& sub_section : sub_sections_)
    sub_section->reset();
}

void SynthSection::resized() {
  juce::Component::resized();
  if (off_overlay_) {
    off_overlay_->setBounds(getLocalBounds());
    off_overlay_->setColor(findColour(Skin::kBackground, true).withMultipliedAlpha(0.8f));
  }
  if (activator_)
    activator_->setBounds(getPowerButtonBounds());
//  if (preset_selector_) {
//    preset_selector_->setBounds(getPresetBrowserBounds());
//    preset_selector_->setRoundAmount(findValue(Skin::kBodyRounding));
//  }
}

void SynthSection::paint(juce::Graphics& g) { }

void SynthSection::paintSidewaysHeadingText(juce::Graphics& g) {
  int title_width = findValue(Skin::kTitleWidth);
  g.setColour(findColour(Skin::kHeadingText, true));
  g.setFont(Fonts::instance()->proportional_light().withPointHeight(size_ratio_ * 14.0f));
  g.saveState();
  g.setOrigin(juce::Point<int>(0, getHeight()));
  g.addTransform(juce::AffineTransform::rotation(-bitklavier::kPi / 2.0f));
  int height = getHeight();
  if (activator_)
    height = getHeight() - title_width / 2;

  g.drawText(getName(), juce::Rectangle<int>(0, 0, height, title_width), juce::Justification::centred, false);
  g.restoreState();
}

void SynthSection::paintHeadingText(juce::Graphics& g) {
  if (sideways_heading_) {
    paintSidewaysHeadingText(g);
    return;
  }

  g.setColour(findColour(Skin::kHeadingText, true));
  g.setFont(Fonts::instance()->proportional_light().withPointHeight(size_ratio_ * 14.0f));
  g.drawText(TRANS(getName()), getTitleBounds(), juce::Justification::centred, false);
}

void SynthSection::paintBackground(juce::Graphics& g) {
  paintContainer(g);
  paintHeadingText(g);

  paintKnobShadows(g);
  paintChildrenBackgrounds(g);
  paintBorder(g);
}

void SynthSection::setSkinValues(const Skin& skin, bool top_level) {
  skin.setComponentColors(this, skin_override_, top_level);
  skin.setComponentValues(this, skin_override_, top_level);
  for (auto& sub_section : sub_sections_)
    sub_section->setSkinValues(skin, false);

  for (auto& open_gl_component : open_gl_components_)
    open_gl_component->setSkinValues(skin);
}

void SynthSection::repaintBackground() {
  if (!isShowing())
    return;

  FullInterface* parent = findParentComponentOfClass<FullInterface>();
  if (parent)
    parent->repaintChildBackground(this);
}







void SynthSection::paintContainer(juce::Graphics& g) {
  paintBody(g);
  
  g.saveState();
  if (sideways_heading_) {
    int title_width = findValue(Skin::kTitleWidth);
    g.reduceClipRegion(0, 0, title_width, getHeight());
    g.setColour(findColour(Skin::kBodyHeading, true));
    g.fillRoundedRectangle(0, 0, title_width * 2, getHeight(), findValue(Skin::kBodyRounding));
  }
  else {
    g.reduceClipRegion(0, 0, getWidth(), getTitleWidth());
    g.setColour(findColour(Skin::kBodyHeading, true));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), findValue(Skin::kBodyRounding));
  }
  
  g.restoreState();
}

void SynthSection::paintBody(juce::Graphics& g, juce::Rectangle<int> bounds) {
  g.setColour(findColour(Skin::kBody, true));
  g.fillRoundedRectangle(bounds.toFloat(), findValue(Skin::kBodyRounding));
}

void SynthSection::paintBorder(juce::Graphics& g, juce::Rectangle<int> bounds) {
  int body_rounding = findValue(Skin::kBodyRounding);
  g.setColour(findColour(Skin::kBorder, true));
  g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f), body_rounding, 1.0f);
}

void SynthSection::paintBody(juce::Graphics& g) {
  paintBody(g, getLocalBounds());
}

void SynthSection::paintBorder(juce::Graphics& g) {
  paintBorder(g, getLocalBounds());
}

int SynthSection::getComponentShadowWidth() {
  return std::round(size_ratio_ * 2.0f);
}

void SynthSection::paintTabShadow(juce::Graphics& g) {
  paintTabShadow(g, getLocalBounds());
}

void SynthSection::paintTabShadow(juce::Graphics& g, juce::Rectangle<int> bounds) {
  static constexpr float kCornerScale = 0.70710678119f;
  int corner_size = findValue(Skin::kBodyRounding);
  int shadow_size = getComponentShadowWidth();
  int corner_and_shadow = corner_size + shadow_size;

  float corner_shadow_offset = corner_size - corner_and_shadow * kCornerScale;
  float corner_ratio = corner_size * 1.0f / corner_and_shadow;

  juce::Colour shadow_color = findColour(Skin::kShadow, true);
  juce::Colour transparent = shadow_color.withAlpha(0.0f);

  int left = bounds.getX();
  int top = bounds.getY();
  int right = bounds.getRight();
  int bottom = bounds.getBottom();

  g.setGradientFill(juce::ColourGradient(shadow_color, left, 0, transparent, left - shadow_size, 0, false));
  g.fillRect(left - shadow_size, top + corner_size, shadow_size, bottom - top - corner_size * 2);

  g.setGradientFill(juce::ColourGradient(shadow_color, right, 0, transparent, right + shadow_size, 0, false));
  g.fillRect(right, top + corner_size, shadow_size, bottom - top - corner_size * 2);

  g.setGradientFill(juce::ColourGradient(shadow_color, 0, top, transparent, 0, top - shadow_size, false));
  g.fillRect(left + corner_size, top - shadow_size, right - left - corner_size * 2, shadow_size);

  g.setGradientFill(juce::ColourGradient(shadow_color, 0, bottom, transparent, 0, bottom + shadow_size, false));
  g.fillRect(left + corner_size, bottom, right - left - corner_size * 2, shadow_size);

  juce::ColourGradient top_left_corner(shadow_color, left + corner_size, top + corner_size,
                                 transparent, left + corner_shadow_offset, top + corner_shadow_offset, true);
  top_left_corner.addColour(corner_ratio, shadow_color);
  g.setGradientFill(top_left_corner);
  g.fillRect(left - shadow_size, top - shadow_size, corner_and_shadow, corner_and_shadow);

  juce::ColourGradient top_right_corner(shadow_color, right - corner_size, top + corner_size,
                                  transparent, right - corner_shadow_offset, top + corner_shadow_offset, true);
  top_right_corner.addColour(corner_ratio, shadow_color);
  g.setGradientFill(top_right_corner);
  g.fillRect(right - corner_size, top - shadow_size, corner_and_shadow, corner_and_shadow);

  juce::ColourGradient bottom_left_corner(shadow_color, left + corner_size, bottom - corner_size,
                                    transparent, left + corner_shadow_offset, bottom - corner_shadow_offset, true);
  bottom_left_corner.addColour(corner_ratio, shadow_color);
  g.setGradientFill(bottom_left_corner);
  g.fillRect(left - shadow_size, bottom - corner_size, corner_and_shadow, corner_and_shadow);

  juce::ColourGradient bottom_right_corner(shadow_color, right - corner_size, bottom - corner_size,
                                     transparent, right - corner_shadow_offset, bottom - corner_shadow_offset, true);
  bottom_right_corner.addColour(corner_ratio, shadow_color);
  g.setGradientFill(bottom_right_corner);
  g.fillRect(right - corner_size, bottom - corner_size, corner_and_shadow, corner_and_shadow);
}

void SynthSection::setSizeRatio(float ratio) {
  size_ratio_ = ratio;

  for (auto& sub_section : sub_sections_)
    sub_section->setSizeRatio(ratio);
}

void SynthSection::paintKnobShadows(juce::Graphics& g) {
  for (auto& slider : slider_lookup_) {
    if (slider.second->isVisible() && slider.second->getWidth() && slider.second->getHeight())
      slider.second->drawShadow(g);
  }
}

void SynthSection::paintChildrenShadows(juce::Graphics& g) {
  for (auto& sub_section : sub_sections_) {
    if (sub_section->isVisible())
      paintChildShadow(g, sub_section);
  }
}

void SynthSection::paintOpenGlChildrenBackgrounds(juce::Graphics& g) {
  for (auto& open_gl_component : open_gl_components_) {
    if (open_gl_component->isVisible())
      paintOpenGlBackground(g, open_gl_component.get());
  }
}

void SynthSection::paintChildrenBackgrounds(juce::Graphics& g) {
  for (auto& sub_section : sub_sections_) {
    if (sub_section->isVisible())
      paintChildBackground(g, sub_section);
  }

  paintOpenGlChildrenBackgrounds(g);

//  if (preset_selector_) {
//    g.saveState();
//    juce::Rectangle<int> bounds = getLocalArea(preset_selector_, preset_selector_->getLocalBounds());
//    g.reduceClipRegion(bounds);
//    g.setOrigin(bounds.getTopLeft());
//    preset_selector_->paintBackground(g);
//    g.restoreState();
//  }
}

void SynthSection::paintChildBackground(juce::Graphics& g, SynthSection* child) {
  g.saveState();
  juce::Rectangle<int> bounds = getLocalArea(child, child->getLocalBounds());
  g.reduceClipRegion(bounds);
  g.setOrigin(bounds.getTopLeft());
  child->paintBackground(g);
  g.restoreState();
}

void SynthSection::paintChildShadow(juce::Graphics& g, SynthSection* child) {
  g.saveState();
  juce::Rectangle<int> bounds = getLocalArea(child, child->getLocalBounds());
  g.setOrigin(bounds.getTopLeft());
  child->paintBackgroundShadow(g);
  child->paintChildrenShadows(g);
  g.restoreState();
}

void SynthSection::paintOpenGlBackground(juce::Graphics &g, OpenGlComponent* open_gl_component) {
  g.saveState();
  juce::Rectangle<int> bounds = getLocalArea(open_gl_component, open_gl_component->getLocalBounds());
  g.reduceClipRegion(bounds);
  g.setOrigin(bounds.getTopLeft());
  open_gl_component->paintBackground(g);
  g.restoreState();
}

void SynthSection::drawTextComponentBackground(juce::Graphics& g, juce::Rectangle<int> bounds, bool extend_to_label) {
  if (bounds.getWidth() <= 0 || bounds.getHeight() <= 0)
    return;

  g.setColour(findColour(Skin::kTextComponentBackground, true));
  int y = bounds.getY();
  int rounding = bounds.getHeight() / 2;

  if (extend_to_label) {
    int label_bottom = bounds.getBottom() + findValue(Skin::kTextComponentLabelOffset);
    rounding = findValue(Skin::kLabelBackgroundRounding);
    g.fillRoundedRectangle(bounds.toFloat(), rounding);

    int extend_y = y + bounds.getHeight() / 2;
    g.fillRect(bounds.getX(), extend_y, bounds.getWidth(), label_bottom - extend_y - rounding);
  }
  else
    g.fillRoundedRectangle(bounds.toFloat(), rounding);
}

void SynthSection::drawTempoDivider(juce::Graphics& g, juce::Component* sync) {
  static constexpr float kLineRatio = 0.5f;

  g.setColour(findColour(Skin::kLightenScreen, true));
  int height = sync->getHeight();
  int line_height = height * kLineRatio;
  int y = sync->getY() + (height - line_height) / 2;
  g.drawRect(sync->getX(), y, 1, line_height);
}

void SynthSection::initOpenGlComponents(OpenGlWrapper& open_gl) {
    if ((juce::OpenGLContext::getCurrentContext() == nullptr))
    {
        open_gl.context.executeOnGLThread([this, &open_gl](juce::OpenGLContext &openGLContext) {
            for (auto &open_gl_component: open_gl_components_) {
                open_gl_component->init(open_gl);
                DBG("init " + open_gl_component->getName());
            }

            for (auto &sub_section: sub_sections_)
                sub_section->initOpenGlComponents(open_gl);

            for (auto &open_gl_component: open_gl_components_)
                open_gl_component->setVisible(true);

            if (background_)
                background_->init(open_gl);
        }, true);
    } else
    {
        DBG("sent: init parameters execute on gl");
        for (auto& open_gl_component : open_gl_components_)
            open_gl_component->init(open_gl);

        for (auto& sub_section : sub_sections_)
            sub_section->initOpenGlComponents(open_gl);

        if(background_)
            background_->init(open_gl);
    }


}

void SynthSection::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {
    if(background_)
        background_->render(open_gl);
  for (auto& sub_section : sub_sections_) {
    if (sub_section->isVisible() && !sub_section->isAlwaysOnTop())
      sub_section->renderOpenGlComponents(open_gl, animate);
  }

  for (auto& open_gl_component : open_gl_components_) {
      if (!open_gl_component->isInit())
      {
          open_gl_component->init(open_gl);
          GLenum gl =  juce::gl::glGetError();
          _ASSERT(gl == juce::gl::GL_NO_ERROR);
      }
    if (open_gl_component->isVisible() && !open_gl_component->isAlwaysOnTop()) {
      open_gl_component->render(open_gl, animate);
      GLenum gl =  juce::gl::glGetError();
      //DBG(juce::String(gl));
      _ASSERT(gl == juce::gl::GL_NO_ERROR);
    }
  }

  for (auto& sub_section : sub_sections_) {
    if (sub_section->isVisible() && sub_section->isAlwaysOnTop())
      sub_section->renderOpenGlComponents(open_gl, animate);
  }

  for (auto& open_gl_component : open_gl_components_) {
      if (!open_gl_component->isInit())
      {
          open_gl_component->init(open_gl);
          GLenum gl =  juce::gl::glGetError();
          _ASSERT(gl == juce::gl::GL_NO_ERROR);
      }
    if (open_gl_component->isVisible() && open_gl_component->isAlwaysOnTop()) {
      open_gl_component->render(open_gl, animate);
      _ASSERT(juce::gl::glGetError() == juce::gl::GL_NO_ERROR);
    }
  }
//    if(background_)
//        background_->render(open_gl);
}

void SynthSection::destroyOpenGlComponents(OpenGlWrapper& open_gl) {
    if ((juce::OpenGLContext::getCurrentContext() == nullptr))
    {
        open_gl.context.executeOnGLThread([this, &open_gl](juce::OpenGLContext &openGLContext) {
            for (auto& open_gl_component : open_gl_components_)
                open_gl_component->destroy(open_gl);

            for (auto& sub_section : sub_sections_)
            {
                sub_section->destroyOpenGlComponents(open_gl);

            }


            if(background_)
                background_->destroy(open_gl);
            DBG("finished openglexercuted");
            GLenum gl =  juce::gl::glGetError();
            DBG(juce::String(gl));

        }, false);
    } else
    {
        for (auto& open_gl_component : open_gl_components_)
            open_gl_component->destroy(open_gl);

        for (auto& sub_section : sub_sections_)
            sub_section->destroyOpenGlComponents(open_gl);

        if(background_)
            background_->destroy(open_gl);
    }
    DBG("openglcomponents destroyed");
}

void SynthSection::destroyOpenGlComponent(OpenGlComponent & open_gl_component, OpenGlWrapper& open_gl)
{
    //moves the component to the end of the array
    //erases it from the vector
    /////TODO: remove this lock can cause opengldeadlocks
    const juce::MessageManagerLock mmLock;
    auto new_logical_end = std::remove_if(open_gl_components_.begin(), open_gl_components_.end(), [&](std::shared_ptr<OpenGlComponent> const& p)
    {
        return *p == open_gl_component;
    });
//    //calls destroy function
//    new_logical_end->get()->destroy(open_gl);
//
//
 auto component = new_logical_end->get();
    component->destroy(open_gl);
    juce::MessageManager::callAsync (
            [new_logical_end,this]()
            {
//                if (auto* comp = safeComp.getComponent())
//                    comp->redoImage();
//                //comp->repaint (cableBounds);
                open_gl_components_.erase(new_logical_end,open_gl_components_.end());
            });
//    open_gl_components_.erase(new_logical_end,open_gl_components_.end());
//

}

void SynthSection::sliderValueChanged(juce::Slider* moved_slider) {
//  std::string name = moved_slider->getName().toStdString();
//  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
//  if (parent)
//    parent->getSynth()->valueChangedInternal(name, moved_slider->getValue());
}

void SynthSection::buttonClicked(juce::Button *clicked_button) {
//  std::string name = clicked_button->getName().toStdString();
//  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
//  if (parent)
//    parent->getSynth()->valueChangedInternal(name, clicked_button->getToggleState() ? 1.0 : 0.0);
}

void SynthSection::guiChanged(SynthButton* button) {
  if (button == activator_)
    setActive(activator_->getToggleStateValue().getValue());
}

void SynthSection::setSliderHasHzAlternateDisplay(SynthSlider* slider) {
//  bitklavier::ValueDetails hz_details = *slider->getDisplayDetails();
//  hz_details.value_scale = bitklavier::ValueDetails::kExponential;
//  hz_details.post_offset = 0.0f;
//  hz_details.display_units = " Hz";
//  hz_details.display_multiply = bitklavier::kMidi0Frequency;
//  slider->setAlternateDisplay(Skin::kFrequencyDisplay, 1.0f, hz_details);
//  slider->setDisplayExponentialBase(pow(2.0f, 1.0f / 12.0f));
}

void SynthSection::addToggleButton(juce::ToggleButton* button, bool show) {
  button_lookup_[button->getName().toStdString()] = button;
  all_buttons_[button->getName().toStdString()] = button;
  button->addListener(this);
  if (show)
    addAndMakeVisible(button);
}

void SynthSection::addButton(OpenGlToggleButton* button, bool show) {
  addToggleButton(button, show);
  addOpenGlComponent(button->getGlComponent());
}

void SynthSection::addButton(OpenGlShapeButton* button, bool show) {
  addToggleButton(button, show);
  addOpenGlComponent(button->getGlComponent());
}



void SynthSection::addSlider(SynthSlider* slider, bool show, bool listen) {
    DBG("adding slider from " + this->getName() + " with name "+ slider->getName());
  slider_lookup_[slider->getName().toStdString()] = slider;
  all_sliders_[slider->getName().toStdString()] = slider;
  all_sliders_v.push_back(slider);
//  if (listen)
//    slider->addListener(this);
  if (show)
    addAndMakeVisible(slider);
  else
      addChildComponent(slider);
 addOpenGlComponent(slider->getImageComponent());
 addOpenGlComponent(slider->getQuadComponent());
 addOpenGlComponent(slider->getTextEditorComponent());
}

void SynthSection::addSubSection(SynthSection* sub_section, bool show) {
    if(sub_section == nullptr)
        return;
  sub_section->setParent(this);

  if (show)
    addAndMakeVisible(sub_section);

  sub_sections_.push_back(sub_section);

  std::map<std::string, SynthSlider*> sub_sliders = sub_section->getAllSliders();
  all_sliders_.insert(sub_sliders.begin(), sub_sliders.end());

  std::map<std::string, juce::ToggleButton*> sub_buttons = sub_section->getAllButtons();
  all_buttons_.insert(sub_buttons.begin(), sub_buttons.end());


}

void SynthSection::removeSubSection(SynthSection* section) {
  auto location = std::find(sub_sections_.begin(), sub_sections_.end(), section);
  if (location != sub_sections_.end())
    sub_sections_.erase(location);
}

void SynthSection::setScrollWheelEnabled(bool enabled) {
//  for (auto& slider : slider_lookup_)
//    slider.second->setScrollEnabled(enabled);
//
//  for (auto& sub_section : sub_sections_)
//    sub_section->setScrollWheelEnabled(enabled);
}

void SynthSection::addBackgroundComponent(OpenGlBackground *open_gl_component, bool to_beginning)
{
    background_ = open_gl_component;

}
void SynthSection::addOpenGlComponent(std::shared_ptr<OpenGlComponent> open_gl_component, bool to_beginning, bool makeVisible ) {
  if (open_gl_component == nullptr)
    return;
  
  _ASSERT(std::find(open_gl_components_.begin(), open_gl_components_.end(),
                         open_gl_component) == open_gl_components_.end());

  open_gl_component->setParent(this);
    if (makeVisible)
        addAndMakeVisible(open_gl_component.get());
    else
        addChildComponent(open_gl_component.get());
  if (to_beginning)
    open_gl_components_.insert(open_gl_components_.begin(), std::move(open_gl_component));
  else
    open_gl_components_.push_back(std::move(open_gl_component));

}



void SynthSection::setActivator(SynthButton* activator) {
  createOffOverlay();

  activator_ = activator;
  activator->setPowerButton();
  activator_->getGlComponent()->setAlwaysOnTop(true);
  activator->addButtonListener(this);
  setActive(activator_->getToggleStateValue().getValue());
}

void SynthSection::createOffOverlay() {
  if (off_overlay_)
    return;

  off_overlay_ = std::make_unique<OffOverlay>();
  addOpenGlComponent(off_overlay_, true);
  off_overlay_->setVisible(false);
  off_overlay_->setAlwaysOnTop(true);
  off_overlay_->setInterceptsMouseClicks(false, false);
}

void SynthSection::paintJointControlSliderBackground(juce::Graphics& g, int x, int y, int width, int height) {
  float rounding = findValue(Skin::kLabelBackgroundRounding);
  g.setColour(findColour(Skin::kTextComponentBackground, true));
  int widget_margin = findValue(Skin::kWidgetMargin);
  int width1 = width / 2;
  int width1_half = width1 / 2;

  g.fillRoundedRectangle(x, y, width1, height, rounding);
  g.fillRect(x + width1 - width1_half, y, width1_half, height);

  g.fillRoundedRectangle(x + width1, y, width1, height, rounding);
  g.fillRect(x + width1, y, width1_half, height);

  g.setColour(findColour(Skin::kLightenScreen, true));
  g.fillRect(x + width1, y + widget_margin, 1, height - 2 * widget_margin);
}

void SynthSection::paintJointControlBackground(juce::Graphics& g, int x, int y, int width, int height) {
  float rounding = findValue(Skin::kLabelBackgroundRounding);
  g.setColour(findColour(Skin::kLabelBackground, true));
  g.fillRect(x + rounding, y * 1.0f, width - 2.0f * rounding, height / 2.0f);

  int label_height = findValue(Skin::kLabelBackgroundHeight);
  int half_label_height = label_height / 2;
  int side_width = height;
  g.setColour(findColour(Skin::kTextComponentBackground, true));
  g.fillRoundedRectangle(x, y + half_label_height, width, height - half_label_height, rounding);
  g.fillRoundedRectangle(x, y, side_width, height, rounding);
  g.fillRoundedRectangle(x + width - side_width, y, side_width, height, rounding);

  juce::Colour label_color = findColour(Skin::kLabelBackground, true);
  if (label_color.getAlpha() == 0)
    label_color = findColour(Skin::kBody, true);
  g.setColour(label_color);
  int rect_width = std::max(width - 2 * side_width, 0);
  g.fillRect(x + side_width, y, rect_width, half_label_height);
  g.fillRoundedRectangle(x + side_width, y, rect_width, label_height, rounding);
}

void SynthSection::paintJointControl(juce::Graphics& g, int x, int y, int width, int height, const std::string& name) {
  paintJointControlBackground(g, x, y, width, height);

  setLabelFont(g);
  g.setColour(findColour(Skin::kBodyText, true));
  g.drawText(name, x, y, width, findValue(Skin::kLabelBackgroundHeight), juce::Justification::centred, false);
}

void SynthSection::placeKnobsInArea(juce::Rectangle<int> area, std::vector<std::unique_ptr<juce::Component>>& knobs) {
  int widget_margin = findValue(Skin::kWidgetMargin);
  //kKnobSectionHeight
  float component_width = (area.getWidth() - (knobs.size() + 1) * widget_margin) / (1.0f * knobs.size());

  int y = area.getY();
  //int height = area.getHeight() - widget_margin;
  int height = std::min<int>(area.getHeight(),component_width) - widget_margin;
  float x = area.getX() + widget_margin;
  for (const auto& knob : knobs) {

    int left = std::round(x);
    int right = std::round(x + component_width);
    //DBG("knob " + juce::String(left));
    if (knob)
      knob->setBounds(left, y, right - left, height);
    x += component_width + widget_margin;
  }
}

void SynthSection::placeKnobsInArea(juce::Rectangle<int> area, std::vector<std::unique_ptr<bitklavier::parameters_view_detail::SliderParameterComponent>>& knobs) {
    int widget_margin = findValue(Skin::kWidgetMargin);
    //kKnobSectionHeight
    float component_width = (area.getWidth() - (knobs.size() + 1) * widget_margin) / (1.0f * knobs.size());

    int y = area.getY();
    //int height = area.getHeight() - widget_margin;
    int height = std::min<int>(area.getHeight(),component_width) - widget_margin;
    float x = area.getX() + widget_margin;
    for (const auto& knob : knobs) {

        int left = std::round(x);
        int right = std::round(x + component_width);
        //DBG("knob " + juce::String(left));
        if (knob)
            knob->slider->setBounds(left, y, right - left, height);
        x += component_width + widget_margin;
    }
}

void SynthSection::lockCriticalSection() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent)
    parent->getSynth()->getCriticalSection().enter();
}

void SynthSection::placeKnobsInArea(juce::Rectangle<int> area, std::vector<juce::Component*> knobs) {
    int widget_margin = findValue(Skin::kWidgetMargin);
    float component_width = (area.getWidth() - (knobs.size() + 1) * widget_margin) / (1.0f * knobs.size());

    int y = area.getY();
    //int height = area.getHeight() - widget_margin;
    int height = std::min<int>(area.getHeight(),component_width) - widget_margin;
    float x = area.getX() + widget_margin;
    for (const auto& knob : knobs) {
        int left = std::round(x);
        int right = std::round(x + component_width);
        if (knob)
            knob->setBounds(left, y, right - left, height);
        x += component_width + widget_margin;
    }
}
void SynthSection::unlockCriticalSection() {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent)
    parent->getSynth()->getCriticalSection().exit();
}

float SynthSection::getTitleWidth() {
  return findValue(Skin::kTitleWidth);
}

float SynthSection::getKnobSectionHeight() {
    return findValue(Skin::kKnobSectionHeight);
}

float SynthSection::getSliderWidth() {
  return findValue(Skin::kSliderWidth);
}



//float SynthSection::getSliderOverlapWithSpace() {
//  return getSliderOverlap() - (int)getWidgetMargin();
//}

float SynthSection::getTextComponentHeight() {
  return findValue(Skin::kTextComponentHeight);
}


float SynthSection::getPadding() {
  return findValue(Skin::kPadding);
}

//float SynthSection::getTextSectionYOffset() {
//  return (getKnobSectionHeight() - getTextComponentHeight()) / 2.0f;
//}



float SynthSection::getWidgetMargin() {
  return findValue(Skin::kWidgetMargin);
}

float SynthSection::getWidgetRounding() {
  return findValue(Skin::kWidgetRoundedCorner);
}

juce::Rectangle<int> SynthSection::getPresetBrowserBounds() {
  static constexpr float kXPercent = 0.4f;
  int title_width = getTitleWidth();
  int widget_margin = getWidgetMargin();
  int width = getWidth();
  int x = width * kXPercent;
  if (preset_selector_half_width_)
    x = width * 0.7f + findValue(Skin::kWidgetMargin);
  return juce::Rectangle<int>(x, widget_margin, width - x - widget_margin, title_width - 2 * widget_margin);
}

int SynthSection::getTitleTextRight() {
  if (preset_selector_ == nullptr)
    return getWidth();

  if (preset_selector_half_width_)
    return getWidth() * 0.2f;
  return getPresetBrowserBounds().getX();
}

juce::Rectangle<int> SynthSection::getPowerButtonBounds() {
  int title_width = getTitleWidth();
  return juce::Rectangle<int>(getPowerButtonOffset(), 0, title_width, title_width);
}

juce::Rectangle<int> SynthSection::getTitleBounds() {
  int title_width = getTitleWidth();
  int from = 0;
  if (activator_)
    from = getPowerButtonBounds().getRight() - title_width * kPowerButtonPaddingPercent;

  int to = getTitleTextRight();
  return juce::Rectangle<int>(from, 0, to - from, title_width);
}

float SynthSection::getDisplayScale() const {
  if (getWidth() <= 0)
    return 1.0f;
  
  juce::Component* top_level = getTopLevelComponent();
  juce::Rectangle<int> global_bounds = top_level->getLocalArea(this, getLocalBounds());
  float display_scale = juce::Desktop::getInstance().getDisplays().getDisplayForRect(top_level->getScreenBounds())->scale;
  return 1;//display_scale * (1.0f * global_bounds.getWidth()) / getWidth();
}

int SynthSection::getPixelMultiple() const {
  if (parent_)
    return parent_->getPixelMultiple();
  return 1.0f;
}

juce::Font SynthSection::getLabelFont() {
  float height = findValue(Skin::kLabelHeight);
  return Fonts::instance()->proportional_regular().withPointHeight(height);
}

void SynthSection::setLabelFont(juce::Graphics& g) {
  g.setColour(findColour(Skin::kBodyText, true));
  g.setFont(getLabelFont());
}



void SynthSection::drawLabelBackground(juce::Graphics& g, juce::Rectangle<int> bounds, bool text_component) {
  int background_rounding = findValue(Skin::kLabelBackgroundRounding);
  g.setColour(findColour(Skin::kLabelBackground, true));
  juce::Rectangle<float> label_bounds = getLabelBackgroundBounds(bounds, text_component).toFloat();
  g.fillRoundedRectangle(label_bounds, background_rounding);
  if (text_component && !findColour(Skin::kTextComponentBackground, true).isTransparent())
    g.fillRect(label_bounds.withHeight(label_bounds.getHeight() / 2));
}

void SynthSection::drawLabelBackgroundForComponent(juce::Graphics& g, juce::Component* component) {
  drawLabelBackground(g, component->getBounds());
}

juce::Rectangle<int> SynthSection::getDividedAreaUnbuffered(juce::Rectangle<int> full_area, int num_sections,
                                                      int section, int buffer) {
  float component_width = (full_area.getWidth() - (num_sections + 1) * buffer) / (1.0f * num_sections);
  int x = full_area.getX() + std::round(section * (component_width + buffer) + buffer);
  int right = full_area.getX() + std::round((section + 1.0f) * (component_width + buffer));
  return juce::Rectangle<int>(x, full_area.getY(), right - x, full_area.getHeight());
}

juce::Rectangle<int> SynthSection::getDividedAreaBuffered(juce::Rectangle<int> full_area, int num_sections,
                                                    int section, int buffer) {
  juce::Rectangle<int> area = getDividedAreaUnbuffered(full_area, num_sections, section, buffer);
  return area.expanded(buffer, 0);
}

juce::Rectangle<int> SynthSection::getLabelBackgroundBounds(juce::Rectangle<int> bounds, bool text_component) {
  int background_height = findValue(Skin::kLabelBackgroundHeight);
  int label_offset = text_component ? findValue(Skin::kTextComponentLabelOffset) : findValue(Skin::kLabelOffset);
  int background_y = bounds.getBottom() - background_height + label_offset;
  return juce::Rectangle<int>(bounds.getX(), background_y, bounds.getWidth(), background_height);
}

void SynthSection::drawLabel(juce::Graphics& g, juce::String text, juce::Rectangle<int> component_bounds, bool text_component) {
  if (component_bounds.getWidth() <= 0 || component_bounds.getHeight() <= 0)
    return;

  drawLabelBackground(g, component_bounds, text_component);
  g.setColour(findColour(Skin::kBodyText, true));
  juce::Rectangle<int> background_bounds = getLabelBackgroundBounds(component_bounds, text_component);
  g.drawText(text, component_bounds.getX(), background_bounds.getY(),
                   component_bounds.getWidth(), background_bounds.getHeight(), juce::Justification::centred, false);
}

void SynthSection::drawTextBelowComponent(juce::Graphics& g, juce::String text, juce::Component* component, int space, int padding) {
  int height = findValue(Skin::kLabelBackgroundHeight);
  g.drawText(text, component->getX() - padding, component->getBottom() + space,
             component->getWidth() + 2 * padding, height, juce::Justification::centred, false);
}

void SynthSection::setActive(bool active) {
  if (active_ == active)
    return;

  if (off_overlay_)
    off_overlay_->setVisible(!active);

  active_ = active;


  repaintBackground();
}

void SynthSection::animate(bool animate) {
  for (auto& sub_section : sub_sections_)
    sub_section->animate(animate);
}
void SynthSection::showPopupDisplay(juce::Component* source, const std::string& text,
    juce::BubbleComponent::BubblePlacement placement, bool primary) {
  FullInterface* parent = findParentComponentOfClass<FullInterface>();
  if (parent)
    parent->popupDisplay(source, text, placement, primary);
}

void SynthSection::hidePopupDisplay(bool primary) {
  FullInterface* parent = findParentComponentOfClass<FullInterface>();
  if (parent)
    parent->hideDisplay(primary);
}
void SynthSection::showPopupSelector(juce::Component* source, juce::Point<int> position, const PopupItems& options,
    std::function<void(int)> callback, std::function<void()> cancel) {
  FullInterface* parent = findParentComponentOfClass<FullInterface>();
  if (parent)
    parent->popupSelector(source, position, options, callback, cancel);
}



//void SynthSection::setValue(const std::string& name, bitklavier::mono_float value, NotificationType notification) {
////  if (all_sliders_.count(name)) {
////    all_sliders_[name]->setValue(value, notification);
////    if (notification == dontSendNotification)
////      all_sliders_[name]->redoImage();
////    all_sliders_[name]->notifyGuis();
////  }
////  if (all_buttons_.count(name))
////    all_buttons_[name]->setToggleState(value, notification);
//}


void SynthSection::showPrepPopup(PreparationSection* prep)
{
    DBG("show prep popup");
    FullInterface* parent = findParentComponentOfClass<FullInterface>();
    if (parent)
        parent->prepDisplay(prep);
}
