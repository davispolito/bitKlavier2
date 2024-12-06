//
// Created by Dan Trueman on 10/22/24.
//

#include "BKSliders.h"

/**
 * move this stuff somewhere else
 */

juce::String BKfloatArrayToString(juce::Array<float> arr)
{
    juce::String s = "";
    for (auto key : arr)
    {
        s.append(juce::String(key), 6);
        s.append(" ", 1);
    }
    return s;
}

juce::Array<float> BKstringToFloatArray(juce::String s)
{
    juce::Array<float> arr = juce::Array<float>();

    juce::String temp = "";
    bool inNumber = false;

    juce::String::CharPointerType c = s.getCharPointer();

    juce::juce_wchar prd = '.';
    juce::juce_wchar dash = '-';
    juce::juce_wchar slash = '/'; // blank: put a zero in

    int prdCnt = 0;

    // DEBUG
    for (int i = 0; i < (s.length()+1); i++)
    {
        juce::juce_wchar c1 = c.getAndAdvance();

        bool isPrd = !juce::CharacterFunctions::compare(c1, prd);
        bool isDash = !juce::CharacterFunctions::compare(c1, dash);
        bool isSlash = !juce::CharacterFunctions::compare(c1, slash);

        if (isPrd) prdCnt += 1;

        bool isNumChar = juce::CharacterFunctions::isDigit(c1) || isPrd || isDash;

        if (!isNumChar)
        {
            if (inNumber)
            {
                arr.add(temp.getFloatValue());
                temp = "";
            }

            // slash indicates a zero slot
            if (isSlash) {
                arr.add(0.);
                temp = "";
            }

            inNumber = false;
            continue;
        }
        else
        {
            inNumber = true;

            temp += c1;
        }
    }

    return arr;
}

// ******************************************************************************************************************** //
// ************************************************  BKStackedSlider ************************************************** //
// ******************************************************************************************************************** //

BKStackedSlider::BKStackedSlider(
    juce::String sliderName,
    double min,
    double max,
    double defmin,
    double defmax,
    double def,
    double increment):
           sliderName(sliderName),
           sliderMin(min),
           sliderMax(max),
           sliderMinDefault(defmin),
           sliderMaxDefault(defmax),
           sliderDefault(def),
           sliderIncrement(increment)
{

    showName.setText(sliderName, juce::dontSendNotification);
    showName.setInterceptsMouseClicks(false, true);
    addAndMakeVisible(showName);

    editValsTextField = std::make_unique<juce::TextEditor>();
    editValsTextField->setMultiLine(true);
    editValsTextField->setName("PARAMTXTEDIT");
    editValsTextField->addListener(this);
    editValsTextField->setColour(juce::TextEditor::highlightColourId, juce::Colours::darkgrey);
#if JUCE_IOS
    editValsTextField->setReadOnly(true);
    editValsTextField->setCaretVisible(true);
#endif
    addAndMakeVisible(*editValsTextField);
    editValsTextField->setVisible(false);

    numSliders = 12;
    numActiveSliders = 1;

    for(int i=0; i<numSliders; i++)
    {
        dataSliders.add(new juce::Slider);
        juce::Slider* newSlider = dataSliders.getLast();

        newSlider->setSliderStyle(juce::Slider::LinearBar);
        newSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        newSlider->setRange(sliderMin, sliderMax, sliderIncrement);
        newSlider->setValue(sliderDefault, juce::dontSendNotification);
        newSlider->setLookAndFeel(&stackedSliderLookAndFeel);
        //newSlider->addListener(this);
        addAndMakeVisible(newSlider);
        if(i>0) {
            newSlider->setVisible(false);
            activeSliders.insert(i, false);
        }
        else activeSliders.insert(0, true);
    }

    topSlider = std::make_unique<juce::Slider>();
    topSlider->setSliderStyle(juce::Slider::LinearBar);
    topSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, true, 0,0);
    //topSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 50,50);
    topSlider->setRange(sliderMin, sliderMax, sliderIncrement);
    topSlider->setValue(sliderDefault, juce::dontSendNotification);
    topSlider->addListener(this);
    topSlider->addMouseListener(this, true);
    topSlider->setLookAndFeel(&topSliderLookAndFeel);
    topSlider->setAlpha(0.);
    addAndMakeVisible(*topSlider);

    topSliderLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colour::greyLevel (0.8f).contrasting().withAlpha (0.0f));
    stackedSliderLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::goldenrod.withMultipliedAlpha(0.95));

}

