//
// Created by Davis Polito on 6/24/24.
//

#ifndef BITKLAVIER2_KEYMAPPREPARATION_H
#define BITKLAVIER2_KEYMAPPREPARATION_H

#include "KeymapProcessor.h"
#include "PreparationSection.h"
#include "popup_browser.h"

class OpenGlSlider;

class MidiInputSelectorComponentListBox : public ListBox,
                                                                              private ListBoxModel,
private ChangeListener,  public tracktion::engine::ValueTreeObjectList<bitklavier::MidiDeviceWrapper>
{
public:
    MidiInputSelectorComponentListBox (MidiInputCallback& midi, const ValueTree &v,
                                       AudioDeviceManager& manager)
            : ListBox ({}, nullptr),
            midi(midi),
            manager(manager),
              tracktion::engine::ValueTreeObjectList<bitklavier::MidiDeviceWrapper>(v)
    {
        updateDevices();
        setModel (this);
        setOutlineThickness (1);
    }

    bitklavier::MidiDeviceWrapper* createNewObject(const juce::ValueTree& v) override
    {
        return new bitklavier::MidiDeviceWrapper(v);
    }
    void deleteObject (bitklavier::MidiDeviceWrapper* at) override
    {
        manager.removeMidiInputDeviceCallback(at->identifier, &midi);
    }
    void newObjectAdded (bitklavier::MidiDeviceWrapper* obj) override
    {
        manager.addMidiInputDeviceCallback(obj->identifier, &midi);
    }
    void objectRemoved (bitklavier::MidiDeviceWrapper*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }
    // void valueTreeParentChanged (juce::ValueTree&) override;
    //void valueTreeRedirected (juce::ValueTree&) override ;
    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override {
        tracktion::engine::ValueTreeObjectList<bitklavier::MidiDeviceWrapper>::valueTreePropertyChanged(v, i);
    }
    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::midiInput);
    }
    void updateDevices()
    {
        items = MidiInput::getAvailableDevices();
    }

    int getNumRows() override
    {
        return items.size();
    }
    static void drawTextLayout (Graphics& g, Component& owner, StringRef text, const Rectangle<int>& textBounds, bool enabled)
    {
        const auto textColour = owner.findColour (ListBox::textColourId, true).withMultipliedAlpha (enabled ? 1.0f : 0.6f);

        AttributedString attributedString { text };
        attributedString.setColour (textColour);
        attributedString.setFont ((float) textBounds.getHeight() * 0.6f);
        attributedString.setJustification (Justification::centredLeft);
        attributedString.setWordWrap (AttributedString::WordWrap::none);

        TextLayout textLayout;
        textLayout.createLayout (attributedString,
                                 (float) textBounds.getWidth(),
                                 (float) textBounds.getHeight());
        textLayout.draw (g, textBounds.toFloat());
    }
    void paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (isPositiveAndBelow (row, items.size()))
        {
            if (rowIsSelected)
                g.fillAll (findColour (TextEditor::highlightColourId)
                                   .withMultipliedAlpha (0.3f));

            auto item = items[row];
            bool _enabled = isMidiInputDeviceEnabled(item.identifier);




            auto x = getTickX();
            auto tickW = (float) height * 0.75f;

            getLookAndFeel().drawTickBox (g, *this, (float) x - tickW, ((float) height - tickW) * 0.5f, tickW, tickW,
                                          _enabled, true, true, false);

            drawTextLayout (g, *this, item.name, { x + 5, 0, width - x - 5, height }, _enabled);
        }
    }

    void listBoxItemClicked (int row, const MouseEvent& e) override
    {
        selectRow (row);

        if (e.x < getTickX())
            flipEnablement (row);
    }

    void listBoxItemDoubleClicked (int row, const MouseEvent&) override
    {
        flipEnablement (row);
    }

    void returnKeyPressed (int row) override
    {
        flipEnablement (row);
    }

    void paint (Graphics& g) override
    {
        ListBox::paint (g);

        if (items.isEmpty())
        {
            g.setColour (Colours::grey);
            g.setFont (0.5f * (float) getRowHeight());
            g.drawText (noItemsMessage,
                        0, 0, getWidth(), getHeight() / 2,
                        Justification::centred, true);
        }
    }

    int getBestHeight (int preferredHeight)
    {
        auto extra = getOutlineThickness() * 2;

        return jmax (getRowHeight() * 2 + extra,
                     jmin (getRowHeight() * getNumRows() + extra,
                           preferredHeight));
    }

