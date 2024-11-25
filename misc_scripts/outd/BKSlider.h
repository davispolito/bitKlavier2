/*
 ==============================================================================
 
 BKSlider.h
 Created: 6 Apr 2017 9:50:44pm
 Author:  Daniel Trueman
 
 ==============================================================================
 */

/* TODO
 
 1. need version of SingleSlider that can handle multiple values (for Nostalgic transposition)
 
 */

#ifndef BKSLIDER_H_INCLUDED
#define BKSLIDER_H_INCLUDED

#include "BKUtilities.h"
#include "BKComponent.h"
#include "BKLookAndFeel.h"
#include "BKUIComponents.h"
#include "BKLabel.h"

// ******************************************************************************************************************** //
// **************************************************  BKSubSlider **************************************************** //
// ******************************************************************************************************************** //

class BKSubSlider : public juce::Slider
{
public:
    
    BKSubSlider (SliderStyle sstyle, double min, double max, double def, double increment, int width, int height);
    ~BKSubSlider();
    
    double getValueFromText    (const juce::String & text ) override;
    bool isActive() { return active; }
    void isActive(bool newactive) {active = newactive; }
    void setMinMaxDefaultInc(std::vector<float> newvals);
    void setSkewFromMidpoint(bool sfm);
    
private:
    
    double sliderMin, sliderMax;
    double sliderDefault;
    double sliderIncrement;
    
    int sliderWidth, sliderHeight;
    
    bool sliderIsVertical;
    bool sliderIsBar;
    bool skewFromMidpoint;
    
    bool active;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKSubSlider)
};




// ******************************************************************************************************************** //
// **************************************************  BKMultiSlider ************************************************** //
// ******************************************************************************************************************** //

/*
 
 BKMultiSlider is a horizontal array of sliders.
 
 Each "slider" can have multiple sliders, facilitating, for instance, multiple transposition values
 
 The user can drag across the sliders to quickly set values
 
 The user can also enter slider values directly via a text editor:
    "1 [-1, -2, -3] 2 3" will create three sliders, and the second will have three overlaid sliders
 
 By default it has 12 sliders, but that can be increased by adding additional values via text entry
 
 Gaps in the slider array are possible, and indicated by a forward slash '/' in the text editor
 
 The API includes functions for setting the slider, either all at once (via text) or by individual slider (via mouse)
 and for deactivating individual sliders or sections of sliders.
 
*/

