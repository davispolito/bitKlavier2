//
// Created by Davis Polito on 2/13/24.
//

#ifndef BITKLAVIER2_PREPARATIONSECTION_H
#define BITKLAVIER2_PREPARATIONSECTION_H
#include "valuetree_utils/VariantConverters.h"
#include <juce_data_structures/juce_data_structures.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "tracktion_ValueTreeUtilities.h"
#include "synth_section.h"
#include "draggable_component.h"
#include "common.h"
#include "Identifiers.h"
#include "BKItem.h"
#include "opengl_ComponentDragger.h"
#include "BKPort.h"

/************************************************************************************/
/*                            CLASS: SynthGuiInterface                              */
/************************************************************************************/

class SynthGuiInterface;

/************************************************************************************/
/*     CLASS: PreparationSection, inherits from SynthSection and Listener           */
/************************************************************************************/
class PreparationSection : public SynthSection, public BKItem::Listener, public BKPort::Listener,  public juce::ChangeListener,
public tracktion::engine::ValueTreeObjectList<BKPort>, public juce::DragAndDropTarget

{
public:
    static constexpr float kItemPaddingY = 2.0f;
    static constexpr float kItemPaddingX = 2.0f;

    class Listener
    {
    public:
        virtual ~Listener() = default;
//        virtual void portClicked(const juce::Point<int>& pos, juce::AudioProcessorGraph::Node::Ptr) = 0;
        virtual void beginConnectorDrag(juce::AudioProcessorGraph::NodeAndChannel source,
                                        juce::AudioProcessorGraph::NodeAndChannel dest,
                                        const juce::MouseEvent& e) = 0;
        virtual void dragConnector(const juce::MouseEvent& e) = 0;
        virtual void endDraggingConnector(const juce::MouseEvent& e) = 0;
        virtual void _update() = 0;
        virtual void preparationDragged(juce::Component* prep, const juce::MouseEvent& e) {}
        virtual void preparationDropped(const juce::MouseEvent& e, juce::Point<int> originalComponentPoint) {} ;
    };

    void beginConnectorDrag(juce::AudioProcessorGraph::NodeAndChannel source,
                                    juce::AudioProcessorGraph::NodeAndChannel dest,
                                    const juce::MouseEvent& e)
    {
        for (auto listener: listeners_)
        {
            listener->beginConnectorDrag(source,
                                         dest,
                                         e);
        }
    }
    void dragConnector(const juce::MouseEvent& e)
    {for (auto listener: listeners_)
        {
            listener->dragConnector(e);
        }

    }
    void endDraggingConnector(const juce::MouseEvent& e)
    {
        for (auto listener: listeners_)
        {
            listener->endDraggingConnector(
                    e);
        }
    }
    void addListener(Listener* listener) { listeners_.push_back(listener); }
    std::vector<Listener*> listeners_;
    // Constructor Declaration
    PreparationSection(juce::String name, juce::ValueTree v, OpenGlWrapper &um, juce::AudioProcessor* proc);

    // Destructor Declaration
    ~PreparationSection();

    virtual void addSoundSet(std::map<juce::String, juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>>* s) {}
    virtual void addSoundSet(
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* s,
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* h,
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* r,
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* p) {}
    // Public member variables for a PreparationSection object
    juce::ValueTree state;
    OpenGlWrapper &_open_gl;
    juce::SelectedItemSet<PreparationSection*> *selectedSet;
    std::unique_ptr<BKItem> item;
    juce::CachedValue<int> x, y, width, height, numIns, numOuts;
//    juce::ComponentDragger myDragger;
    juce::ComponentBoundsConstrainer constrainer;

    BKPort* createNewObject(const juce::ValueTree& v) override;
    void deleteObject (BKPort* at) override;


    void reset() override;
    void newObjectAdded (BKPort*) override;
    void objectRemoved (BKPort*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }
    // void valueTreeParentChanged (juce::ValueTree&) override;
    void valueTreeRedirected (juce::ValueTree&) override ;
    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override {
        tracktion::engine::ValueTreeObjectList<BKPort>::valueTreePropertyChanged(v, i);
    }
    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::PORT);
    }


    void changeListenerCallback(juce::ChangeBroadcaster *source)
    {
       //DBG("changelistener");
        if(selectedSet->isSelected(this))
        {
           // DBG("white");
            item->setColor(juce::Colours::white);
            isSelected = true;
        }
        else{
            item->setColor(findColour(Skin::kWidgetPrimary1, true));
            isSelected = false;
        }
        item->redoImage();

    }

    // Public function declarations, which override base class (SynthSection) virtual functions
    void paintBackground(juce::Graphics& g) override;
    void resized() override;
    void setDefaultColor()
    {
        item->setColor(findColour(Skin::kWidgetPrimary1, true));
        item->redoImage();
    }
    // Public function definitions, which override base class (SynthSection) virtual functions
    void setSizeRatio(float ratio) override
    {
        size_ratio_ = ratio;
        item->size_ratio = ratio;
    }

    void mouseDown (const juce::MouseEvent& e) override;
    void mouseEnter(const juce::MouseEvent& e) override
    {
                setMouseCursor(juce::MouseCursor::ParentCursor);
        DBG(" mosue enter -> " + this->getName());
    }
    void mouseDrag (const juce::MouseEvent& e) override
    {
//        myDragger.dragComponent (this, e, &constrainer);
        for (auto listener: listeners_)
        {
            listener->_update();
            listener->preparationDragged(this, e);
        }
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    }

    void mouseUp (const juce::MouseEvent& e) override
    {
        setMouseCursor(juce::MouseCursor::ParentCursor);
        DBG("triggermouseup  " + this->getName());
        for (auto listener : listeners_)
        {
            listener->_update();
            listener->preparationDropped(e, pointBeforDrag);
        }
    }

    void mouseDoubleClick(const juce::MouseEvent &event) override
    {
        DBG("sc");
        showPrepPopup(this);
    }

    ///drraganddrop for line view and modulator

    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override { return true;}
    void itemDropped(const SourceDetails& dragSourceDetails) override;


    void 	itemDragMove (const SourceDetails &dragSourceDetails)
    {

        DBG(getName() + "draggemove");
    }
    void itemDragEnter (const SourceDetails &dragSourceDetails){
        DBG(getName() + "drag emter'");
    }
    void 	itemDragExit (const SourceDetails &dragSourceDetails)
    {

        DBG(getName() + "drag exort");
    }