void BKStackedSlider::setDim(float alphaVal)
{
    showName.setAlpha(alphaVal);
    //topSlider->setAlpha(alphaVal);

    for(int i=0; i<numSliders; i++)
    {
        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            if(activeSliders.getUnchecked(i))
            {
                newSlider->setAlpha(alphaVal);
            }
        }
    }
}

void BKStackedSlider::setBright()
{
    showName.setAlpha(1.);
    //topSlider->setAlpha(1.);

    for(int i=0; i<numSliders; i++)
    {
        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            if(activeSliders.getUnchecked(i))
            {
                newSlider->setAlpha(1.);
            }
        }
    }
}

void BKStackedSlider::sliderValueChanged (juce::Slider *slider)
{

}

void BKStackedSlider::addSlider(juce::NotificationType newnotify)
{
    juce::Array<float> sliderVals = getAllActiveValues();
    //sliderVals.add(sliderDefault);
    sliderVals.add(topSlider->proportionOfLengthToValue((double)clickedPosition / getWidth()));
    setTo(sliderVals, newnotify);
    topSlider->setValue(topSlider->proportionOfLengthToValue((double)clickedPosition / getWidth()), juce::dontSendNotification);

//    listeners.call(&BKStackedSlider::Listener::BKStackedSliderValueChanged,
//        getName(),
//        getAllActiveValues());
}

void BKStackedSlider::setTo(juce::Array<float> newvals, juce::NotificationType newnotify)
{

    int slidersToActivate = newvals.size();
    if(slidersToActivate > numSliders) slidersToActivate = numSliders;
    if(slidersToActivate <= 0)
    {
        slidersToActivate = 1;
        newvals.add(sliderDefault);
    }

    //activate sliders
    for(int i=0; i<slidersToActivate; i++)
    {

        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            if(newvals.getUnchecked(i) > sliderMax)
                newSlider->setRange(sliderMin, newvals.getUnchecked(i), sliderIncrement);

            if(newvals.getUnchecked(i) < sliderMin)
                newSlider->setRange(newvals.getUnchecked(i), sliderMax, sliderIncrement);

            newSlider->setValue(newvals.getUnchecked(i));
            newSlider->setVisible(true);
        }

        activeSliders.set(i, true);
    }

    //deactivate unused sliders
    for(int i=slidersToActivate; i<numSliders; i++)
    {

        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            newSlider->setValue(sliderDefault);
            newSlider->setVisible(false);
        }

        activeSliders.set(i, false);
    }

    //make sure there is one!
    if(slidersToActivate <= 0)
    {
        dataSliders.getFirst()->setValue(sliderDefault);
        activeSliders.set(0, true);
    }

    resetRanges();

    topSlider->setValue(dataSliders.getFirst()->getValue(), juce::dontSendNotification);
}


void BKStackedSlider::mouseDown (const juce::MouseEvent &event)
{
    if(event.mouseWasClicked())
    {
        clickedSlider = whichSlider();
        clickedPosition = event.x;

        mouseJustDown = true;

        if(event.mods.isCtrlDown())
        {
            showModifyPopupMenu();
        }

    }
}


void BKStackedSlider::mouseDrag(const juce::MouseEvent& e)
{
    //DBG("BKStacked Slider: mouseDrag");
    if(!mouseJustDown)
    {
        juce::Slider* currentSlider = dataSliders.operator[](clickedSlider);
        if(currentSlider != nullptr)
        {
            if(e.mods.isShiftDown())
            {
                currentSlider->setValue(round(topSlider->getValue()));
                topSlider->setValue(round(topSlider->getValue()));
            }
            else {
                currentSlider->setValue(topSlider->getValue(), juce::sendNotification);
                //DBG("slider val = " + juce::String(topSlider->getValue()));
            }
        }
    }
    else mouseJustDown = false;
}

