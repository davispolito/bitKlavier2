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

#include "open_gl_image_component.h"
#include "utils.h"
OpenGlImageComponent::OpenGlImageComponent(juce::String name) : OpenGlComponent(name + "_image"), component_(nullptr),
                                                          active_(true), static_image_(false),
                                                          paint_entire_component_(true) {
   image_.setTopLeft(-1.0f, 1.0f);
   image_.setTopRight(1.0f, 1.0f);
   image_.setBottomLeft(-1.0f, -1.0f);
   image_.setBottomRight(1.0f, -1.0f);
   image_.setColor(juce::Colours::white);

   if (name == "")
       setInterceptsMouseClicks(false, false);
}

void OpenGlImageComponent::redrawImage(bool force, bool clear) {
    if (!active_)
        return;
    Component* component = component_ ? component_ : this;

    float pixel_scale =  juce::Desktop::getInstance().getDisplays().getDisplayForRect(getScreenBounds())->scale;
    int width = component->getWidth() * pixel_scale;
    int height = component->getHeight() * pixel_scale;
    float val = getApproximateScaleFactorForComponent(component);
    DBG("approx scale " + juce::String(val));
    DBG("pixel scale" + juce::String(pixel_scale) );
    if (width <= 0 || height <= 0)
        return;

    bool new_image = draw_image_ == nullptr || draw_image_->getWidth() != width || draw_image_->getHeight() != height;
    if (!new_image && (static_image_ || !force))
        return;

    image_.lock();

    if (new_image)
        draw_image_ = std::make_unique<juce::Image>(juce::Image::ARGB, width, height, false);

    draw_image_->clear(juce::Rectangle<int>(0, 0, width, height));
    juce::Graphics g(*draw_image_);
    g.addTransform(juce::AffineTransform::scale(pixel_scale));
    paintToImage(g);
    image_.setImage(draw_image_.get());

    auto gl_width = (float)bitklavier::utils::nextPowerOfTwo(width);
    auto gl_height = (float)bitklavier::utils::nextPowerOfTwo(height);
    float width_ratio = gl_width / (float)width;
    float height_ratio = gl_height / (float)height;
    float right = -1.0f + pixel_scale * width_ratio;
    float bottom = 1.0f - pixel_scale * height_ratio;
//    image_.setTopRight(right, 1.0f);
//    image_.setBottomLeft(-1.0f, bottom);
//    image_.setBottomRight(right, bottom);
    image_.unlock();
}

void OpenGlImageComponent::init(OpenGlWrapper& open_gl) {
   image_.init(open_gl);
}

bool OpenGlImageComponent::isInit()
{
    return image_.shader() != nullptr;
}

void OpenGlImageComponent::render(OpenGlWrapper& open_gl, bool animate) {
   juce::Component* component = component_ ? component_ : this;
   if (!active_ || !setViewPort(component, open_gl) || !component->isVisible())
       return;

   image_.drawImage(open_gl);
}

void OpenGlImageComponent::destroy(OpenGlWrapper& open_gl) {
   image_.destroy(open_gl);
}
