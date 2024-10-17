//
// Created by Davis Polito on 6/19/24.
//

#include "KeymapProcessor.h"
#include "common.h"
KeymapProcessor::KeymapProcessor(const ValueTree& v, AudioDeviceManager* manager) : PluginBase(v, nullptr, keymapBusLayout()), _midi(std::make_unique<MidiManager>(&keyboard_state,manager,v))
{

    /**
     * user settings
     */
    //invertNoteOnNoteOff = true;
}

void KeymapProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    _midi->midi_collector_.reset(sampleRate);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);
}


void KeymapProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    midiMessages.clear();
    int num_samples = buffer.getNumSamples();
    //MidiBuffer midi_messages;
    _midi->removeNextBlockOfMessages(midiMessages, num_samples);
    _midi->replaceKeyboardMessages(midiMessages, num_samples);

    // if invert noteOn/noteOff is set
    // this could surely be optimized, though i'm not sure how important that is ./dlt
    if (invertNoteOnNoteOff) {
        MidiBuffer saveMidi(midiMessages);
        midiMessages.clear();
        for(auto mi : saveMidi)
        {
            auto message = mi.getMessage();
            if(message.isNoteOn() || message.isNoteOff())
                midiMessages.addEvent(swapNoteOnNoteOff(message), mi.samplePosition);
            else
                midiMessages.addEvent(message, mi.samplePosition);
        }
    }

    // print them out for now
    for(auto mi : midiMessages)
    {
        auto message = mi.getMessage();

        mi.samplePosition;
        mi.data;
        DBG(bitklavier::printMidi(message, "kmap"));
    }

}

// turns noteOn messages into noteOff messages, and vice versa
// there is surely a more efficient way to do this...
MidiMessage KeymapProcessor::swapNoteOnNoteOff (MidiMessage inmsg)
{
    if (inmsg.isNoteOff())
    {
        auto newmsg = MidiMessage::noteOn(inmsg.getChannel(), inmsg.getNoteNumber(), inmsg.getVelocity());
        newmsg.addToTimeStamp(inmsg.getTimeStamp());
        return newmsg;
    }
    else if (inmsg.isNoteOn())
    {
        auto newmsg = MidiMessage::noteOff(inmsg.getChannel(), inmsg.getNoteNumber(), inmsg.getVelocity());
        newmsg.addToTimeStamp(inmsg.getTimeStamp());
        return newmsg;
    }

    return inmsg;
}