class BKMultiSlider :
public juce::Component,
public juce::Slider::Listener,
public juce::TextEditor::Listener,
public juce::ImageButton::Listener
#if JUCE_IOS
, public WantsBigOne
#endif
{
    
public:
    
// *** Public Instance Methods *** //
    
    // constructor
    BKMultiSlider();
    ~BKMultiSlider();
        
    // when the client sends an array of only the active slider values, this will construct the complete array
    // of slider values, including inactive sliders, and then call setTo. So
    //      newActiveVals = {1, 2, 3}
    //      newactives = {true, false, false, true, true, false....... false}
    // will result in [1 / / 2 3 / / / / / / /], where / represents an inactive slider (gap)
    void setToOnlyActive(juce::Array<juce::Array<float>> newActiveVals, juce::Array<bool> newactives, NotificationType newnotify);
    
    // as above, but takes a 1d array, for sliders that don't use subSliders
    void setToOnlyActive(juce::Array<float> newActiveVals, juce::Array<bool> newactives, NotificationType newnotify);
    
    // identifiers
    void setName(juce::String newName){
        sliderName = newName;
        showName.setText(sliderName, dontSendNotification);
    }
    juce::String getName() { return sliderName; }
    void setToolTipString(juce::String newTip) { showName.setTooltip(newTip); bigInvisibleSlider->setTooltip(newTip); }

    // slider range and skew
    void setMinMaxDefaultInc(std::vector<float> newvals);
    void setSkewFromMidpoint(bool sfm);
    
    // highlight whichever slider is currently active in bK
    void setCurrentSlider(int activeSliderNum);
    void deHighlightCurrentSlider(void);
    inline int getNumVisible(void) const noexcept { return allSliderVals.size(); } //return numVisibleSliders;}
    
    // should this slider allow subsliders? (multiple transpositions, for instance)
    void setAllowSubSlider(bool ss) { allowSubSliders = ss; }
    
    // name of the subslider (add transposition, for instance)
    void setSubSliderName(juce::String ssname) { subSliderName = ssname; }
    
    // listeners overrride these functions so they can get what they need from MultiSlider
    // Multislider will call these when values change, so the clients can update
    class Listener
    {
        public:
            
        virtual ~Listener() {};
        virtual void multiSliderValueChanged(juce::String name, int whichSlider, juce::Array<float> values) = 0;
        virtual void multiSliderAllValuesChanged(juce::String name, juce::Array<juce::Array<float>> values, juce::Array<bool> states) = 0;
    };
    
    // listeners
    juce::ListenerList<Listener> listeners;
    void addMyListener(Listener* listener)     { listeners.add(listener);      }
    void removeMyListener(Listener* listener)  { listeners.remove(listener);   }
  
    
private:
    
// *** Private Instance Methods *** //

    // this is the main function for setting the multislider
    // the array of arrays is the slider values, with the inner array for subsliders (multiple sliders at one position
    // the array of bools sets which of the sliders is actually active
    void setTo(juce::Array<juce::Array<float>> newvals, juce::Array<bool> newactives, NotificationType newnotify);
    
    // initialize the slider; it should have no less than numDefaultSliders, all set to sliderDefault value
    void initializeSliderVals(int howmany);
    
    // update and activate a particular slider
    void updateSliderVal(int which, int whichSub, float val);
    
    // print out the current vals, for debugging
    void printSliderVals();
    
    // draw the actual sliders
    void drawSliders(NotificationType newnotify);
    
    // return all values, including inactive sliders
    juce::Array<juce::Array<float>> getAllValues() { return allSliderVals; }
    
    // return only those sliders that are active
    juce::Array<juce::Array<float>> getAllActiveValues();
    
    // return the values for one slider
    juce::Array<float> getOneSliderBank(int which);
    
    // inserts a slider into the multislider
    void addSlider(int where, bool active, NotificationType newnotify);
    
    // adds an additional slider at a particular index; it's possible to have multiple
    // sliders at one location, facilitating, for instance, multiple transpositions at one time
    void addSubSlider(int where, bool active, NotificationType newnotify);
    
    // as above, but updates the allSliderVals and whichSlidersActive first
    void addActiveSubSlider(int where, NotificationType newnotify);
    
    // these methods clear currently active sliders
    // exception: you can't clear the FIRST slider, as we need at least one active slider
    void deactivateSlider(int where, NotificationType notify);
    void deactivateAll(NotificationType notify);
    void deactivateAllAfter(int where, NotificationType notify);
    void deactivateAllBefore(int where, NotificationType notify);
    
    // slider values can be edited directly via a text editor
    // values in brackets will be collected at one index
    // for example: "0 [1 2 3] 4" will have three active sliders, and the second one will have three subsliders
    inline void setText(juce::String text) { editValsTextField->setText(text, dontSendNotification); }
    inline juce::TextEditor* getTextEditor(void) { return editValsTextField.get();}
    inline void dismissTextEditor(bool setValue = false);
    
    // identify which slider is mouseDowned
    int whichSlider (const juce::MouseEvent &e);
    
    // identify which subSlider is closest to cursor after mouseDown on a particular slider
    int whichSubSlider (int which);
    
    // identify which subSlider is closest on mouseOver
    int whichSubSlider (int which, const juce::MouseEvent &e);
    
    // update which slider is active after mouseDrag and mouseUp; to support dragging across multiple sliders
    int whichActiveSlider (int which);
    
    // for highlighting text related to sliders
    void highlight(int activeSliderNum);
    void deHighlight(int sliderNum);
    int getActiveSlider(int sliderNum);
    
    // rescale/normalize slider ranges
    void resetRanges();
    
    // mouse/text events
    void mouseDrag(const juce::MouseEvent &e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    void mouseDoubleClick (const juce::MouseEvent &e) override;
    void mouseDown (const juce::MouseEvent &event) override;
    void mouseUp (const juce::MouseEvent &event) override;
    void textEditorEscapeKeyPressed (juce::TextEditor& textEditor) override;
    void sliderValueChanged (juce::Slider *slider) override;
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;
    void textEditorFocusLost(juce::TextEditor& textEditor) override;
    void textEditorTextChanged(juce::TextEditor&) override;
    void buttonClicked(juce::Button* button) override;
    void showModifyPopupMenu(int which);
    static void sliderModifyMenuCallback (const int result, BKMultiSlider* slider, int which);
    
    void resized() override;
    

// *** Instance Variables *** //
    
    // holds all the current values, including for sub sliders (hence the double array)
    juce::Array<juce::Array<float>> allSliderVals;
    
    // which of these sliders is active; the first is always true
    juce::Array<bool> whichSlidersActive;
    
    // the invisible slider lays on top of all the sliders, its value
    // is used to set the values of the individual slider that the mouse is nearest
    double currentInvisibleSliderValue;
    
    // allow sliders to have multiple values
    bool allowSubSliders;
    
    // if allowSubSliders, what to call it "add [subSliderName]"
    juce::String subSliderName;
    
    // to keep track of stuff
    int currentSubSlider;
    int lastHighlightedSlider;
    bool focusLostByEscapeKey;
    bool skewFromMidpoint;
    float clickedHeight;
    
    // default values
    double sliderMin, sliderMax, sliderMinDefault, sliderMaxDefault;
    double sliderDefault;
    double sliderIncrement;
    
    // dimensions
    int totalWidth;
    int sliderHeight;
    float sliderWidth;
    int displaySliderWidth;

    // by default, how many sliders to show (12)
    int numDefaultSliders;
        
    // UI elements
    BKMultiSliderLookAndFeel activeSliderLookAndFeel;
    BKMultiSliderLookAndFeel passiveSliderLookAndFeel;
    BKMultiSliderLookAndFeel highlightedSliderLookAndFeel;
    BKMultiSliderLookAndFeel displaySliderLookAndFeel;
    juce::String sliderName;
    BKLabel showName;
    juce::Slider::SliderStyle subsliderStyle;
    juce::OwnedArray<juce::OwnedArray<BKSubSlider>> sliders;
    std::unique_ptr<BKSubSlider> displaySlider;
    std::unique_ptr<BKSubSlider> bigInvisibleSlider;
    std::unique_ptr<juce::TextEditor> editValsTextField;
    std::unique_ptr<juce::ImageButton> rotateButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKMultiSlider)
};



