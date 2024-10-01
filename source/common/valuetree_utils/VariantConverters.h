//
// Created by Davis Polito on 9/23/24.
//

#ifndef BITKLAVIER2_VARIANTCONVERTERS_H
#define BITKLAVIER2_VARIANTCONVERTERS_H
#include <juce_audio_processors/juce_audio_processors.h>
namespace juce {
    template<>
    struct VariantConverter<juce::AudioProcessorGraph::NodeID> final
    {
        using Type = String;

        static juce::AudioProcessorGraph::NodeID fromVar(const var& v)
        {
            if(v.isString())
            {
                return juce::AudioProcessorGraph::NodeID(static_cast<uint32_t>(int64(v)));
            }
            return {};
        }

        static var toVar(const AudioProcessorGraph::NodeID& id)
        {
            return String(id.uid);
        }
    };
}

#endif //BITKLAVIER2_VARIANTCONVERTERS_H
