//
// Created by Davis Polito on 5/2/24.
//

#include "DirectProcessor.h"
#include "Synthesiser/Sample.h"
#include "common.h"
#include <chowdsp_serialization/chowdsp_serialization.h>
DirectProcessor::DirectProcessor (const juce::ValueTree& v) : PluginBase (v, nullptr, directBusLayout()),
mainSynth(new BKSynthesiser(state.params.env, state.params.gainParam)),
hammerSynth(new BKSynthesiser(state.params.env,state.params.hammerParam)),
releaseResonanceSynth(new BKSynthesiser(state.params.env,state.params.releaseResonanceParam)),
pedalSynth(new BKSynthesiser(state.params.env,state.params.pedalParam))
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

void DirectProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
#if JUCE_MODULE_AVAILABLE_chowdsp_plugin_state
    state.getParameterListeners().callAudioThreadBroadcasters();
#endif

    buffer.clear(); // always top of the chain as an instrument source; doesn't take audio in

    if (mainSynth->hasSamples() )
        mainSynth->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    if (hammerSynth->hasSamples())
        hammerSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    if (releaseResonanceSynth->hasSamples())
        releaseResonanceSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    if (pedalSynth->hasSamples())
        pedalSynth->renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

//DBG ("attack: " + juce::String (state.params.env.attackParam->get()));
    //juce::dsp::AudioBlock<float> block(buffer);
    //melatonin::printSparkline(buffer);
    bufferDebugger->capture("direct", buffer.getReadPointer(0), buffer.getNumSamples(), -1.f, 1.f);
}