// ******************************************************************************************************************** //
// **************************************************  BKSingleSlider ************************************************* //
// ******************************************************************************************************************** //


//basic horizontal slider with its own text box and label
//entering values in the text box will reset the range as needed
class BKSingleSlider :
public juce::Component,
public ModdableComponent,
public juce::Button::Listener,
public juce::Slider::Listener,
public juce::TextEditor::Listener
#if JUCE_IOS
, public WantsBigOne
#endif
{
public:
    BKSingleSlider(juce::String sliderName, juce::String Id, double min, double max, double def, double increment, juce::String minDisplay = juce::String());
    ~BKSingleSlider()
    {
        displaySlider->setLookAndFeel(nullptr);
        moddableOptionsButton.setLookAndFeel(nullptr);
        setLookAndFeel(nullptr);
    };
    
    juce::Slider thisSlider;
    std::unique_ptr<juce::Slider> displaySlider;
    
    juce::TextButton moddableOptionsButton;
    
    juce::String sliderName;
    BKLabel showName;
    bool textIsAbove;
    
    BKTextEditor valueTF;
    
    void setChangeNotificationOnlyOnRelease(bool yn) { thisSlider.setChangeNotificationOnlyOnRelease(yn);}
    
    inline juce::String getText(void) { return valueTF.getText(); }
    inline void setText(juce::String text, NotificationType notify = dontSendNotification) { valueTF.setText(text, notify);}
    
    void setName(juce::String newName)    { sliderName = newName; showName.setText(sliderName, dontSendNotification); }
    juce::String getName()                { return sliderName; }
    void setTextIsAbove(bool nt)    { textIsAbove = nt; }
    void setToolTipString(juce::String newTip) { showName.setTooltip(newTip); thisSlider.setTooltip(newTip); valueTF.setTooltip(newTip);}
    
    void setJustifyRight(bool jr)
    {
        justifyRight = jr;
        if (justifyRight) showName.setJustificationType(juce::Justification::bottomRight);
        else showName.setJustificationType(juce::Justification::bottomLeft);
    }
    
    void setValue(double newval, NotificationType notify);
    void setValue(double newval, int numDecimalPoints, NotificationType notify);
    void checkValue(double newval);
    double getValue() {return thisSlider.getValue();}
    
    void setDisplayValue(double newval) { displaySlider->setValue(newval); }
    void displaySliderVisible(bool vis) { displaySlider->setVisible(vis); }
    
    void sliderValueChanged (juce::Slider *slider) override;
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &e) override;

    void textEditorEscapeKeyPressed (juce::TextEditor& textEditor) override;
    void textEditorFocusLost(juce::TextEditor& textEditor) override;
    void textEditorTextChanged(juce::TextEditor& textEditor) override;
    void resized() override;
    
    void buttonClicked(juce::Button* b) override;
    
    void setSkewFactor (double factor, bool symmetricSkew) { thisSlider.setSkewFactor(factor, symmetricSkew); }
    void setSkewFactorFromMidPoint (double sliderValueToShowAtMidPoint    ) { thisSlider.setSkewFactorFromMidPoint(sliderValueToShowAtMidPoint); }
    
    void setSliderTextResolution(int res) {sliderTextResolution = res;}
    
    class Listener
    {
    public:
        
        //BKSingleSlider::Listener() {}
        virtual ~Listener() {};
        
        virtual void BKSingleSliderValueChanged(BKSingleSlider* slider, juce::String name, double val) = 0;
    };
    
    juce::ListenerList<Listener> listeners;
    void addMyListener(Listener* listener)     { listeners.add(listener);      }
    void removeMyListener(Listener* listener)  { listeners.remove(listener);   }
    
    void setDim(float newAlpha);
    void setBright();
    
