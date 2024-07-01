//
// Created by Davis Polito on 1/17/24.
//

#ifndef BITKLAVIER2_CONSTRUCTIONSITE_H
#define BITKLAVIER2_CONSTRUCTIONSITE_H
#include  "draggable_component.h"
#include "common.h"
#include "Preparations/PreparationSection.h"
#include "PreparationSelector.h"
#include "Cable/Cable.h"
class SynthGuiInterface;
typedef Loki::Factory<PreparationSection, int,  juce::ValueTree,  OpenGlWrapper&> PreparationFactory;
class ConstructionSite : public SynthSection, public juce::ValueTree::Listener,
                         public tracktion::engine::ValueTreeObjectList<PreparationSection>,private KeyListener,
                         public DragAndDropContainer

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


    bool altDown;










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


    void newObjectAdded (PreparationSection*) override;
    void objectRemoved (PreparationSection*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }

//PreparationSection * preparation;
    juce::Viewport* view;
   juce::Point<float> mouse;
private:
    //std::unique_ptr<PreparationList> prepList;
    SynthGuiInterface* _parent;

    OpenGlWrapper &open_gl;



    int leftMost, rightMost, topMost, bottomMost;


    bool connect;

    juce::Point<int> lastPosition;

    int lastX, lastY;
    int lastEX,lastEY;

    int lineOX, lineOY, lineEX, lineEY;
    bool multiple;
    bool held;



    PreparationSelector preparationSelector;
    LassoComponent<PreparationSection*> selectorLasso;


    //SelectedItemSet<PreparationSection*> lassoSelection;
    ValueTree state;
    UndoManager &undo;

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
    //std::shared_ptr<juce::AudioProcessor> last_proc;
 //    std::vector<chowdsp::Broadcaster<void()>> audioThreadBroadcasters { 10 };
//    using AudioThreadAction = juce::dsp::FixedSizeFunction<actionSize, void()>;
//    moodycamel::ReaderWriterQueue<AudioThreadAction> audioThreadBroadcastQueue { 10 };

    JUCE_LEAK_DETECTOR(ConstructionSite)
};


#endif //BITKLAVIER2_CONSTRUCTIONSITE_H