void BKStackedSlider::mouseUp(const juce::MouseEvent& e)
{
    DBG("BKStackedSlider::mouseUp");

//    listeners.call(&BKStackedSlider::Listener::BKStackedSliderValueChanged,
//        getName(),
//        getAllActiveValues());
}

void BKStackedSlider::mouseMove(const juce::MouseEvent& e)
{
    //topSlider->setValue(topSlider->proportionOfLengthToValue((double)e.x / getWidth()), dontSendNotification);
//    topSlider->setValue(dataSliders.getUnchecked(whichSlider(e))->getValue());
//
//    for(int i=0; i<dataSliders.size(); i++)
//    {
//        if(dataSliders.getUnchecked(whichSlider(e)) == dataSliders.getUnchecked(i))
//            dataSliders.getUnchecked(i)->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 50);
//        else
//            dataSliders.getUnchecked(i)->setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
//    }
}


void BKStackedSlider::mouseDoubleClick (const juce::MouseEvent &e)
{
    juce::StringArray tokens;
    tokens.addTokens(BKfloatArrayToString(getAllActiveValues()), false); //arrayFloatArrayToString
    int startPoint = 0;
    int endPoint;

    int which = whichSlider();

    for(int i=0; i < which; i++) {
        startPoint += tokens[i].length() + 1;
    }
    endPoint = startPoint + tokens[which].length();

    editValsTextField->setVisible(true);
    editValsTextField->toFront(true);
    editValsTextField->grabKeyboardFocus();
    editValsTextField->setText(BKfloatArrayToString(getAllActiveValues()), juce::dontSendNotification); //arrayFloatArrayToString

    juce::Range<int> highlightRange(startPoint, endPoint);
    editValsTextField->setHighlightedRegion(highlightRange);

    focusLostByEscapeKey = false;
}

void BKStackedSlider::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    if(textEditor.getName() == editValsTextField->getName())
    {
        editValsTextField->setVisible(false);
        editValsTextField->toBack();

        setTo(BKstringToFloatArray(textEditor.getText()), juce::sendNotification);
        clickedSlider = 0;
        resized();

//        listeners.call(&BKStackedSlider::Listener::BKStackedSliderValueChanged,
//            getName(),
//            getAllActiveValues());

    }
}

void BKStackedSlider::textEditorFocusLost(juce::TextEditor& textEditor)
{
#if !JUCE_IOS
    if(!focusLostByEscapeKey) {
        textEditorReturnKeyPressed(textEditor);
    }
#endif
}

void BKStackedSlider::textEditorEscapeKeyPressed (juce::TextEditor& textEditor)
{
    if(textEditor.getName() == editValsTextField->getName())
    {
        focusLostByEscapeKey = true;
        editValsTextField->setVisible(false);
        editValsTextField->toBack();
        unfocusAllComponents();
    }
}

void BKStackedSlider::textEditorTextChanged(juce::TextEditor& textEditor)
{
#if JUCE_IOS
    if (hasBigOne)
    {
        hasBigOne = false;
        textEditorReturnKeyPressed(textEditor);
    }
#endif
}

void BKStackedSlider::resetRanges()
{
    double sliderMinTemp = sliderMinDefault;
    double sliderMaxTemp = sliderMaxDefault;

    for(int i = 0; i<dataSliders.size(); i++)
    {
        juce::Slider* currentSlider = dataSliders.operator[](i);
        if(currentSlider != nullptr)
        {
            if(currentSlider->getValue() > sliderMaxTemp) sliderMaxTemp = currentSlider->getValue();
            if(currentSlider->getValue() < sliderMinTemp) sliderMinTemp = currentSlider->getValue();
        }
    }

    if( (sliderMax != sliderMaxTemp) || sliderMin != sliderMinTemp)
    {
        sliderMax = sliderMaxTemp;
        sliderMin = sliderMinTemp;

        for(int i = 0; i<dataSliders.size(); i++)
        {
            juce::Slider* currentSlider = dataSliders.operator[](i);
            if(currentSlider != nullptr)
            {
                currentSlider->setRange(sliderMin, sliderMax, sliderIncrement);
            }
        }

        topSlider->setRange(sliderMin, sliderMax, sliderIncrement);
    }
}