private:
    
    double sliderMin, sliderMax;
    double sliderDefault;
    double sliderIncrement;
    int sliderTextResolution;
    
    juce::String minDisplay;
    
    bool focusLostByEscapeKey;
    
    bool justifyRight;
    
    BKDisplaySliderLookAndFeel displaySliderLookAndFeel;
    BKButtonAndMenuLAF buttonLAF;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKSingleSlider)
    
};



// ******************************************************************************************************************** //
// **************************************************  BKRangeSlider ************************************************** //
// ******************************************************************************************************************** //


class BKRangeSlider :
public juce::Component,
public juce::Slider::Listener,
public juce::TextEditor::Listener
#if JUCE_IOS
, public WantsBigOne
#endif
{
public:
    BKRangeSlider(juce::String sliderName, double min, double max, double defmin, double defmax, double increment);
    ~BKRangeSlider()
    {
        minSlider.setLookAndFeel(nullptr);
        maxSlider.setLookAndFeel(nullptr);
        invisibleSlider.setLookAndFeel(nullptr);
        displaySlider->setLookAndFeel(nullptr);
    };
    
    juce::Slider minSlider;
    juce::Slider maxSlider;
    juce::String minSliderName;
    juce::String maxSliderName;
    
    juce::Slider invisibleSlider;
    
    std::unique_ptr<juce::Slider> displaySlider;
    
    juce::String sliderName;
    BKLabel showName;
    
    BKTextEditor minValueTF;
    BKTextEditor maxValueTF;
    
    void setName(juce::String newName)    { sliderName = newName; showName.setText(sliderName, dontSendNotification); }
    juce::String getName()                { return sliderName; }
    void setToolTipString(juce::String newTip) {  showName.setTooltip(newTip);
                                            invisibleSlider.setTooltip(newTip);
                                            minValueTF.setTooltip(newTip);
                                            maxValueTF.setTooltip(newTip); }
    
    void setMinValue(double newval, NotificationType notify);
    void setMaxValue(double newval, NotificationType notify);
    void setIsMinAlwaysLessThanMax(bool im) { isMinAlwaysLessThanMax = im; }

    double getMinValue() { return sliderMin; }
    double getMaxValue() { return sliderMax; }
    
    void setDisplayValue(double newval) { displaySlider->setValue(newval); }
    void displaySliderVisible(bool vis) { displaySlider->setVisible(vis); }
    
    void setJustifyRight(bool jr)
    {
        justifyRight = jr;
        if (justifyRight) showName.setJustificationType(juce::Justification::bottomRight);
        else showName.setJustificationType(juce::Justification::bottomLeft);
    }
    
    inline void setText(BKRangeSliderType which, juce::String text)
    {
        if (which == BKRangeSliderMin)      minValueTF.setText(text, false);
        else if (which == BKRangeSliderMax) maxValueTF.setText(text, false);
    }
    
    inline juce::TextEditor* getTextEditor(BKRangeSliderType which)
    {
        if (which == BKRangeSliderMin) return &minValueTF;
        if (which == BKRangeSliderMax) return &maxValueTF;
        
        return nullptr;
    }
    
    inline void dismissTextEditor(bool setValue = false)
    {
        if (setValue)
        {
            textEditorReturnKeyPressed(minValueTF);
            textEditorReturnKeyPressed(maxValueTF);
        }
        else
        {
            textEditorEscapeKeyPressed(minValueTF);
            textEditorEscapeKeyPressed(maxValueTF);
        }
    }
    
    void checkValue(double newval);
    void rescaleMinSlider();
    void rescaleMaxSlider();
    
    void sliderValueChanged (juce::Slider *slider) override;
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;
    void textEditorFocusLost(juce::TextEditor& textEditor) override;
    void textEditorEscapeKeyPressed (juce::TextEditor& textEditor) override;
    void textEditorTextChanged(juce::TextEditor& textEditor) override;
    void resized() override;
    void sliderDragEnded(juce::Slider *slider) override;
    void mouseDown (const juce::MouseEvent &event) override;
    
    void setDim(float newAlpha);
    void setBright();
    
    class Listener
    {
        
    public:
        virtual ~Listener() {};
        
        virtual void BKRangeSliderValueChanged(juce::String name, double min, double max) = 0;
    };
    
    juce::ListenerList<Listener> listeners;
    void addMyListener(Listener* listener)     { listeners.add(listener);      }
    void removeMyListener(Listener* listener)  { listeners.remove(listener);   }
    
    
private:
    
    double sliderMin, sliderMax;
    double sliderDefaultMin, sliderDefaultMax;
    double sliderIncrement;
    
    bool newDrag;
    bool clickedOnMinSlider;
    bool isMinAlwaysLessThanMax;
    bool focusLostByEscapeKey;
    bool justifyRight;
    
    BKRangeMinSliderLookAndFeel minSliderLookAndFeel;
    BKRangeMaxSliderLookAndFeel maxSliderLookAndFeel;
    BKDisplaySliderLookAndFeel displaySliderLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKRangeSlider)
    
};



