//
// Created by Davis Polito on 6/19/24.
//

#include "KeymapProcessor.h"
#include "common.h"
KeymapProcessor::KeymapProcessor (const juce::ValueTree& v, juce::AudioDeviceManager* manager) : PluginBase (v), _midi (std::make_unique<MidiManager> (&keyboard_state, manager, v))
{
    /**
     * user settings
     */
    //invertNoteOnNoteOff = true;
}
static juce::String getMidiMessageDescription (const juce::MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on "          + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off "         + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change "   + juce::String (m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel "      + juce::String (m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch "      + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + juce::String (m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + juce::String (m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";

    if (m.isController())
    {
        juce::String name (juce::MidiMessage::getControllerName (m.getControllerNumber()));

        if (name.isEmpty())
            name = "[" + juce::String (m.getControllerNumber()) + "]";

        return "Controller " + name + ": " + juce::String (m.getControllerValue());
    }

    return juce::String::toHexString (m.getRawData(), m.getRawDataSize());
}
static juce::String printMidi(juce::MidiMessage& message, const juce::String& source)
{

    auto time = message.getTimeStamp(); //- startTime;

    auto hours   = ((int) (time / 3600.0)) % 24;
    auto minutes = ((int) (time / 60.0)) % 60;
    auto seconds = ((int) time) % 60;
    auto millis  = ((int) (time * 1000.0)) % 1000;

    auto timecode = juce::String::formatted ("%02d:%02d:%02d.%03d",
                                             hours,
                                             minutes,
                                             seconds,
                                             millis);

    auto description = getMidiMessageDescription (message);

    return juce::String(timecode + "  -  " + description + " (" + source + ")"); // [7]
}
void KeymapProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    _midi->midi_collector_.reset (sampleRate);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
}

void KeymapProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    midiMessages.clear();
    int num_samples = buffer.getNumSamples();
    //MidiBuffer midi_messages;
    _midi->removeNextBlockOfMessages (midiMessages, num_samples);
    _midi->replaceKeyboardMessages (midiMessages, num_samples);

    /**
     *
     * MIDI Processing Happens here
     *
     */

    // this could surely be optimized, though i'm not sure how important that is ./dlt
    // i'm also not 100% this properly retain the samplePosition, but again, it's not clear how important
    // that is when noteOn and noteOff are inverted... might be important for other MIDI processing, however
    if (invertNoteOnNoteOff)
    {
        juce::MidiBuffer saveMidi (midiMessages);
        midiMessages.clear();
        for (auto mi : saveMidi)
        {
            auto message = mi.getMessage();
            if (message.isNoteOn() || message.isNoteOff())
                midiMessages.addEvent (swapNoteOnNoteOff (message), mi.samplePosition);
            else
                midiMessages.addEvent (message, mi.samplePosition);
        }
    }

    /**
     * others to do from the original KeyMap
     *
     * Trigger All Notes Off
     * Ignore Sustain Pedal
     * Use as Sustain Pedal
     * Toggle Keys
     * Sostenuto Mode
     * Ignore NoteOff
     *
     */

    // print them out for now
    for (auto mi : midiMessages)
    {
        auto message = mi.getMessage();

        mi.samplePosition;
        mi.data;
        DBG (printMidi (message, "kmap"));
    }
}

// turns noteOn messages into noteOff messages, and vice versa
// there is surely a more efficient way to do this...
juce::MidiMessage KeymapProcessor::swapNoteOnNoteOff (juce::MidiMessage inmsg)
{
    if (inmsg.isNoteOff())
    {
        auto newmsg = juce::MidiMessage::noteOn (inmsg.getChannel(), inmsg.getNoteNumber(), inmsg.getVelocity());
        newmsg.addToTimeStamp (inmsg.getTimeStamp());
        return newmsg;
    }
    else if (inmsg.isNoteOn())
    {
        auto newmsg = juce::MidiMessage::noteOff (inmsg.getChannel(), inmsg.getNoteNumber(), inmsg.getVelocity());
        newmsg.addToTimeStamp (inmsg.getTimeStamp());
        return newmsg;
    }

    return inmsg;
}
//template<>
//std::string bitklavier::PluginBase<bitklavier::PreparationStateImpl<KeymapParams, KeymapNonParameterState, chowdsp::XMLSerializer>>::name = "keymap";