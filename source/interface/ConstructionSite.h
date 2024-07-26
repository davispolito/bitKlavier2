//
// Created by Davis Polito on 1/17/24.
//

#ifndef BITKLAVIER2_CONSTRUCTIONSITE_H
#define BITKLAVIER2_CONSTRUCTIONSITE_H
#include  "draggable_component.h"
#include "common.h"
#include "Preparations/PreparationSection.h"
#include "PreparationSelector.h"
#include "Cable/CableView.h"
class SynthGuiInterface;
typedef Loki::Factory<PreparationSection, int,  juce::ValueTree,  OpenGlWrapper&> PreparationFactory;
class ConstructionSite : public SynthSection,
                         public tracktion::engine::ValueTreeObjectList<PreparationSection>,private KeyListener,
                         public DragAndDropContainer

{
public:
    ConstructionSite(juce::ValueTree &v, juce::UndoManager &um, OpenGlWrapper &open_gl);

    ~ConstructionSite(void);

    void redraw(void);

    bool keyPressed(const KeyPress &k, Component *c) override;
    void itemIsBeingDragged(BKItem* thisItem, const MouseEvent& e);

    void paintBackground(Graphics& g) override;

    void addItem(bitklavier::BKPreparationType type, bool center = false);


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
    BKPort* findPinAt (juce::Point<float> pos) const
    {
        for (auto* fc : objects)
        {
            // NB: A Visual Studio optimiser error means we have to put this Component* in a local
            // variable before trying to cast it, or it gets mysteriously optimised away..
            auto* comp = fc->getComponentAt (pos.toInt() - fc->getPosition());

            if (auto* pin = dynamic_cast<BKPort*> (comp))
                return pin;
        }

        return nullptr;
    }
   PreparationSection* getComponentForPlugin (AudioProcessorGraph::NodeID nodeID) const
    {
        for (auto* fc : objects)
            if (fc->pluginID == nodeID)
                return fc;

        return nullptr;
    }
    juce::Viewport* view;
   juce::Point<float> mouse;
    OpenGlWrapper &open_gl;
private:

    SynthGuiInterface* _parent;




    bool edittingComment;

    OwnedArray<HashMap<int,int>> pastemap;
    friend class CableView;



    bool connect;

    int lastX, lastY;

    bool multiple;
    bool held;



    PreparationSelector preparationSelector;
    LassoComponent<PreparationSection*> selectorLasso;

    friend class CableView;
    CableView cableView;
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
        tracktion::engine::ValueTreeObjectList<PreparationSection>::valueTreePropertyChanged (v, i);
    }

    PreparationFactory prepFactory;

    JUCE_LEAK_DETECTOR(ConstructionSite)
};


#endif //BITKLAVIER2_CONSTRUCTIONSITE_H
