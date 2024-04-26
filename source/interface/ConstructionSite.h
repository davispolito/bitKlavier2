//
// Created by Davis Polito on 1/17/24.
//

#ifndef BITKLAVIER2_CONSTRUCTIONSITE_H
#define BITKLAVIER2_CONSTRUCTIONSITE_H
#include  "draggable_component.h"
#include "common.h"
#include "Preparations/PreparationSection.h"
class SynthGuiInterface;
typedef Loki::Factory<PreparationSection, int,  juce::ValueTree,  juce::UndoManager&> PreparationFactory;
class ConstructionSite : public LassoSource<BKItem*>, public SynthSection, public juce::ValueTree::Listener,
                         public tracktion::engine::ValueTreeObjectList<PreparationSection>,private KeyListener, public DragAndDropContainer
{
public:
    ConstructionSite(juce::ValueTree &v, juce::UndoManager &um, OpenGlWrapper &open_gl);

    ~ConstructionSite(void);

    void redraw(void);

    //void paint(Graphics& g) override;

    bool keyPressed(const KeyPress &k, Component *c) override;
    void itemIsBeingDragged(BKItem* thisItem, const MouseEvent& e);

    void paintBackground(Graphics& g) override;



    bool inPaste, inCopyDrag, inCut;






    void addItem(bitklavier::BKPreparationType type, bool center = false);


    inline void setCurrentItem(BKItem* item) { currentItem = item;}
    inline BKItem* getCurrentItem(void) { return currentItem;}



    bool altDown;






    struct TouchEvent
    {
        TouchEvent (const MouseInputSource& ms)
            : source (ms)
        {}

        void pushPoint (juce::Point<float> newPoint, ModifierKeys newMods)
        {
            currentPosition = newPoint;
            modifierKeys = newMods;

            if (lastPoint.getDistanceFrom (newPoint) > 5.0f)
            {
                if (lastPoint != juce::Point<float>())
                {
                    Path newSegment;
                    newSegment.startNewSubPath (lastPoint);
                    newSegment.lineTo (newPoint);

                    PathStrokeType (1.0f, PathStrokeType::curved, PathStrokeType::rounded).createStrokedPath (newSegment, newSegment);
                    path.addPath (newSegment);

                }

                lastPoint = newPoint;
            }
        }

        MouseInputSource source;
        Path path;
        juce::Point<float> lastPoint, currentPosition;
        ModifierKeys modifierKeys;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TouchEvent)
    };

    void drawTouch (TouchEvent& touch, Graphics& g)
    {
        g.setColour (Colours::antiquewhite);
        g.fillPath (touch.path);

        const float radius = 10.0f;

        g.drawEllipse (touch.currentPosition.x - radius,
            touch.currentPosition.y - radius,
            radius * 2.0f, radius * 2.0f, 2.0f);

        g.setFont (14.0f);

        String desc ("Mouse #");
        desc << touch.source.getIndex();

        float pressure = touch.source.getCurrentPressure();

        if (pressure > 0.0f && pressure < 1.0f)
            desc << "  (pressure: " << (int) (pressure * 100.0f) << "%)";

        if (touch.modifierKeys.isCommandDown()) desc << " (CMD)";
        if (touch.modifierKeys.isShiftDown())   desc << " (SHIFT)";
        if (touch.modifierKeys.isCtrlDown())    desc << " (CTRL)";
        if (touch.modifierKeys.isAltDown())     desc << " (ALT)";

        g.drawText (desc,
            Rectangle<int> ((int) touch.currentPosition.x - 200,
                (int) touch.currentPosition.y - 60,
                400, 20),
            Justification::centredTop, false);
    }

    inline int getNumberOfTouches(void)
    {
        return touches.size();
    }

    bool edittingComment;

    OwnedArray<HashMap<int,int>> pastemap;
    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::PREPARATION);
    }

    PreparationSection* createNewObject(const juce::ValueTree& v) override;
    void deleteObject (PreparationSection* at) override
    {
        delete at;
    }


    void newObjectAdded (PreparationSection*) override    { resized();} //resized(); }
    void objectRemoved (PreparationSection*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }

//PreparationSection * preparation;
    juce::Viewport* view;
    Point<float> mouse;
private:
    //std::unique_ptr<PreparationList> prepList;
    SynthGuiInterface* _parent;
    OwnedArray<TouchEvent> touches;
    OpenGlWrapper &open_gl;
    TouchEvent* getTouchEvent (const MouseInputSource& source)
    {
        for (int i = 0; i < touches.size(); ++i)
        {
            TouchEvent* t = touches.getUnchecked(i);

            if (t->source == source)
                return t;
        }

        return nullptr;
    }



    int leftMost, rightMost, topMost, bottomMost;


    bool connect;

    juce::Point<int> lastPosition;

    int lastX, lastY;
    int lastEX,lastEY;

    int lineOX, lineOY, lineEX, lineEY;
    bool multiple;
    bool held;



    BKItem* itemSource;
    BKItem* itemTarget;
    BKItem* itemToSelect;
    BKItem* lastItem;
    BKItem* currentItem;
    BKItem* upperLeftest;

    std::unique_ptr<LassoComponent<BKItem*>> lasso;
    bool inLasso;
    SelectedItemSet<BKItem*> lassoSelection;
    ValueTree state;
    UndoManager &undo;
    void findLassoItemsInArea (Array <BKItem*>& itemsFound,
        const Rectangle<int>& area) override;

    SelectedItemSet<BKItem*>& getLassoSelection(void) override;

    void draw(void);

    void prepareItemDrag(BKItem* item, const MouseEvent& e, bool center);

    void resized() override;

    void mouseDown (const MouseEvent& eo) override;

    void mouseUp (const MouseEvent& eo) override;

    void mouseDrag (const MouseEvent& e) override;

    void mouseMove (const MouseEvent& e) override;

    void deleteItem (BKItem* item);

    BKItem* getItemAtPoint(const int X, const int Y);


    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override
    {
        //        if (isSuitableType (v))
        //            if (i == IDs::start || i == IDs::length)
        //                resized();

        tracktion::engine::ValueTreeObjectList<PreparationSection>::valueTreePropertyChanged (v, i);
    }
    //ReferenceCountedArray<PreparationSection,

    PreparationFactory prepFactory;

    JUCE_LEAK_DETECTOR(ConstructionSite)
};


#endif //BITKLAVIER2_CONSTRUCTIONSITE_H