// ******************************************************************************************************************** //
// *******************************************  BKWaveDistanceUndertowSlider ****************************************** //
// ******************************************************************************************************************** //


class BKWaveDistanceUndertowSlider :
public juce::Component,
public juce::Slider::Listener,
public juce::TextEditor::Listener
#if JUCE_IOS
, public WantsBigOne
#endif
{
public:
    BKWaveDistanceUndertowSlider();
    ~BKWaveDistanceUndertowSlider()
    {
        wavedistanceSlider->setLookAndFeel(nullptr);
        undertowSlider->setLookAndFeel(nullptr);
        for(int i=0; i<maxSliders; i++)
        {
            juce::Slider* newSlider = displaySliders.getUnchecked(i);
            newSlider->setLookAndFeel(nullptr);
        }
    };
    
    std::unique_ptr<juce::Slider> wavedistanceSlider;
    std::unique_ptr<juce::Slider> undertowSlider;
    juce::OwnedArray<juce::Slider> displaySliders;
    
    juce::String wavedistanceSliderName;
    juce::String undertowSliderName;
    
    juce::String sliderName;
    BKLabel wavedistanceName;
    BKLabel undertowName;
    
    BKTextEditor wavedistanceValueTF;
    BKTextEditor undertowValueTF;
    
    void setName(juce::String newName)    { sliderName = newName; }
    juce::String getName()                { return sliderName; }
    
    void updateSliderPositions(juce::Array<int> newpositions);
    
    void sliderValueChanged (juce::Slider *slider) override {};
    void resized() override;
    void sliderDragEnded(juce::Slider *slider) override;
    void mouseDoubleClick (const juce::MouseEvent &event) override;
    
    void setWaveDistance(int newwavedist, NotificationType notify);
    void setUndertow(int newundertow, NotificationType notify);
    
    void setDim(float newAlpha);
    void setBright();
    
    inline void setText(NostalgicParameterType which, juce::String text)
    {
        //DBG("Nostalgic setText " + text);
        if (which == NostalgicUndertow)             undertowValueTF.setText(text, false);
        else if (which == NostalgicWaveDistance)    wavedistanceValueTF.setText(text, false);
    }
    
    inline void setUndertowTooltip(juce::String tip) {    undertowSlider->setTooltip(tip);
                                                    undertowName.setTooltip(tip);
                                                    undertowValueTF.setTooltip(tip); }
    
    inline void setWaveDistanceTooltip(juce::String tip) {    wavedistanceSlider->setTooltip(tip);
                                                        wavedistanceName.setTooltip(tip);
                                                        wavedistanceValueTF.setTooltip(tip); }
    
    inline juce::TextEditor* getTextEditor(NostalgicParameterType which)
    {
        if (which == NostalgicUndertow) return &undertowValueTF;
        
        if (which == NostalgicWaveDistance) return &wavedistanceValueTF;
        
        return nullptr;
    }
    
    inline void dismissTextEditor(bool setValue = false)
    {
        //DBG("nostalgic text editor dismissed");
        if (setValue)
        {
            textEditorReturnKeyPressed(wavedistanceValueTF);
            textEditorReturnKeyPressed(undertowValueTF);
        }
        else
        {
            textEditorEscapeKeyPressed(wavedistanceValueTF);
            textEditorEscapeKeyPressed(undertowValueTF);
        }
    }
    
    class Listener
    {
        
    public:
        
        //BKRangeSlider::Listener() {}
        virtual ~Listener() {};
        
        virtual void BKWaveDistanceUndertowSliderValueChanged(juce::String name, double wavedist, double undertow) = 0;
    };
    
    juce::ListenerList<Listener> listeners;
    void addMyListener(Listener* listener)     { listeners.add(listener);      }
    void removeMyListener(Listener* listener)  { listeners.remove(listener);   }

    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;
    void textEditorEscapeKeyPressed (juce::TextEditor& textEditor) override;
    void textEditorFocusLost(juce::TextEditor& textEditor) override;
    void textEditorTextChanged(juce::TextEditor& textEditor) override;
    
private:
    double sliderMin, sliderMax;
    double sliderIncrement;
    
    int maxSliders;
    
    bool newDrag;
    bool clickedOnMinSlider;
    
    juce::ImageComponent sampleImageComponent;
    
    bool focusLostByEscapeKey;
    
    //BKRangeMinSliderLookAndFeel minSliderLookAndFeel; //possibly need to remake for this
    //BKRangeMaxSliderLookAndFeel maxSliderLookAndFeel;
    BKMultiSliderLookAndFeel displaySliderLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKWaveDistanceUndertowSlider)
    
};



