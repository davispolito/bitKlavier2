//
// Created by Davis Polito on 9/23/24.
//

#ifndef BITKLAVIER2_VARIANTCONVERTERS_H
#define BITKLAVIER2_VARIANTCONVERTERS_H
#include <juce_audio_processors/juce_audio_processors.h>
namespace juce {
    template<>
    struct juce::VariantConverter<juce::AudioProcessorGraph::NodeID> final
    {
        using Type = juce::String;

        static juce::AudioProcessorGraph::NodeID fromVar(const juce::var& v)
        {
            if(v.isString())
            {
                return juce::AudioProcessorGraph::NodeID(static_cast<uint32_t>(int64(v)));
            }
            return {};
        }

        static juce::var toVar(const juce::AudioProcessorGraph::NodeID& id)
        {
            return juce::String(id.uid);
        }
    };
}

#endif //BITKLAVIER2_VARIANTCONVERTERS_H