//    void 	itemDragMove (const SourceDetails &dragSourceDetails)
//    void itemDragEnter (const SourceDetails &dragSourceDetails)
//    void 	itemDragExit (const SourceDetails &dragSourceDetails)
//    bool 	shouldDrawDragImageWhenOver ()
    /************************************************************************************/
    /*            NESTED CLASS: PreparationPopup, inherits from SynthSection            */
    /************************************************************************************/

    class PreparationPopup : public SynthSection
    {
    public:
        PreparationPopup (OpenGlWrapper &open_gl) : SynthSection("prep_popup", &open_gl)
        {

        }

        void initOpenGlComponents(OpenGlWrapper &open_gl){}

        void resized(){}
    private:

    };

    juce::Point<float> getPinPos (int index, bool isInput) const
    {
        for (auto* port : objects)
            if (port->pin.channelIndex == index && isInput == port->isInput)
                return getPosition().toFloat() + port->getBounds().getCentre().toFloat();

        return {};
    }
    void setPortInfo()
    {
        numIns = getProcessor()->getTotalNumInputChannels();
        if (getProcessor()->acceptsMidi())
            numIns = numIns + 1;

        numOuts = getProcessor()->getTotalNumOutputChannels();
        if (getProcessor()->producesMidi())
            numOuts = numOuts + 1;


            SynthGuiInterface* parent = this->findParentComponentOfClass<SynthGuiInterface>();
            auto& processor = *this->node->getProcessor();
            for (int i = 0; i < processor.getTotalNumInputChannels(); ++i)
            {
                juce::ValueTree v{IDs::PORT} ;
                v.setProperty(IDs::nodeID,juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(this->pluginID), nullptr);
                v.setProperty(IDs::chIdx, i, nullptr);
                state.addChild(v, -1, nullptr) ;
            }
                //this->objects.add (new BKPort (true, { this->pluginID, i }, parent));

            if(processor.acceptsMidi())
            {
                juce::ValueTree v{IDs::PORT} ;
                v.setProperty(IDs::nodeID,juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(this->pluginID), nullptr);
                v.setProperty(IDs::chIdx, juce::AudioProcessorGraph::midiChannelIndex, nullptr);
                v.setProperty(IDs::isIn, true, nullptr);
                state.addChild(v, -1, nullptr) ;
            }

            for(int i = 0; i < processor.getTotalNumOutputChannels(); ++i)
            {
                juce::ValueTree v{IDs::PORT} ;
                v.setProperty(IDs::nodeID,juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(this->pluginID), nullptr);
                v.setProperty(IDs::chIdx, i, nullptr);
                v.setProperty(IDs::isIn, false, nullptr);
                state.addChild(v, -1, nullptr) ;
            }

            if(processor.producesMidi())
            {
                juce::ValueTree v{IDs::PORT} ;
                v.setProperty(IDs::nodeID,juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(this->pluginID), nullptr);
                v.setProperty(IDs::chIdx, juce::AudioProcessorGraph::midiChannelIndex, nullptr);
                v.setProperty(IDs::isIn, false, nullptr);
                state.addChild(v, -1, nullptr) ;
            }





//            this->_open_gl.initOpenGlComp.try_enqueue([ this] {
//                for (auto *port: this->objects) {
//
//                    port->getImageComponent()->init(this->_open_gl);
//                    juce::MessageManagerLock mm;
//                    this->addOpenGlComponent(port->getImageComponent(),false, true);
//                    this->addAndMakeVisible(port);
//                    port->addListener(this);
//                }
//                juce::MessageManagerLock mm;
//                this->resized();
//                DBG("ports added");
//            });




    }
    virtual std::shared_ptr<SynthSection> getPrepPopup(){}
    void setNodeInfo(juce::AudioProcessorGraph::Node::Ptr _node)
    {
        node = _node;
        pluginID = node->nodeID;
        this->state.setProperty(IDs::nodeID,  juce::VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(this->pluginID), nullptr);
        int i = 0;
        auto& processor = *_node->getProcessor();
        if (objects.isEmpty()) {
            juce::MessageManager::callAsync ([safeComp = juce::Component::SafePointer<PreparationSection> (this)]
                                       {
                safeComp->setPortInfo();
                                       });
        }




    }
    juce::AudioProcessorGraph::Node::Ptr node;
    juce::AudioProcessorGraph::NodeID pluginID;
    juce::CachedValue<juce::Uuid> uuid;
//juce::AudioProcessor _proc;
    virtual juce::AudioProcessor* getProcessor(){}
    virtual std::unique_ptr<juce::AudioProcessor> getProcessorPtr(){}
    std::shared_ptr<PreparationPopup> popup_view;
//std::shared_ptr<juce::AudioProcessor> _proc;
//
//    void itemDropped (const SourceDetails &dragSourceDetails) override
//    {
//        DBG(" dropped" + this->getName());
//    }
//    bool isInterestedInDragSource (const SourceDetails &dragSourceDetails) override
//    {
//        return true;
//    }
protected:


    int portSize = 16;
private:
    juce::Point<int> pointBeforDrag; // e.getEventRelativeTo (componentToDrag).getMouseDownPosition();
    bool isSelected = true;
    juce::AudioProcessor* _proc;

    //void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override;
    //SynthGuiInterface *_parent;



};




#endif // BITKLAVIER2_PREPARATIONSECTION_H
