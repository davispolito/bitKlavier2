//
// Created by Dan Trueman on 10/22/24.
//

#ifndef BITKLAVIER2_BKSLIDERS_H
#define BITKLAVIER2_BKSLIDERS_H

#endif //BITKLAVIER2_BKSLIDERS_H

#include "FullInterface.h"
#include "PreparationSection.h"

/*
class BKButtonAndMenuLAF : public juce::LookAndFeel_V4, public juce::DeletedAtShutdown
{

public:

    BKButtonAndMenuLAF() :
                           comboBoxJustification (juce::Justification::centredLeft)
    {

        setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
        setColour(juce::ComboBox::textColourId, juce::Colours::antiquewhite);
        setColour(juce::ComboBox::buttonColourId, juce::Colours::black);
        setColour(juce::ComboBox::outlineColourId, juce::Colours::antiquewhite);
        setColour(juce::ComboBox::arrowColourId, juce::Colours::antiquewhite);

        setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black);
        setColour(juce::PopupMenu::textColourId, juce::Colours::antiquewhite);

        setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        setColour(juce::TextButton::textColourOffId, juce::Colours::antiquewhite);
        setColour(juce::TextButton::textColourOnId, juce::Colours::antiquewhite);
        setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
        setColour(juce::TextEditor::textColourId, juce::Colours::antiquewhite);
        setColour(juce::TextEditor::outlineColourId, juce::Colours::antiquewhite);

        setColour(juce::Label::textColourId, juce::Colours::antiquewhite);

        setColour(juce::Slider::trackColourId, juce::Colours::burlywood.withMultipliedBrightness(0.4));
        setColour(juce::Slider::thumbColourId, juce::Colours::lightgrey);
        setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);

        setColour(juce::ToggleButton::textColourId, juce::Colours::antiquewhite);

        toggleTextToRight = true;

        //setColour(juce::LookAndFeel_V4::ColourScheme::outline, Colours::black);
        //getCurrentColourScheme().setUIColour(juce::LookAndFeel_V4::ColourScheme::widgetBackground, Colours::yellow);

    }

    ~BKButtonAndMenuLAF()
    {
        //         setDefaultLookAndFeel(nullptr);
    }

    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;

    void positionComboBoxText (juce::ComboBox& box, juce::Label& label) override;
    void drawComboBox (juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox& box) override;

    void drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
        const bool isSeparator, const bool isActive,
        const bool isHighlighted, const bool isTicked,
        const bool hasSubMenu, const juce::String& text,
        const juce::String& shortcutKeyText,
        const juce::Drawable* icon, const juce::Colour* const textColourToUse) override;

    void setComboBoxJustificationType (juce::Justification justification)    { comboBoxJustification = justification; }
    void setToggleBoxTextToRightBool (bool ttr)    { toggleTextToRight = ttr; }
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
        bool isMouseOverButton, bool isButtonDown) override;

    juce::Font getComboBoxFont (juce::ComboBox&) override;
    juce::Font getPopupMenuFont (void) override;
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight)  override;
    juce::Font getLabelFont (juce::Label&) override;
    int getDefaultMenuBarHeight() override;
    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override;

private:
    juce::Justification comboBoxJustification;
    bool toggleTextToRight;
};

class BKMultiSliderLookAndFeel : public BKButtonAndMenuLAF
{

public:

    BKMultiSliderLookAndFeel()
    {
        setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey.withMultipliedAlpha(0.25));
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgrey.withMultipliedAlpha(0.1));
    }
    ~BKMultiSliderLookAndFeel() {}


    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};
 */

/**
 * BKStacked Slider
 *
 * for having multiple simultaneous overlaid sliders
 * main application: transposition sliders in Direct and Nostalgic
 *
 */
class BKStackedSlider :
    public juce::Component,
    public juce::Slider::Listener,
    public juce::TextEditor::Listener
{
public:

    BKStackedSlider(juce::String sliderName, double min, double max, double defmin, double defmax, double def, double increment);

    ~BKStackedSlider()
    {
        /*
        topSlider->setLookAndFeel(nullptr);

        for(int i=0; i<numSliders; i++)
        {
            juce::Slider* newSlider = dataSliders.operator[](i);
            if(newSlider != nullptr)
            {
                newSlider->setLookAndFeel(nullptr);
            }
        }
         */
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

    inline juce::TextEditor* getTextEditor(void)
    {
        return editValsTextField.get();
    }

    inline void dismissTextEditor(bool setValue = false)
    {
        if (setValue)   textEditorReturnKeyPressed(*editValsTextField);
        else            textEditorEscapeKeyPressed(*editValsTextField);
    }

    void setTo(juce::Array<float> newvals, juce::NotificationType newnotify);
    void setValue(juce::Array<float> newvals, juce::NotificationType newnotify) { setTo(newvals, newnotify); }
    void resetRanges();
    int whichSlider();
    int whichSlider(const juce::MouseEvent& e);
    void addSlider(juce::NotificationType newnotify);

    inline juce::String getText(void) { return editValsTextField->getText(); }
    inline void setText(juce::String text) { editValsTextField->setText(text, juce::dontSendNotification); }

    void setName(juce::String newName)    { sliderName = newName; showName.setText(sliderName, juce::dontSendNotification); }
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

    std::unique_ptr<juce::TextEditor> editValsTextField;

    int numSliders;
    int numActiveSliders;
    int clickedSlider;
    float clickedPosition;

    juce::String sliderName;
    juce::Label showName;
    bool justifyRight;

    //BKMultiSliderLookAndFeel stackedSliderLookAndFeel;
    //BKMultiSliderLookAndFeel topSliderLookAndFeel;

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