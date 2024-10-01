//
// Created by Davis Polito on 6/28/24.
//

#ifndef BITKLAVIER2_CABLEVIEW_H
#define BITKLAVIER2_CABLEVIEW_H


#pragma once
#include "valuetree_utils/VariantConverters.h"
#include <JuceHeader.h>
#include "Cable.h"
#include "PreparationSection.h"

class ConstructionSite;
class CableView :
//                  private Timer,
                  public PreparationSection::Listener,
                  public SynthSection,
                  public tracktion::engine::ValueTreeObjectList<Cable>

{
public:
    explicit CableView (ConstructionSite &site);
    ~CableView() override;

    void paint (Graphics& g) override;
    void resized() override;

    void mouseMove (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;

//    auto* getConnectionHelper() { return connectionHelper.get(); }
//    auto* getPortLocationHelper() { return portLocationHelper.get(); }
////    void processorBeingAdded (BaseProcessor* newProc);
//    void processorBeingRemoved (const BaseProcessor* proc);

    bool cableBeingDragged() const;
    juce::Point<float> getCableMousePosition() const;
    void updateCablePositions();

    void updateComponents();

    void _update();
    //void portClicked(const juce::Point<int>& pos, juce::AudioProcessorGraph::Node::Ptr node) override;

    void paintBackground(juce::Graphics &g)
    {
//        DBG("paintbackground");
//        for(auto cable : cables)
//        {
//            cable->paint(g);
//        }
    }

    Cable* getComponentForConnection (const AudioProcessorGraph::Connection& conn) const
    {
        for (auto* cc : objects)
            if (cc->connection == conn)
                return cc;

        return nullptr;
    }
    juce::Point<int> currentPort;
    void dragConnector (const MouseEvent& e) override;

    void beginConnectorDrag (AudioProcessorGraph::NodeAndChannel source,
                             AudioProcessorGraph::NodeAndChannel dest,
                             const MouseEvent& e) override;



    void endDraggingConnector (const MouseEvent& e) override;



    /////VALUe Tree objectlist overrides
    Cable* createNewObject(const juce::ValueTree& v) override;
    void deleteObject (Cable* at) override;


    void reset() override;
    void newObjectAdded (Cable*) override;
    void objectRemoved (Cable*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }
   // void valueTreeParentChanged (juce::ValueTree&) override;
    void valueTreeRedirected (juce::ValueTree&) override ;
    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override {
        tracktion::engine::ValueTreeObjectList<Cable>::valueTreePropertyChanged(v, i);
    }
    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::CONNECTION);
    }

private:
//    void timerCallback() override;

    ConstructionSite& site;


    float scaleFactor = 1.0f;
    bool isDraggingCable = false;
    std::optional<juce::Point<int>> mousePosition;
    std::unique_ptr<Cable> draggingConnector;


    juce::Point<int> portToPaint;

    bool mouseOverClickablePort();
    bool mouseDraggingOverOutputPort();

    CriticalSection cableMutex;

    bool portGlow = false;

//    struct PathGeneratorTask : juce::TimeSliceClient
//    {
//    public:
//        explicit PathGeneratorTask (CableView& cv);
//        ~PathGeneratorTask() override;
//
//        int useTimeSlice() override;
//
//    private:
//        struct TimeSliceThread : juce::TimeSliceThread
//        {
//            TimeSliceThread() : juce::TimeSliceThread ("Cable Drawing Background Thread") {}
//        };
//
//        juce::SharedResourcePointer<TimeSliceThread> sharedTimeSliceThread;
//
//        CableView& cableView;
//    } pathTask;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CableView)
};



#endif //BITKLAVIER2_CABLEVIEW_H
