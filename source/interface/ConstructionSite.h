//
// Created by Davis Polito on 1/17/24.
//

#ifndef BITKLAVIER2_CONSTRUCTIONSITE_H
#define BITKLAVIER2_CONSTRUCTIONSITE_H
#include "Preparations/PreparationSection.h"
#include "PreparationSelector.h"
#include "Cable/CableView.h"
#include "templates/Factory.h"
#include "common.h"
class SynthGuiInterface;
typedef Loki::Factory<PreparationSection, int,  juce::ValueTree,  SynthGuiInterface*> PreparationFactory;
class ConstructionSite : public SynthSection,
                         public tracktion::engine::ValueTreeObjectList<PreparationSection>,private juce::KeyListener,
                         public juce::DragAndDropContainer,
                         public juce::ChangeListener

{
public:
    ConstructionSite(juce::ValueTree &v, juce::UndoManager &um, OpenGlWrapper &open_gl, SynthGuiData* data);

    ~ConstructionSite(void);

    void redraw(void);

    bool keyPressed(const juce::KeyPress &k, juce::Component *c) override;
    void itemIsBeingDragged(BKItem* thisItem, const juce::MouseEvent& e);

    void paintBackground(juce::Graphics& g) override;

    void addItem(bitklavier::BKPreparationType type, bool center = false);

    void changeListenerCallback(juce::ChangeBroadcaster *source) override

    {
        updateComponents();
    }

    void updateComponents();

    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::PREPARATION) || v.hasType(IDs::PIANO);
    }

    PreparationSection* createNewObject(const juce::ValueTree& v) override;
    void deleteObject (PreparationSection* at) override;


    void reset() override;
    void newObjectAdded (PreparationSection*) override;
    void objectRemoved (PreparationSection*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }
    void valueTreeParentChanged (juce::ValueTree&) override;
    void valueTreeRedirected (juce::ValueTree&) override ;

    BKPort* findPinAt (juce::Point<float> pos) const
    {
        for (auto* fc : objects)
        {
            // NB: A Visual Studio optimiser error means we have to put this juce::Component* in a local
            // variable before trying to cast it, or it gets mysteriously optimised away..
            auto* comp = fc->getComponentAt (pos.toInt() - fc->getPosition());

            if (auto* pin = dynamic_cast<BKPort*> (comp))
                return pin;
        }

        return nullptr;
    }
   PreparationSection* getComponentForPlugin (juce::AudioProcessorGraph::NodeID nodeID) const
    {
        for (auto* fc : objects)
            if (fc->pluginID == nodeID)
                return fc;

        return nullptr;
    }
    juce::Viewport* view;
   juce::Point<float> mouse;
    OpenGlWrapper &open_gl;

    juce::ValueTree getState()
    {
        return parent;
    }
    void copyValueTree(const juce::ValueTree& vt){
        parent.copyPropertiesFrom(vt,nullptr);
    }
private:

    SynthGuiInterface* _parent;




    bool edittingComment;

    juce::OwnedArray<juce::HashMap<int,int>> pastemap;
    friend class CableView;



    bool connect;

    int lastX, lastY;

    bool multiple;
    bool held;



    PreparationSelector preparationSelector;
    juce::LassoComponent<PreparationSection*> selectorLasso;

    friend class CableView;
    CableView cableView;

    juce::UndoManager &undo;

    void draw(void);

    void prepareItemDrag(BKItem* item, const juce::MouseEvent& e, bool center);

    void resized() override;

    void mouseDown (const juce::MouseEvent& eo) override;

    void mouseUp (const juce::MouseEvent& eo) override;

    void mouseDrag (const juce::MouseEvent& e) override;

    void mouseMove (const juce::MouseEvent& e) override;

    void deleteItem (BKItem* item);

    BKItem* getItemAtPoint(const int X, const int Y);


    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override
    {
        tracktion::engine::ValueTreeObjectList<PreparationSection>::valueTreePropertyChanged (v, i);
        if(v.getProperty("sync",0))
        {
            for(auto obj : objects)
            {
                juce::MemoryBlock data;
                obj->getProcessor()->getStateInformation(data);
                auto xml = juce::parseXML(data.toString());
//auto xml = juce::AudioProcessor::getXmlF(data.getData(), (int)data.getSize());
                if (obj->state.getChild(0).isValid())
                    obj->state.getChild(0).copyPropertiesFrom(juce::ValueTree::fromXml(*xml),nullptr);
                  //  state.addChild(juce::ValueTree::fromXml(*xml),0,nullptr);
            }
            v.removeProperty("sync", nullptr);
        }
    }

    PreparationFactory prepFactory;

    JUCE_LEAK_DETECTOR(ConstructionSite)
};


#endif //BITKLAVIER2_CONSTRUCTIONSITE_H