juce::Array<float> BKStackedSlider::getAllActiveValues()
{
    juce::Array<float> currentVals;

    for(int i=0; i<dataSliders.size(); i++)
    {
        juce::Slider* currentSlider = dataSliders.operator[](i);
        if(currentSlider != nullptr)
        {
            if(activeSliders.getUnchecked(i))
                currentVals.add(currentSlider->getValue());
        }

    }

    return currentVals;
}

int BKStackedSlider::whichSlider()
{
    float refDistance;
    int whichSub = 0;

    juce::Slider* refSlider = dataSliders.getFirst();
    refDistance = fabs(refSlider->getValue() - topSlider->getValue());

    for(int i=1; i<dataSliders.size(); i++)
    {
        if(activeSliders.getUnchecked(i))
        {
            juce::Slider* currentSlider = dataSliders.operator[](i);
            if(currentSlider != nullptr) {
                float tempDistance = fabs(currentSlider->getValue() - topSlider->getValue());
                if(tempDistance < refDistance)
                {
                    whichSub = i;
                    refDistance = tempDistance;
                }
            }
        }
    }
    //DBG("whichSlider = " + String(whichSub));

    return whichSub;
}

int BKStackedSlider::whichSlider(const juce::MouseEvent& e)
{
    float refDistance;
    float topSliderVal = topSlider->proportionOfLengthToValue((double)e.x / getWidth());

    int whichSub = 0;

    juce::Slider* refSlider = dataSliders.getFirst();
    refDistance = fabs(refSlider->getValue() - topSliderVal);

    for(int i=1; i<dataSliders.size(); i++)
    {
        if(activeSliders.getUnchecked(i))
        {
            juce::Slider* currentSlider = dataSliders.operator[](i);
            if(currentSlider != nullptr) {
                float tempDistance = fabs(currentSlider->getValue() - topSliderVal);
                if(tempDistance < refDistance)
                {
                    whichSub = i;
                    refDistance = tempDistance;
                }
            }
        }
    }
    //DBG("whichSlider = " + String(whichSub));

    return whichSub;
}

void BKStackedSlider::showModifyPopupMenu()
{
    juce::PopupMenu m;
    m.addItem (1, juce::translate ("add transposition"), true, false);
    m.addSeparator();

    m.showMenuAsync (juce::PopupMenu::Options(),
        juce::ModalCallbackFunction::forComponent (sliderModifyMenuCallback, this));
}

void BKStackedSlider::sliderModifyMenuCallback (const int result, BKStackedSlider* ss)
{
    if (ss == nullptr)
    {
        juce::PopupMenu::dismissAllActiveMenus();
        return;
    }

    switch (result)
    {
        case 1:   ss->addSlider(juce::sendNotification); break;

        default:  break;
    }
}


void BKStackedSlider::resized ()
{
    juce::Rectangle<int> area (getLocalBounds());

    showName.setBounds(area.toNearestInt());
    showName.setJustificationType(juce::Justification::topRight);
    showName.toFront(false);

    topSlider->setBounds(area);

    editValsTextField->setBounds(area);
    editValsTextField->setVisible(false);

    for(int i=0; i<numSliders; i++)
    {
        juce::Slider* newSlider = dataSliders.getUnchecked(i);
        newSlider->setBounds(area);
    }

}

// ********************************************************************************************* //
// **********************************  BKRangeSlider ******************************************* //
// ********************************************************************************************* //

