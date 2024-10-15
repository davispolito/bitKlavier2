//
// Created by Davis Polito on 11/8/23.
//

#ifndef BITKLAVIER2_COMMON_H
#define BITKLAVIER2_COMMON_H
/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <juce_core/juce_core.h>

// Debugging.
#if DEBUG
    #include <cassert>
    #define _ASSERT(x) assert(x)
#else
    #define _ASSERT(x) ((void)0)
#endif // DEBUG

#define UNUSED(x) ((void)x)

#if !defined(force_inline)
    #if defined (_MSC_VER)
        #define force_inline __forceinline
        #define vector_call __vectorcall
    #else
        #define force_inline inline __attribute__((always_inline))
        #define vector_call
    #endif
#endif

#include "melatonin_audio_sparklines/melatonin_audio_sparklines.h"
namespace bitklavier {

    typedef float mono_float;

    constexpr mono_float kPi = 3.1415926535897932384626433832795f;
    constexpr mono_float kSqrt2 = 1.414213562373095048801688724209698f;
    constexpr mono_float kEpsilon = 1e-16f;
    constexpr int kMaxBufferSize = 128;
    constexpr int kMaxOversample = 8;
    constexpr int kDefaultSampleRate = 44100;
    constexpr mono_float kMinNyquistMult = 0.45351473923f;
    constexpr int kMaxSampleRate = 192000;
    constexpr int kMidiSize = 128;
    constexpr int kMidiTrackCenter = 60;

    constexpr mono_float kMidi0Frequency = 8.1757989156f;
    constexpr mono_float kDbfsIncrease = 6.0f;
    constexpr int kDegreesPerCycle = 360;
    constexpr int kMsPerSec = 1000;
    constexpr int kNotesPerOctave = 12;
    constexpr int kCentsPerNote = 100;
    constexpr int kCentsPerOctave = kNotesPerOctave * kCentsPerNote;

    constexpr int kPpq = 960; // Pulses per quarter note.
    constexpr mono_float kVoiceKillTime = 0.05f;
    constexpr int kNumChannels = 2;
    constexpr int kNumMidiChannels = 16;
    constexpr int kFirstMidiChannel = 0;
    constexpr int kLastMidiChannel = kNumMidiChannels - 1;

    enum VoiceEvent {
        kInvalid,
        kVoiceIdle,
        kVoiceOn,
        kVoiceHold,
        kVoiceDecay,
        kVoiceOff,
        kVoiceKill,
        kNumVoiceEvents
    };

    typedef enum BKPreparationType {
        PreparationTypeDirect = 0,
        PreparationTypeSynchronic,
        PreparationTypeNostalgic,
        PreparationTypeBlendronic,
        PreparationTypeResonance,
        PreparationTypeTuning,
        PreparationTypeTempo,
        PreparationTypeKeymap,
        PreparationTypeDirectMod,
        PreparationTypeSynchronicMod,
        PreparationTypeNostalgicMod,
        PreparationTypeBlendronicMod,
        PreparationTypeResonanceMod,
        PreparationTypeTuningMod,
        PreparationTypeTempoMod,
        PreparationTypeGenericMod,
        PreparationTypePianoMap,
        PreparationTypeReset,
        PreparationTypePiano,
        PreparationTypeComment,
        PreparationTypeCompressor,
        BKPreparationTypeNil,
    } BKPreparationType;

//    //==============================================================================
//    /**
//    Utility wrapper for ValueTree::Listener's that only want to override valueTreePropertyChanged.
//*/
//    struct ValueTreePropertyChangeListener  : public juce::ValueTree::Listener
//    {
//        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override {}
//        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override {}
//        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}
//        void valueTreeParentChanged (juce::ValueTree&) override {}
//        void valueTreeRedirected (juce::ValueTree&) override {}
//    };

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
} // namespace vital



#endif //BITKLAVIER2_COMMON_H
