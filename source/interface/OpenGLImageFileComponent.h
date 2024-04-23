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

#include "open_gl_component.h"
#include "look_and_feel/fonts.h"
#include "open_gl_image.h"

class SynthSection;

class OpenGlImageFileComponent : public OpenGlComponent {
public:
   OpenGlImageFileComponent(String name = "");
   virtual ~OpenGlImageFileComponent() = default;

   virtual void paintBackground(Graphics& g) override {
       redrawImage(false);
   }

   virtual void paintToImage(Graphics& g) {
       Component* component = component_ ? component_ : this;
       if (paint_entire_component_)
           component->paintEntireComponent(g, false);
       else
           component->paint(g);
   }

   virtual void init(OpenGlWrapper& open_gl) override;
   virtual void render(OpenGlWrapper& open_gl, bool animate) override;
   virtual void destroy(OpenGlWrapper& open_gl) override;

   virtual void redrawImage(bool force);
   void setComponent(Component* component) { component_ = component; }
   void setScissor(bool scissor) { image_.setScissor(scissor); }
   void setUseAlpha(bool use_alpha) { image_.setUseAlpha(use_alpha); }
   void setColor(Colour color) { image_.setColor(color); }
   OpenGlImage& image() { return image_; }
   void setActive(bool active) { active_ = active; }
   void setStatic(bool static_image) { static_image_ = static_image; }
   void paintEntireComponent(bool paint_entire_component) { paint_entire_component_ = paint_entire_component; }
   bool isActive() const { return active_; }

   void setOwnImage(Image image) {
      image_.setOwnImage(image);
      image_height_ = image_.getImageHeight();
      image_width_ = image_.getImageWidth();
   }

   void setImage(Image* image) {
       image_.setImage(image);
   }
   int image_width_;
   int image_height_;
protected:

   std::mutex mutex_;
   Component* component_;
   bool active_;
   bool static_image_;
   bool paint_entire_component_;
   //std::unique_ptr<Image> draw_image_;
   OpenGlImage image_;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlImageFileComponent)
};


