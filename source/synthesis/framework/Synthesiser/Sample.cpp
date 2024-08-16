//
// Created by Davis Polito on 8/7/24.
//
#include "Sample.h"



//==============================================================================
BKSynthesiserVoice::BKSynthesiserVoice() {}
BKSynthesiserVoice::~BKSynthesiserVoice() {}

bool BKSynthesiserVoice::isPlayingChannel (const int midiChannel) const
{
    return currentPlayingMidiChannel == midiChannel;
}

void BKSynthesiserVoice::setCurrentPlaybackSampleRate (const double newRate)
{
    currentSampleRate = newRate;
}

bool BKSynthesiserVoice::isVoiceActive() const
{
    return getCurrentlyPlayingNote() >= 0;
}

void BKSynthesiserVoice::clearCurrentNote()
{
    currentlyPlayingNote = -1;
    currentlyPlayingSound = nullptr;
    currentPlayingMidiChannel = 0;
}

void BKSynthesiserVoice::aftertouchChanged (int) {}
void BKSynthesiserVoice::channelPressureChanged (int) {}

bool BKSynthesiserVoice::wasStartedBefore (const BKSynthesiserVoice& other) const noexcept
{
    return noteOnTime < other.noteOnTime;
}

//void BKSynthesiserVoice::renderNextBlock (juce::AudioBuffer<double>& outputBuffer,
//                                        int startSample, int numSamples)
//{
//    juce::AudioBuffer<double> subBuffer (outputBuffer.getArrayOfWritePointers(),
//                                   outputBuffer.getNumChannels(),
//                                   startSample, numSamples);
//
//    tempBuffer.makeCopyOf (subBuffer, true);
//    renderNextBlock (tempBuffer, 0, numSamples);
//    subBuffer.makeCopyOf (tempBuffer, true);
//}
