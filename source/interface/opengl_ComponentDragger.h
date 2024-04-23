//
// Created by Davis Polito on 4/23/24.
//

#ifndef BITKLAVIER2_OPENGL_COMPONENTDRAGGER_H
#define BITKLAVIER2_OPENGL_COMPONENTDRAGGER_H
#include "juce_gui_basics/juce_gui_basics.h"
class openGLComponentDragger
{

public:
    openGLComponentDragger() : size_ratio_(1.f)
    {

    }

    void startDraggingComponent (juce::Component* const componentToDrag, const juce::MouseEvent& e)
    {
        jassert (componentToDrag != nullptr);
        jassert (e.mods.isAnyMouseButtonDown()); // The event has to be a drag event!

        if (componentToDrag != nullptr)
        {
            mouseDownWithinTarget = e.getEventRelativeTo (componentToDrag).getMouseDownPosition();
            mouseDownWithinTarget = mouseDownWithinTarget * size_ratio_;
           // mouseDownWithinTarget.setXY(mouseDownWithinTarget.getX() * size_ratio_,mouseDownWithinTarget.getY() * size_ratio_ );
        }

    }

    void dragComponent (juce::Component* const componentToDrag, const juce::MouseEvent& e,
        juce::ComponentBoundsConstrainer* const constrainer)
    {
        jassert (componentToDrag != nullptr);
        jassert (e.mods.isAnyMouseButtonDown()); // The event has to be a drag event!

        if (componentToDrag != nullptr)
        {
            auto bounds = componentToDrag->getBounds();

            // If the component is a window, multiple mouse events can get queued while it's in the same position,
            // so their coordinates become wrong after the first one moves the window, so in that case, we'll use
            // the current mouse position instead of the one that the event contains...
            if (componentToDrag->isOnDesktop())
                bounds += componentToDrag->getLocalPoint (nullptr, e.source.getScreenPosition()).roundToInt() * size_ratio_ - mouseDownWithinTarget;
            else
                bounds += e.getEventRelativeTo (componentToDrag).getPosition() * size_ratio_ - mouseDownWithinTarget;

            if (constrainer != nullptr)
                constrainer->setBoundsForComponent (componentToDrag, bounds, false, false, false, false);
            else
                componentToDrag->setBounds (bounds);
        }
    }
    float size_ratio_;
private :
    juce::Point<int> mouseDownWithinTarget;
};

#endif //BITKLAVIER2_OPENGL_COMPONENTDRAGGER_H