BKRangeSlider::BKRangeSlider (juce::String name, double min, double max, double defmin, double defmax, double increment):
                               sliderName(name),
                               sliderMin(min),
                               sliderMax(max),
                               sliderDefaultMin(defmin),
                               sliderDefaultMax(defmax),
                               sliderIncrement(increment)
{

    justifyRight = true;

    showName.setText(sliderName, juce::dontSendNotification);
    if(justifyRight) showName.setJustificationType(juce::Justification::bottomRight);
    else showName.setJustificationType(juce::Justification::bottomLeft);
    addAndMakeVisible(showName);

    minValueTF.setText(juce::String(sliderDefaultMin));
    minValueTF.setName("minvalue");
    minValueTF.addListener(this);
    minValueTF.setSelectAllWhenFocused(true);
    minValueTF.setColour(juce::TextEditor::highlightColourId, juce::Colours::darkgrey);
    addAndMakeVisible(minValueTF);

    maxValueTF.setText(juce::String(sliderDefaultMax));
    maxValueTF.setName("maxvalue");
    maxValueTF.addListener(this);
    maxValueTF.setSelectAllWhenFocused(true);
    maxValueTF.setColour(juce::TextEditor::highlightColourId, juce::Colours::darkgrey);
    addAndMakeVisible(maxValueTF);

    minSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    minSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    minSlider.setRange(sliderMin, sliderMax, sliderIncrement);
    minSlider.setValue(sliderDefaultMin, juce::dontSendNotification);
    minSlider.addListener(this);
    minSlider.setLookAndFeel(&minSliderLookAndFeel);
    //minSlider.setInterceptsMouseClicks(false, true);
    //minSliderLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colour::fromRGBA(55, 105, 250, 50));
    addAndMakeVisible(minSlider);

    maxSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    maxSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    maxSlider.setRange(sliderMin, sliderMax, sliderIncrement);
    maxSlider.setValue(sliderDefaultMax, juce::dontSendNotification);
    maxSlider.addListener(this);
    maxSlider.setLookAndFeel(&maxSliderLookAndFeel);
    //maxSlider.setInterceptsMouseClicks(false, true);
    //maxSliderLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colour::greyLevel (0.8f).contrasting().withAlpha (0.13f));
    //maxSliderLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colour::fromRGBA(55, 105, 250, 50));
    addAndMakeVisible(maxSlider);

    invisibleSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    invisibleSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    invisibleSlider.setRange(sliderMin, sliderMax, sliderIncrement);
    invisibleSlider.setValue(sliderDefaultMin, juce::dontSendNotification);
    invisibleSlider.setAlpha(0.0);
    invisibleSlider.addListener(this);
    invisibleSlider.addMouseListener(this, true);
    //invisibleSlider.setInterceptsMouseClicks(true, true);
    addAndMakeVisible(invisibleSlider);

    newDrag = false;
    isMinAlwaysLessThanMax = false;

    displaySlider = std::make_unique<juce::Slider>();
    displaySlider->setRange(min, max, increment);
    displaySlider->setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    displaySlider->setLookAndFeel(&displaySliderLookAndFeel);
    displaySlider->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*displaySlider);

#if JUCE_IOS
    maxValueTF.setReadOnly(true);
    maxValueTF.addMouseListener(this, true);

    minValueTF.setReadOnly(true);
    minValueTF.addMouseListener(this, true);
#endif
}

void BKRangeSlider::setDim(float alphaVal)
{
    minSlider.setAlpha(alphaVal);
    maxSlider.setAlpha(alphaVal);
    showName.setAlpha(alphaVal);
    minValueTF.setAlpha(alphaVal);
    maxValueTF.setAlpha(alphaVal);
}

void BKRangeSlider::setBright()
{
    minSlider.setAlpha(1.);
    maxSlider.setAlpha(1.);
    showName.setAlpha(1.);
    minValueTF.setAlpha(1.);
    maxValueTF.setAlpha(1.);
}

void BKRangeSlider::setMinValue(double newval, juce::NotificationType notify)
{
    checkValue(newval);
    minSlider.setValue(newval, notify);
    minValueTF.setText(juce::String(minSlider.getValue()), juce::dontSendNotification);
    rescaleMinSlider();
    // displaySlider->setRange(sliderMin, sliderMax, sliderIncrement);
}

