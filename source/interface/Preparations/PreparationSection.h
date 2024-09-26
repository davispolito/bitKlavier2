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
#include "BKPort.h"
#include "valuetree_utils/VariantConverters.h"
/************************************************************************************/
/*                            CLASS: SynthGuiInterface                              */
/************************************************************************************/

class SynthGuiInterface;

/************************************************************************************/
/*     CLASS: PreparationSection, inherits from SynthSection and Listener           */
/************************************************************************************/
class PreparationSection : public SynthSection, public BKItem::Listener, public BKPort::Listener,  public ChangeListener
{
public:
    static constexpr float kItemPaddingY = 2.0f;
    static constexpr float kItemPaddingX = 2.0f;

    class Listener
    {
    public:
        virtual ~Listener() = default;
//        virtual void portClicked(const juce::Point<int>& pos, juce::AudioProcessorGraph::Node::Ptr) = 0;
        virtual void beginConnectorDrag(AudioProcessorGraph::NodeAndChannel source,
                                        AudioProcessorGraph::NodeAndChannel dest,
                                        const MouseEvent& e) = 0;
        virtual void dragConnector(const MouseEvent& e) = 0;
        virtual void endDraggingConnector(const MouseEvent& e) = 0;
        virtual void _update() = 0;
    };

    void beginConnectorDrag(AudioProcessorGraph::NodeAndChannel source,
                                    AudioProcessorGraph::NodeAndChannel dest,
                                    const MouseEvent& e)
    {
        for (auto listener: listeners_)
        {
            listener->beginConnectorDrag(source,
                                         dest,
                                         e);
        }
    }
    void dragConnector(const MouseEvent& e)
    {for (auto listener: listeners_)
        {
            listener->dragConnector(e);
        }

    }
    void endDraggingConnector(const MouseEvent& e)
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
    PreparationSection(juce::String name, juce::ValueTree v, OpenGlWrapper &um);

    // Destructor Declaration
    ~PreparationSection();

    virtual void addSoundSet(juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* s) {}
    // Public member variables for a PreparationSection object
    juce::ValueTree state;
    OpenGlWrapper &_open_gl;
    SelectedItemSet<PreparationSection*> *selectedSet;
    std::unique_ptr<BKItem> item;
    CachedValue<int> x, y, width, height, numIns, numOuts;
    juce::ComponentDragger myDragger;
    juce::ComponentBoundsConstrainer constrainer;
    OwnedArray<BKPort> ports;

    void update()
    {
//        const AudioProcessorGraph::Node::Ptr f (graph.graph.getNodeForId (pluginID));
//        jassert (f != nullptr);
//
//        auto& processor = *f->getProcessor();
//
//        numIns = processor.getTotalNumInputChannels();
//        if (processor.acceptsMidi())
//            ++numIns;
//
//        numOuts = processor.getTotalNumOutputChannels();
//        if (processor.producesMidi())
//            ++numOuts;
//
//        int w = 100;
//        int h = 60;
//
//        w = jmax (w, (jmax (numIns, numOuts) + 1) * 20);
//
//        const int textWidth = font.getStringWidth (processor.getName());
//        w = jmax (w, 16 + jmin (textWidth, 300));
//        if (textWidth > 300)
//            h = 100;
//
//        setSize (w, h);
//        setName (processor.getName() + formatSuffix);
//
//        {
//            auto p = graph.getNodePosition (pluginID);
//            setCentreRelative ((float) p.x, (float) p.y);
//        }
//
//        if (numIns != numInputs || numOuts != numOutputs)
//        {
//            numInputs = numIns;
//            numOutputs = numOuts;
//
//            pins.clear();
//
//            for (int i = 0; i < processor.getTotalNumInputChannels(); ++i)
//                addAndMakeVisible (pins.add (new PinComponent (panel, { pluginID, i }, true)));
//
//            if (processor.acceptsMidi())
//                addAndMakeVisible (pins.add (new PinComponent (panel, { pluginID, AudioProcessorGraph::midiChannelIndex }, true)));
//
//            for (int i = 0; i < processor.getTotalNumOutputChannels(); ++i)
//                addAndMakeVisible (pins.add (new PinComponent (panel, { pluginID, i }, false)));
//
//            if (processor.producesMidi())
//                addAndMakeVisible (pins.add (new PinComponent (panel, { pluginID, AudioProcessorGraph::midiChannelIndex }, false)));
//
//            resized();
//        }
    }

