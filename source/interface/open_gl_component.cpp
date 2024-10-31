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

#include "open_gl_component.h"

#include "open_gl_multi_quad.h"
#include "fullInterface.h"


namespace {
  juce::Rectangle<int> getGlobalBounds(juce::Component* component, juce::Rectangle<int> bounds) {
    juce::Component* parent = component->getParentComponent();
    while (parent && dynamic_cast<FullInterface*>(component) == nullptr) {
      bounds = bounds + component->getPosition();
      component = parent;
      parent = component->getParentComponent();
    }

    return bounds;
  }

  juce::Rectangle<int> getGlobalVisibleBounds(juce::Component* component, juce::Rectangle<int> visible_bounds) {
    juce::Component* parent = component->getParentComponent();
    while (parent && dynamic_cast<FullInterface*>(parent) == nullptr) {
      visible_bounds = visible_bounds + component->getPosition();
      parent->getLocalBounds().intersectRectangle(visible_bounds);
      component = parent;
      parent = component->getParentComponent();
    }

    return visible_bounds + component->getPosition();
  }
}

int OpenGlComponent::nID = 0;
OpenGlComponent::OpenGlComponent(juce::String name) : juce::Component(name), only_bottom_corners_(false),
                                                parent_(nullptr), skin_override_(Skin::kNone)
                                                {
  background_color_ = juce::Colours::transparentBlack;
  id = generateID();
}

OpenGlComponent::~OpenGlComponent() {
}

bool OpenGlComponent::setViewPort(Component* component, juce::Rectangle<int> bounds, OpenGlWrapper& open_gl) {
    FullInterface* top_level = component->findParentComponentOfClass<FullInterface>();
    //DBG("----" + component->getName() + "-----");
    if(top_level == nullptr)
        return false;
    float scale = open_gl.display_scale;
    float resize_scale = top_level->getResizingScale();
//  float render_scale = 1.0f;
//  if (scale == 1.0f)
    float render_scale = open_gl.context.getRenderingScale();
    //DBG("resize_scale " + juce::String(resize_scale));
    float gl_scale = render_scale * resize_scale;
    float comp_scale = scale* Component::getApproximateScaleFactorForComponent(component);
    auto top_level_bounds = top_level->getBounds();
    auto global_bounds = getGlobalBounds(component, bounds);
    auto visible_bounds = getGlobalVisibleBounds(component,  bounds);
    juce::gl::glViewport(comp_scale * global_bounds.getX(),
                         std::ceil(comp_scale * top_level_bounds.getHeight()) - comp_scale * global_bounds.getBottom(),
                         comp_scale * global_bounds.getWidth(), comp_scale  * global_bounds.getHeight());
    //DBG("gl scale" + juce::String(gl_scale));
    //DBG("x" + juce::String(global_bounds.getX()));
    //DBG("Y pos" + juce::String(std::ceil(render_scale * top_level_bounds.getHeight())) +   " - "  + juce::String(gl_scale * global_bounds.getBottom()) + " = "
    //    + juce::String(std::ceil(render_scale * top_level_bounds.getHeight()) - gl_scale * global_bounds.getBottom()));
    //DBG(comp_scale);

    if (visible_bounds.getWidth() <= 0 || visible_bounds.getHeight() <= 0)
        return false;
    juce::gl::glScissor(comp_scale * visible_bounds.getX(),
                        std::ceil( comp_scale * top_level_bounds.getHeight()) - comp_scale * visible_bounds.getBottom(),
                        comp_scale * visible_bounds.getWidth(), comp_scale * visible_bounds.getHeight());
    return true;
}

bool OpenGlComponent::setViewPort(juce::Component* component, OpenGlWrapper& open_gl) {
  return setViewPort(component, component->getLocalBounds(), open_gl);
}

bool OpenGlComponent::setViewPort(OpenGlWrapper& open_gl) {
  return setViewPort(this, open_gl);
}

void OpenGlComponent::setScissor(juce::Component* component, OpenGlWrapper& open_gl) {
  setScissorBounds(component, component->getLocalBounds(), open_gl);
}

void OpenGlComponent::setScissorBounds(juce::Component* component, juce::Rectangle<int> bounds, OpenGlWrapper& open_gl) {
    if (component == nullptr)
        return;

    FullInterface* top_level = component->findParentComponentOfClass<FullInterface>();
    float scale = open_gl.display_scale;
    float resize_scale = top_level->getResizingScale();
//  float render_scale = 1.0f;
//  if (scale == 1.0f)
    float render_scale = open_gl.context.getRenderingScale();

    float gl_scale = render_scale * resize_scale;

    auto top_level_bounds = top_level->getBounds();
    auto visible_bounds = getGlobalVisibleBounds(component, bounds);
    float comp_scale = Component::getApproximateScaleFactorForComponent(component);
    if (visible_bounds.getHeight() > 0 && visible_bounds.getWidth() > 0) {
        juce::gl::glScissor(comp_scale * visible_bounds.getX(),
                            std::ceil(comp_scale * top_level_bounds.getHeight()) - comp_scale * visible_bounds.getBottom(),
                            comp_scale * visible_bounds.getWidth(), comp_scale * visible_bounds.getHeight());
    }
}

void OpenGlComponent::paintBackground(juce::Graphics& g) {
  if (!isVisible())
    return;

  g.fillAll(findColour(Skin::kWidgetBackground, true));
}

void OpenGlComponent::repaintBackground() {
  if (!isShowing())
    return;

  FullInterface* parent = findParentComponentOfClass<FullInterface>();
  if (parent)
    parent->repaintOpenGlBackground(this);
}

void OpenGlComponent::resized() {
  if (corners_)
    corners_->setBounds(getLocalBounds());

  body_color_ = findColour (Skin::kBody, true);
}

void OpenGlComponent::parentHierarchyChanged() {
//  if (num_voices_readout_ == nullptr) {
//    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
//    if (parent)
//      num_voices_readout_ = parent->getSynth()->getStatusOutput("num_voices");
//  }

  juce::Component::parentHierarchyChanged();
}

void OpenGlComponent::addRoundedCorners() {
  corners_ = std::make_unique<OpenGlCorners>();
  addAndMakeVisible(corners_.get());
}

void OpenGlComponent::addBottomRoundedCorners() {
  only_bottom_corners_ = true;
  addRoundedCorners();
}

void OpenGlComponent::init(OpenGlWrapper& open_gl) {
  if (corners_)
    corners_->init(open_gl);
}

void OpenGlComponent::renderCorners(OpenGlWrapper& open_gl, bool animate, juce::Colour color, float rounding) {
  if (corners_) {
    if (only_bottom_corners_)
      corners_->setBottomCorners(getLocalBounds(), rounding);
    else
      corners_->setCorners(getLocalBounds(), rounding);
    corners_->setColor(color);
    corners_->render(open_gl, animate);
  }
}

void OpenGlComponent::renderCorners(OpenGlWrapper& open_gl, bool animate) {
  renderCorners(open_gl, animate, body_color_, findValue(Skin::kWidgetRoundedCorner));
}

void OpenGlComponent::destroy(OpenGlWrapper& open_gl) {
  if (corners_)
    corners_->destroy(open_gl);
}

bool OpenGlComponent::isInit() {
    if (corners_)
        return corners_->shader() != nullptr;
}

float OpenGlComponent::findValue(Skin::ValueId value_id) {
  if (parent_)
    return parent_->findValue(value_id);

  _ASSERT(false);
  return 0.0f;
}