void BKRangeSlider::setMaxValue(double newval, juce::NotificationType notify)
{
    checkValue(newval);
    maxSlider.setValue(newval, notify);
    maxValueTF.setText(juce::String(maxSlider.getValue()), juce::dontSendNotification);
    rescaleMaxSlider();
}

void BKRangeSlider::sliderValueChanged (juce::Slider *slider)
{
    if(slider == &invisibleSlider)
    {
        if(newDrag)
        {
            if(!clickedOnMinSlider)
            {
                //maxSlider.setValue(invisibleSlider.getValue(), juce::dontSendNotification);
                maxSlider.setValue(invisibleSlider.getValue(), juce::sendNotification);
                maxValueTF.setText(juce::String(maxSlider.getValue()), juce::dontSendNotification);
                if(isMinAlwaysLessThanMax)
                    if(maxSlider.getValue() < minSlider.getValue())
                        setMinValue(maxSlider.getValue(), juce::dontSendNotification);
                //displaySlider->setRange(0,invisibleSlider.getValue(), sliderIncrement);
            }
            else
            {
                //minSlider.setValue(invisibleSlider.getValue(), juce::dontSendNotification);
                minSlider.setValue(invisibleSlider.getValue(), juce::sendNotification);
                minValueTF.setText(juce::String(minSlider.getValue()), juce::dontSendNotification);
                if(isMinAlwaysLessThanMax)
                    if(minSlider.getValue() > maxSlider.getValue())
                        setMaxValue(minSlider.getValue(), juce::dontSendNotification);
            }

//            DBG("BKRangeSlider::sliderValueChanged: " + getName() + " " + juce::String(minSlider.getValue()) + " " + juce::String(maxSlider.getValue()));
//            listeners.call(&BKRangeSlider::Listener::BKRangeSliderValueChanged,
//                getName(),
//                minSlider.getValue(),
//                maxSlider.getValue());
        }
    }
}

void BKRangeSlider::mouseDown (const juce::MouseEvent &event)
{
    juce::Component* ec = event.eventComponent;

    if (ec == &invisibleSlider)
    {
        if(event.mouseWasClicked())
        {
            if(event.y > invisibleSlider.getHeight() / 2.)
            {
                clickedOnMinSlider = true;
            }
            else
            {
                clickedOnMinSlider = false;
            }

            newDrag = true;
        }

        unfocusAllComponents();
    }

#if JUCE_IOS
    else if (ec == &minValueTF)
    {
        hasBigOne = true;
        WantsBigOne::listeners.call(&WantsBigOne::Listener::iWantTheBigOne, &minValueTF, "cluster min");
    }
    else if (ec == &maxValueTF)
    {
        hasBigOne = true;
        WantsBigOne::listeners.call(&WantsBigOne::Listener::iWantTheBigOne, &maxValueTF, "cluster max");
    }
#endif

}

void BKRangeSlider::sliderDragEnded(juce::Slider *slider)
{
    newDrag = false;
    unfocusAllComponents();
}


void BKRangeSlider::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    double newval = textEditor.getText().getDoubleValue();

    //adjusts min/max of sldiers as needed
    checkValue(newval);

    if(textEditor.getName() == "minvalue")
    {
        minSlider.setValue(newval, juce::sendNotification);
        if(isMinAlwaysLessThanMax)
            if(minSlider.getValue() > maxSlider.getValue())
                setMaxValue(minSlider.getValue(), juce::dontSendNotification);
        rescaleMinSlider();
    }
    else if(textEditor.getName() == "maxvalue")
    {
        maxSlider.setValue(newval, juce::sendNotification);
        if(isMinAlwaysLessThanMax)
            if(maxSlider.getValue() < minSlider.getValue())
                setMinValue(maxSlider.getValue(), juce::dontSendNotification);
        rescaleMaxSlider();
    }

    unfocusAllComponents();

