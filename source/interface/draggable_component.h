//
// Created by Davis Polito on 1/17/24.
//

#ifndef BITKLAVIER2_DRAGGABLE_COMPONENT_H
#define BITKLAVIER2_DRAGGABLE_COMPONENT_H
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "open_gl_image_component.h"
#include "common.h"
#include "synth_section.h"
#include "common.h"
class DraggableComponent    : public juce::Component, public juce::DragAndDropTarget
{
public:

    DraggableComponent(bool isDrag, bool isDrop, bool isConn):
                                                                   isDraggable(isDrag),
                                                                   isDroppable(isDrop),
                                                                   isConnectable(isConn)
    {
        constrainer.setMinimumOnscreenAmounts(50,50,50,50);
        addMouseListener(this,true);
    }

    DraggableComponent(bool isDrag, bool isDrop, bool isConn,
        int top, int left, int bottom, int right):
                     isDraggable(isDrag),
                     isDroppable(isDrop),
                     isConnectable(isConn)
    {
        constrainer.setMinimumOnscreenAmounts(top,left,bottom,right);
        addMouseListener(this,true);
    }

    ~DraggableComponent()
    {
        removeMouseListener(this);
    }

    inline void setConstrainer(int top, int left, int bottom, int right)
    {

    }

    //virtual void itemWasDropped(BKPreparationType type, Array<int>, int x, int y){};
    virtual void itemIsBeingDragged(const juce::MouseEvent& e){};
    virtual void itemWasDragged(const juce::MouseEvent& e){};
    virtual void keyPressedWhileSelected(const juce::KeyPress& e) {};

    bool isDragging;

    void prepareDrag(const juce::MouseEvent& e)
    {
        isDragging = true;

        dragger.startDraggingComponent (this, e);
    }

    void performDrag(const juce::MouseEvent& e)
    {
        dragger.dragComponent (this, e, &constrainer);

        itemIsBeingDragged(e);
    }

protected:
    bool itemIsHovering, isSelected;

    bool isDraggable, isDroppable, isConnectable;

private:
    // Drag interface
    bool isInterestedInDragSource (const SourceDetails& /*dragSourceDetails*/) override
    {
        // should check drag source to see if its what we want...
        return true;
    }

    void itemDragEnter (const SourceDetails& /*dragSourceDetails*/) override
    {
        itemIsHovering = true;
        if (isDroppable) repaint();
    }

    void itemDragMove (const SourceDetails& /*dragSourceDetails*/) override
    {

    }

    void itemDragExit (const SourceDetails& /*dragSourceDetails*/) override
    {
        itemIsHovering = false;
        if (isDroppable) repaint();
    }

    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        juce::String received = dragSourceDetails.description.toString();
        DBG("Items dropped: " + received);

        //juce::Array<int> data = stringToIntArray(received);
        //BKPreparationType type = (BKPreparationType)data[0];
        //data.remove(0);

        juce::Point<int> where = dragSourceDetails.localPosition;

        if (isDroppable)
        {
            //itemWasDropped(type, data, where.getX(), where.getY());

            itemIsHovering = false;

            repaint();
        }
    }

    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;

    juce::Point<int> startPosition;
    juce::Point<int> vector;


    JUCE_LEAK_DETECTOR (DraggableComponent)
};


class BKItem : /*public DraggableComponent,*/ public Button
{
public:
    BKItem( bitklavier::BKPreparationType type);
    void mouseDown(const MouseEvent& e) override;
    void mouseDoubleClick(const MouseEvent& e) override;

   // void paint(Graphics& g) override;

    void resized() override;

    //void itemIsBeingDragged(const MouseEvent&) override;
    void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown);
    //void paintBackground(Graphics& g) override;
    //void parentHierarchyChanged() override;

    class Listener
    {
    public:
        virtual ~Listener() = default;
    };

    OpenGlImageComponent* getImageComponent() { return &image_component_; }
    void redoImage() { image_component_.redrawImage(true); }
private :

    //void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override;
//    juce::ValueTree &state;
//    juce::UndoManager &um;
    Image shadow_;
    std::vector<Listener*> listeners_;
    OpenGlImageComponent image_component_;
    juce::Path icon_;
    bool wasJustDragged;
   // std::unique_ptr<OpenGlImageComponent> background_;

};


#endif //BITKLAVIER2_DRAGGABLE_COMPONENT_H