// ******************************************************************************************************************** //
// ************************************************** BKStackedSlider ************************************************* //
// ******************************************************************************************************************** //

class BKStackedSlider :
public juce::Component,
public juce::Slider::Listener,
public juce::TextEditor::Listener
#if JUCE_IOS
, public WantsBigOne
#endif
{
public:
    
    BKStackedSlider(juce::String sliderName, double min, double max, double defmin, double defmax, double def, double increment);
    
    ~BKStackedSlider()
    {
        topSlider->setLookAndFeel(nullptr);
        
        for(int i=0; i<numSliders; i++)
        {
            juce::Slider* newSlider = dataSliders.operator[](i);
            if(newSlider != nullptr)
            {
                newSlider->setLookAndFeel(nullptr);
            }
        }
    };
    
    void sliderValueChanged (juce::Slider *slider) override;
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;
    void textEditorFocusLost(juce::TextEditor& textEditor) override;
    void textEditorEscapeKeyPressed (juce::TextEditor& textEditor) override;
    void textEditorTextChanged(juce::TextEditor& textEditor) override;
    void mouseDown (const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    void mouseDoubleClick (const juce::MouseEvent &e) override;
    
    inline BKTextEditor* getTextEditor(void)
    {
        return editValsTextField.get();
    }
    
    inline void dismissTextEditor(bool setValue = false)
    {
        if (setValue)   textEditorReturnKeyPressed(*editValsTextField);
        else            textEditorEscapeKeyPressed(*editValsTextField);
    }
    
    void setTo(juce::Array<float> newvals, NotificationType newnotify);
    void setValue(juce::Array<float> newvals, NotificationType newnotify) { setTo(newvals, newnotify); }
    void resetRanges();
    int whichSlider();
    int whichSlider(const juce::MouseEvent& e);
    void addSlider(NotificationType newnotify);
    
    inline juce::String getText(void) { return editValsTextField->getText(); }
    inline void setText(juce::String text) { editValsTextField->setText(text, dontSendNotification); }
    
    void setName(juce::String newName)    { sliderName = newName; showName.setText(sliderName, dontSendNotification); }
    juce::String getName()                { return sliderName; }
    void setTooltip(juce::String newTip)  { topSlider->setTooltip(newTip); showName.setTooltip(newTip); }
    
    void resized() override;
    
    void setDim(float newAlpha);
    void setBright();
    
    class Listener
    {
        
    public:
        
        virtual ~Listener() {};
        
        virtual void BKStackedSliderValueChanged(juce::String name, juce::Array<float> val) = 0; //rewrite all this to pass "this" and check by slider ref instead of name?
    };
    
    juce::ListenerList<Listener> listeners;
    void addMyListener(Listener* listener)     { listeners.add(listener);      }
    void removeMyListener(Listener* listener)  { listeners.remove(listener);   }
    
    
private:
    
    std::unique_ptr<juce::Slider> topSlider; //user interacts with this
    juce::OwnedArray<juce::Slider> dataSliders;  //displays data, user controls with topSlider
    juce::Array<bool> activeSliders;
    
    std::unique_ptr<BKTextEditor> editValsTextField;
    
    int numSliders;
    int numActiveSliders;
    int clickedSlider;
    float clickedPosition;
    
    juce::String sliderName;
    BKLabel showName;
    bool justifyRight;
    
    BKMultiSliderLookAndFeel stackedSliderLookAndFeel;
    BKMultiSliderLookAndFeel topSliderLookAndFeel;
    
    double sliderMin, sliderMax, sliderMinDefault, sliderMaxDefault;
    double sliderDefault;
    double sliderIncrement;
    double currentDisplaySliderValue;
    
    bool focusLostByEscapeKey;
    bool focusLostByNumPad;
    bool mouseJustDown;
    
    juce::Array<float> getAllActiveValues();
    
    void showModifyPopupMenu();
    static void sliderModifyMenuCallback (const int result, BKStackedSlider* ss);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKStackedSlider)
};


