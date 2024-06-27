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


/************************************************************************************/
/*                            CLASS: SynthGuiInterface                              */
/************************************************************************************/

class SynthGuiInterface;

/************************************************************************************/
/*     CLASS: PreparationSection, inherits from SynthSection and Listener           */
/************************************************************************************/
class PreparationSection : public SynthSection, public BKItem::Listener
{
public:
    static constexpr float kItemPaddingY = 2.0f;
    static constexpr float kItemPaddingX = 2.0f;

    // Constructor Declaration
    PreparationSection(juce::String name, juce::ValueTree v, OpenGlWrapper &um);

    // Destructor Declaration
    ~PreparationSection();

    // Public member variables for a PreparationSection object
    juce::ValueTree state;
    OpenGlWrapper &_open_gl;
    std::unique_ptr<BKItem> item;
    int x, y, width, height;
    juce::ComponentDragger myDragger;
    juce::ComponentBoundsConstrainer constrainer;

    // Public function declarations, which override base class (SynthSection) virtual functions
    void paintBackground(juce::Graphics& g) override;
    void resized() override;

    // Public function definitions, which override base class (SynthSection) virtual functions
    void setSizeRatio(float ratio) override
    {
        size_ratio_ = ratio;
        item->size_ratio = ratio;
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        DBG(e.getNumberOfClicks());
            if(e.getNumberOfClicks() == 2)
            {
                showPrepPopup(this);

            }
            myDragger.startDraggingComponent (this, e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        myDragger.dragComponent (this, e, &constrainer);
    }

    void mouseDoubleClick(const juce::MouseEvent &event) override
    {
        DBG("sc");
        showPrepPopup(this);
    }


    /************************************************************************************/
    /*            NESTED CLASS: PreparationPopup, inherits from SynthSection            */
    /************************************************************************************/

    class PreparationPopup : public SynthSection
    {
    public:
        PreparationPopup (OpenGlWrapper &open_gl) : SynthSection("prep_popup"), _open_gl(open_gl)
        {

        }

        void initOpenGlComponents(OpenGlWrapper &open_gl){}

        void resized(){}
        OpenGlWrapper &_open_gl;
    private:

    };


    virtual std::shared_ptr<SynthSection> getPrepPopup(){}
    juce::AudioProcessorGraph::Node::Ptr node;
//juce::AudioProcessor _proc;
    virtual juce::AudioProcessor* getProcessor(){}
    virtual std::unique_ptr<juce::AudioProcessor> getProcessorPtr(){}
//std::shared_ptr<juce::AudioProcessor> _proc;
protected:
    std::shared_ptr<PreparationPopup> popup_view;
private:


    //void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override;
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

#endif // BITKLAVIER2_PREPARATIONSECTION_H