//    DBG("BKRangeSlider::textEditorReturnKeyPressed: " + getName() + " " + juce::String(minSlider.getValue()) + " " + juce::String(maxSlider.getValue()));
//    listeners.call(&BKRangeSlider::Listener::BKRangeSliderValueChanged,
//        getName(),
//        minSlider.getValue(),
//        maxSlider.getValue());
}

void BKRangeSlider::textEditorTextChanged(juce::TextEditor& textEditor)
{
    focusLostByEscapeKey = false;

#if JUCE_IOS
    if (hasBigOne)
    {
        hasBigOne = false;
        textEditorReturnKeyPressed(textEditor);
    }
#endif
}

void BKRangeSlider::textEditorEscapeKeyPressed (juce::TextEditor& textEditor)
{
    focusLostByEscapeKey = true;
    unfocusAllComponents();
}

void BKRangeSlider::textEditorFocusLost(juce::TextEditor& textEditor)
{
#if !JUCE_IOS
    if(!focusLostByEscapeKey)
    {
        textEditorReturnKeyPressed(textEditor);
    }
#endif
}

void BKRangeSlider::checkValue(double newval)
{
    if(newval > maxSlider.getMaximum())
    {
        maxSlider.setRange(minSlider.getMinimum(), newval, sliderIncrement);
        minSlider.setRange(minSlider.getMinimum(), newval, sliderIncrement);
        invisibleSlider.setRange(minSlider.getMinimum(), newval, sliderIncrement);
    }

    if(newval < minSlider.getMinimum())
    {
        maxSlider.setRange(newval, maxSlider.getMaximum(), sliderIncrement);
        minSlider.setRange(newval, maxSlider.getMaximum(), sliderIncrement);
        invisibleSlider.setRange(newval, maxSlider.getMaximum(), sliderIncrement);
    }
}

void BKRangeSlider::rescaleMinSlider()
{
    if(minSlider.getMinimum() < sliderMin &&
        minSlider.getValue() > sliderMin &&
        maxSlider.getValue() > sliderMin)
    {
        maxSlider.setRange(sliderMin, maxSlider.getMaximum(), sliderIncrement);
        minSlider.setRange(sliderMin, maxSlider.getMaximum(), sliderIncrement);
        invisibleSlider.setRange(sliderMin, maxSlider.getMaximum(), sliderIncrement);
    }
}

void BKRangeSlider::rescaleMaxSlider()
{

    if(maxSlider.getMaximum() > sliderMax &&
        maxSlider.getValue() < sliderMax &&
        minSlider.getValue() < sliderMax)
    {
        maxSlider.setRange(minSlider.getMinimum(), sliderMax, sliderIncrement);
        minSlider.setRange(minSlider.getMinimum(), sliderMax, sliderIncrement);
        invisibleSlider.setRange(minSlider.getMinimum(), sliderMax, sliderIncrement);
    }
}

void BKRangeSlider::resized()
{
    juce::Rectangle<int> area (getLocalBounds());
    juce::Rectangle<int> topSlab (area.removeFromTop(gComponentTextFieldHeight));

    if(justifyRight)
    {
        topSlab.removeFromRight(5);
        maxValueTF.setBounds(topSlab.removeFromRight(75));
        topSlab.removeFromRight(gXSpacing);
        minValueTF.setBounds(topSlab.removeFromLeft(75));
        showName.setBounds(topSlab.removeFromRight(getWidth() - 150));
    }
    else
    {
        topSlab.removeFromLeft(5);
        minValueTF.setBounds(topSlab.removeFromLeft(75));
        topSlab.removeFromLeft(gXSpacing);
        maxValueTF.setBounds(topSlab.removeFromLeft(75));
        showName.setBounds(topSlab.removeFromLeft(getWidth() - 150));
    }

    juce::Rectangle<int> sliderArea (area.removeFromTop(40));
    minSlider.setBounds(sliderArea);
    maxSlider.setBounds(sliderArea);
    invisibleSlider.setBounds(sliderArea);

    juce::Rectangle<int> displaySliderArea = maxSlider.getBounds();
    displaySliderArea.reduce(8, 0);
    displaySlider->setBounds(displaySliderArea.removeFromBottom(8));
}