// ******************************************************************************************************************** //
// **************************************************  BKADSRSlider ************************************************** //
// ******************************************************************************************************************** //


class BKADSRSlider :
public juce::Component,
public BKSingleSlider::Listener,
public BKTextButton::Listener
#if JUCE_IOS
, public WantsBigOne
#endif
{
public:
    BKADSRSlider(juce::String name);
    BKADSRSlider()
    {
        
    };
    
    ~BKADSRSlider()
    {
        attackSlider->setLookAndFeel(nullptr);
        decaySlider->setLookAndFeel(nullptr);
        sustainSlider->setLookAndFeel(nullptr);
        releaseSlider->setLookAndFeel(nullptr);
        adsrButton.setLookAndFeel(nullptr);
        
        setLookAndFeel(nullptr);
    };
    
    void setName(juce::String newName)    { sliderName = newName; showName.setText(sliderName, dontSendNotification); }
    juce::String getName()                { return sliderName; }
    void setToolTip(juce::String newTip)  { adsrButton.setTooltip(newTip); }
    
    void setButtonText(juce::String buttonText)
    {
        adsrButton.setButtonText(buttonText);
    }
    
    void setAttackValue(int newval, NotificationType notify)
    {
        attackSlider->setValue(newval, notify);
    }
    void setDecayValue(int newval, NotificationType notify)
    {
        decaySlider->setValue(newval, notify);
    }
    void setSustainValue(float newval, NotificationType notify)
    {
        sustainSlider->setValue(newval, notify);
    }
    void setReleaseValue(int newval, NotificationType notify)
    {
        releaseSlider->setValue(newval, notify);
    }
    
    void setValue(juce::Array<float> newvals, NotificationType notify)
    {
        setAttackValue(newvals[0], notify);
        setDecayValue(newvals[1], notify);
        setSustainValue(newvals[2], notify);
        setReleaseValue(newvals[3], notify);
        
        if(newvals.size() > 4)
        {
            if(newvals[4] > 0) setActive();
            else setPassive();
        }
    }
    
    int getAttackValue()    { return attackSlider->getValue(); }
    int getDecayValue()     { return decaySlider->getValue(); }
    float getSustainValue() { return sustainSlider->getValue(); }
    int getReleaseValue()   { return releaseSlider->getValue(); }
    
    void setIsButtonOnly(bool state) { isButtonOnly = state; }
    void setButtonToggle(bool state) { adsrButton.setToggleState(state, dontSendNotification);}
    bool getButtonToggle()           { return adsrButton.getToggleState(); }
    void setButtonMode(bool bmode)   { buttonMode = bmode; adsrButton.setVisible(bmode); }
    
    void setHighlighted() { adsrButton.setToggleState(false, dontSendNotification); adsrButton.setLookAndFeel(&highlightedADSRLookAndFeel); }
    void setActive() { adsrButton.setToggleState(false, dontSendNotification); adsrButton.setLookAndFeel(&activeADSRLookAndFeel);}
    void setPassive() { adsrButton.setToggleState(true, dontSendNotification); adsrButton.setLookAndFeel(&passiveADSRLookAndFeel); }
    
    void setJustifyRight(bool jr)
    {
        justifyRight = jr;
        if (justifyRight) showName.setJustificationType(juce::Justification::bottomRight);
        else showName.setJustificationType(juce::Justification::bottomLeft);
    }
    
    void BKSingleSliderValueChanged(BKSingleSlider* slider, juce::String name, double val) override;
    void buttonStateChanged (juce::Button*) override;
    void buttonClicked (juce::Button*) override;
    void mouseDown (const juce::MouseEvent &event) override {};
    void mouseUp (const juce::MouseEvent &event) override;

    
    void resized() override;
    
    void setDim(float newAlpha);
    void setBright();
    
    class Listener
    {
    public:
        virtual ~Listener() {};
        
        virtual void BKADSRSliderValueChanged(juce::String name, int attack, int decay, float sustain, int release) = 0;
        virtual void BKADSRButtonStateChanged(juce::String name, bool mod, bool state) = 0;
    };
    
    juce::ListenerList<Listener> listeners;
    void addMyListener(Listener* listener)     { listeners.add(listener);      }
    void removeMyListener(Listener* listener)  { listeners.remove(listener);   }
    
    BKTextButton adsrButton;
    
    
private:
    
    std::unique_ptr<BKSingleSlider> attackSlider;
    std::unique_ptr<BKSingleSlider> decaySlider;
    std::unique_ptr<BKSingleSlider> sustainSlider;
    std::unique_ptr<BKSingleSlider> releaseSlider;
    
    juce::String sliderName;
    BKLabel showName;
    
    BKButtonAndMenuLAF highlightedADSRLookAndFeel;
    BKButtonAndMenuLAF activeADSRLookAndFeel;
    BKButtonAndMenuLAF passiveADSRLookAndFeel;

    bool justifyRight;
    bool isButtonOnly;
    
    bool buttonMode; //true by default; appears as button that can be clicked to open juce::ADSR sliders

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKADSRSlider)
    
};




#endif  // BKSLIDER_H_INCLUDED

