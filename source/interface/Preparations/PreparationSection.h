//
// Created by Davis Polito on 2/13/24.
//

#ifndef BITKLAVIER2_PREPARATIONSECTION_H
#define BITKLAVIER2_PREPARATIONSECTION_H
#include "synth_section.h"
#include "draggable_component.h"
#include "tracktion_engine.h"
#include "common.h"
#include "Identifiers.h"
#include "BKItem.h"
#include "opengl_ComponentDragger.h"
class SynthGuiInterface;
class PreparationSection : public SynthSection, public BKItem::Listener,  public bitklavier::ValueTreePropertyChangeListener
{
public:
    static constexpr float kItemPaddingY = 2.0f;
    static constexpr float kItemPaddingX = 2.0f;
    //static constexpr float kItemPaddingY = 2.0f;
    PreparationSection(String name, ValueTree v, UndoManager &um);
    ~PreparationSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    ValueTree state;
    std::unique_ptr<BKItem> item;
    int x, y, width, height;
    void setSizeRatio(float ratio) override
    {
        size_ratio_ = ratio;
        item->size_ratio = ratio;
        myDragger.size_ratio_ = ratio;
    }
    openGLComponentDragger myDragger;

    void mouseDown (const MouseEvent& e) override
    {
        myDragger.startDraggingComponent (this, e);
    }

    void mouseDrag (const MouseEvent& e) override
    {
        myDragger.dragComponent (this, e, nullptr);
        //setBounds(getX() - getX() * size_ratio_,getY() - getY() * size_ratio_, getWidth(), getHeight());
    }
private:


    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override;
    //SynthGuiInterface *_parent;



};



//typedef Loki::Factory<PreparationSection, int,  juce::ValueTree,  juce::UndoManager&> PreparationFactory;
//class PreparationList : public SynthSection,
//                        public tracktion::engine::ValueTreeObjectList<PreparationSection>
//{
//public:
//    PreparationList(ValueTree editTree, juce::UndoManager& um);
//
//
//    ~PreparationList()
//    {
//        freeObjects();
//    }
//
//    bool isSuitableType (const juce::ValueTree& v) const override
//    {
//        return v.hasType (IDs::PREPARATION);
//    }
//
//    PreparationSection* createNewObject(const juce::ValueTree& v) override;
//    void deleteObject (PreparationSection* at) override
//    {
//        delete at;
//    }
//
//    void resized() override
//    {
//        for (auto prep: objects)
//        {
//            prep->setBounds(prep->x, prep->y, prep->getWidth(), prep->getHeight());
//        }
//    }
//    void newObjectAdded (PreparationSection*) override    { } //resized(); }
//    void objectRemoved (PreparationSection*) override     { }//resized(); }
//    void objectOrderChanged() override              { }//resized(); }
//
//private :
//    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override
//    {
////        if (isSuitableType (v))
////            if (i == IDs::start || i == IDs::length)
////                resized();
//
//        tracktion::engine::ValueTreeObjectList<PreparationSection>::valueTreePropertyChanged (v, i);
//    }
//    //ReferenceCountedArray<PreparationSection,
//
//    PreparationFactory prepFactory;
//    SynthGuiInterface* _parent;
//};
#endif //BITKLAVIER2_PREPARATIONSECTION_H