    void changeListenerCallback(ChangeBroadcaster *source)
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

    void mouseDown (const juce::MouseEvent& e) override
    {
        DBG(e.getNumberOfClicks());
            if(e.getNumberOfClicks() == 2)
            {
                showPrepPopup(this);

            } else
            {
              //  selectedSet->addToSelectionBasedOnModifiers(this, e.mods);
            }
            myDragger.startDraggingComponent (this, e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        myDragger.dragComponent (this, e, &constrainer);
        for (auto listener: listeners_)
        {
            listener->_update();
        }
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

    juce::Point<float> getPinPos (int index, bool isInput) const
    {
        for (auto* port : ports)
            if (port->pin.channelIndex == index && isInput == port->isInput)
                return getPosition().toFloat() + port->getBounds().getCentre().toFloat();

        return {};
    }
    virtual std::shared_ptr<SynthSection> getPrepPopup(){}
    void setNodeAndPortInfo(juce::AudioProcessorGraph::Node::Ptr _node)
    {
        node = _node;
        pluginID = node->nodeID;
        int i = 0;
        auto& processor = *_node->getProcessor();

        numIns = processor.getTotalNumInputChannels();
        if (processor.acceptsMidi())
            numIns = numIns + 1;

        numOuts = processor.getTotalNumOutputChannels();
        if (processor.producesMidi())
            numOuts = numOuts + 1;
        MessageManager::callAsync (
                [safeComp = Component::SafePointer<PreparationSection> (this)]
                {
                    safeComp->state.setProperty(IDs::nodeID,  VariantConverter<juce::AudioProcessorGraph::NodeID>::toVar(safeComp->pluginID), nullptr);

                    SynthGuiInterface* parent = safeComp->findParentComponentOfClass<SynthGuiInterface>();
                    auto& processor = *safeComp->node->getProcessor();
                    for (int i = 0; i < processor.getTotalNumInputChannels(); ++i)
                        safeComp->ports.add (new BKPort (true, { safeComp->pluginID, i }, parent));

                    if(processor.acceptsMidi())
                        safeComp->ports.add (new BKPort (true, {safeComp-> pluginID, AudioProcessorGraph::midiChannelIndex}, parent));

                    for(int i = 0; i < processor.getTotalNumInputChannels(); ++i)
                        safeComp->ports.add (new BKPort (false, {safeComp->pluginID, i}, parent));

                    if(processor.producesMidi())
                        safeComp->ports.add (new BKPort (false, {safeComp->pluginID, AudioProcessorGraph::midiChannelIndex}, parent));




                        safeComp->_open_gl.initOpenGlComp.try_enqueue([ safeComp] {
                            for (auto *port: safeComp->ports) {

                                port->getImageComponent()->init(safeComp->_open_gl);
                                MessageManagerLock mm;
                                safeComp->addOpenGlComponent(port->getImageComponent(),false, true);
                                safeComp->addAndMakeVisible(port);
                                port->addListener(safeComp);
                            }
                            MessageManagerLock mm;
                            safeComp->resized();
                        });


                });




    }
    juce::AudioProcessorGraph::Node::Ptr node;
    juce::AudioProcessorGraph::NodeID pluginID;
    CachedValue<juce::Uuid> uuid;
//juce::AudioProcessor _proc;
    virtual juce::AudioProcessor* getProcessor(){}
    virtual std::unique_ptr<juce::AudioProcessor> getProcessorPtr(){}
//std::shared_ptr<juce::AudioProcessor> _proc;
protected:
    std::shared_ptr<PreparationPopup> popup_view;

    int portSize = 16;
private:
    bool isSelected = true;

    //void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override;
    //SynthGuiInterface *_parent;



};




#endif // BITKLAVIER2_PREPARATIONSECTION_H
