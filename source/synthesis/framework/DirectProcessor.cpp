//
// Created by Davis Polito on 5/2/24.
//

#include "DirectProcessor.h"
#include "Synthesiser/Sample.h"
#include "common.h"
#include <chowdsp_serialization/chowdsp_serialization.h>

DirectProcessor::DirectProcessor (const juce::ValueTree& v) :
    PluginBase (v, nullptr, directBusLayout()),
    mainSynth(new BKSynthesiser(state.params.env, state.params.gainParam, state.params.velocityRangeParams)),
    hammerSynth(new BKSynthesiser(state.params.env, state.params.gainParam, state.params.velocityRangeParams)),
    releaseResonanceSynth(new BKSynthesiser(state.params.env, state.params.gainParam, state.params.velocityRangeParams)),
    pedalSynth(new BKSynthesiser(state.params.env, state.params.gainParam, state.params.velocityRangeParams))
{
    for (int i = 0; i < 300; i++)
    {
        mainSynth->addVoice (new BKSamplerVoice());
        hammerSynth->addVoice (new BKSamplerVoice());
        releaseResonanceSynth->addVoice (new BKSamplerVoice());
        pedalSynth->addVoice (new BKSamplerVoice());
    }

    // these synths play their stuff on noteOff rather than noteOn
    hammerSynth->isKeyReleaseSynth (true);
    releaseResonanceSynth->isKeyReleaseSynth (true);

    pedalSynth->isPedalSynth (true);

    bufferDebugger = new BufferDebugger();

}

void DirectProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto spec = juce::dsp::ProcessSpec { sampleRate, (uint32_t) samplesPerBlock, (uint32_t) getMainBusNumInputChannels() };
    mainSynth->setCurrentPlaybackSampleRate (sampleRate);
    gain.prepare (spec);
    gain.setRampDurationSeconds (0.05);

    hammerSynth->setCurrentPlaybackSampleRate (sampleRate);
    releaseResonanceSynth->setCurrentPlaybackSampleRate (sampleRate);
    pedalSynth->setCurrentPlaybackSampleRate (sampleRate);
}

//void DirectProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
//{
//    buffer.clear();
//
//
//
//    gain.setGainDecibels (*state.params.gainParam);
//    gain.process (buffer);
//}

bool DirectProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const
{
    //    // only supports mono and stereo (for now)
    //    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
    //        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    //        return false;
    //
    //    // input and output layout must be the same
    //    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    //        return false;

    return true;
}

/*
 * grabs all the TransposeParams values and compiles them into a single array
 * the first slider is always represented, so we always have at least on value to return
 *
 * these operate at the synthesizer level, not the voice level, so need to be passed here
 * and not just looked at by individual voices in the synth
 */
juce::Array<float> DirectProcessor::getMidiNoteTranspositions()
{
    juce::Array<float> transps;

    auto paramVals = state.params.transpose.getFloatParams();
    for (auto const& tp : *paramVals)
    {
        if (tp->getCurrentValue() != 0.)
            transps.addIfNotAlreadyThere(tp->getCurrentValue());
    }

    // make sure that the first slider is always represented
    transps.addIfNotAlreadyThere(state.params.transpose.t0->getCurrentValue());
    //DBG("DirectProcessor::getMidiNoteTranspositions() t0 = " + juce::String(state.params.transpose.t0->getCurrentValue()));

    return transps;
}


void DirectProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
#if JUCE_MODULE_AVAILABLE_chowdsp_plugin_state
    state.getParameterListeners().callAudioThreadBroadcasters();
#endif

    //DBG("velocity Max = " + juce::String(state.params.velocityRangeParams.velocityParamMax->getCurrentValue()));

    buffer.clear(); // always top of the chain as an instrument source; doesn't take audio in
    juce::Array<float> updatedTransps = getMidiNoteTranspositions(); // from the Direct transposition slider

    bool useTuningForTranspositions = state.params.transpositionUsesTuning->get();

    if (mainSynth->hasSamples() )
    {
        mainSynth->updateMidiNoteTranspositions(updatedTransps, useTuningForTranspositions);
        mainSynth->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }

    if (hammerSynth->hasSamples())
    {
        hammerSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    }

    if (releaseResonanceSynth->hasSamples())
    {
        releaseResonanceSynth->updateMidiNoteTranspositions(updatedTransps, useTuningForTranspositions);
        releaseResonanceSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    }

    if (pedalSynth->hasSamples())
    {
        pedalSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    }

    //DBG ("attack: " + juce::String (state.params.env.attackParam->get()));
    //juce::dsp::AudioBlock<float> block(buffer);
    //melatonin::printSparkline(buffer);
    bufferDebugger->capture("direct", buffer.getReadPointer(0), buffer.getNumSamples(), -1.f, 1.f);
}