private:
    //==============================================================================
    void changeListenerCallback (ChangeBroadcaster*) override
    {
        resized();
    }
    const String noItemsMessage;
    Array<MidiDeviceInfo> items;
    //std::vector<bitklavier::MidiDeviceWrapper> &enabledMidiInputs;
    MidiInputCallback& midi;
    AudioDeviceManager& manager;
    bool isMidiInputDeviceEnabled (const String& identifier)
    {
        for (auto mi : objects) {

            if (mi->identifier == identifier) {
                return true;
            }
        }
        return false;
    }
    void flipEnablement (const int row)
    {
        if (isPositiveAndBelow (row, items.size()))
        {
            auto identifier = items[row].identifier;
            if (!isMidiInputDeviceEnabled(identifier))
            {

               ValueTree t(IDs::midiInput);
               parent.appendChild(t, nullptr);
            }
            else
            {

                parent.removeChild(parent.getChildWithProperty(IDs::midiDeviceId, identifier), nullptr);

            }

            //deviceManager.setMidiInputDeviceEnabled (identifier, ! deviceManager.isMidiInputDeviceEnabled (identifier));
        }
        resized(); // should probably do this with a changelistener/changebroadcaster
    }

    int getTickX() const
    {
        return getRowHeight();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiInputSelectorComponentListBox)
};


class OpenGlMidiSelector : public OpenGlAutoImageComponent<MidiInputSelectorComponentListBox> {
public:
    OpenGlMidiSelector(MidiInputCallback& midi, const ValueTree & v,
            AudioDeviceManager& manager) :
            OpenGlAutoImageComponent<MidiInputSelectorComponentListBox>(midi,v,
                                                                         manager) {
        image_component_ = std::make_shared<OpenGlImageComponent>();
        setLookAndFeel(DefaultLookAndFeel::instance());
        image_component_->setComponent(this);
    }

    virtual void resized() override {
        OpenGlAutoImageComponent<MidiInputSelectorComponentListBox>::resized();
        if (isShowing())
            redoImage();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlMidiSelector)
};
class KeymapPreparation : public PreparationSection {
public:

// Constructor method that takes three arguments: a smart pointer to a PolygonalOscProcessor,
// a value tree, and a reference to an OpenGlWrapper object
    KeymapPreparation(std::unique_ptr<KeymapProcessor> proc, juce::ValueTree v, OpenGlWrapper &um);

// Destructor method
    ~KeymapPreparation();

// Static function that returns a pointer to a KeymapPreparation object
    static PreparationSection *createKeymapSection(ValueTree v, OpenGlWrapper &um) {

        return new KeymapPreparation(std::make_unique<KeymapProcessor>(), v, um);
    }

// Public function definitions for the KeymapPreparation class, which override functions
// in the PreparationSection base class
    std::shared_ptr<SynthSection> getPrepPopup() override;



    juce::AudioProcessor *getProcessor() override;

    std::unique_ptr<juce::AudioProcessor> getProcessorPtr() override;

private:

// Private member variable for the KeymapPreparation class: proc is a pointer to a
// KeymapProcessor Object
    KeymapProcessor &proc;
    std::unique_ptr<KeymapProcessor> _proc_ptr;

/************************************************************************************/
/*             NESTED CLASS: KeymapPopup, inherits from PreparationPopup            */
/************************************************************************************/

    class KeymapPopup : public PreparationPopup {
    public:
        void setColorRecursively(Component *component, int color_id, const Colour& color) {
            component->setColour(color_id, color);
            for (Component *child : component->getChildren())
                setColorRecursively(child, color_id, color);
        }
        // Constructor method that takes two arguments: a smart pointer to a KeymapProcessor,
        // and a reference to an OpenGlWrapper
        KeymapPopup(KeymapProcessor &proc, OpenGlWrapper &open_gl);

        // Public function definitions for the class, which override the base class methods for
        // initializing, rendering, resizing, and painting OpenGl components
        void initOpenGlComponents(OpenGlWrapper &open_gl) override;



        void resized() override;

        void paintBackground(Graphics &g) override {
            SynthSection::paintContainer(g);
            paintHeadingText(g);
            paintBorder(g);
            paintKnobShadows(g);
            paintChildrenBackgrounds(g);
        }

        int getViewPosition() {
            int view_height = getHeight();
            return view_height; //std::max(0, std::min<int>(selections_.size() * getRowHeight() - view_height, view_position_));
        }

        ~KeymapPopup();


    private:
        OpenGlWrapper &opengl;
        // Private function definitions and member variables for the KeymapPopup class
        void redoImage();

        KeymapParams *params = nullptr;
        KeymapProcessor &proc;

        std::unique_ptr<OpenGlMidiSelector> midi_selector_;
        // Change based on other names

        //OpenGlWrapper& open_gl;

        OpenGlImage sliderShadows;

    };

};
#endif //BITKLAVIER2_KEYMAPPREPARATION_H
