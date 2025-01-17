//
// Created by Davis Polito on 9/23/24.
//

#ifndef BITKLAVIER2_AUDIOPROCESSORGRAPH_H
#define BITKLAVIER2_AUDIOPROCESSORGRAPH_H
#include <juce_audio_processors/juce_audio_processors.h>

namespace bitklavier {
    class AudioProcessorGraph : public juce::AudioProcessorGraph {
    public :
        void getStateInformation(juce::MemoryBlock &) override;

        void setStateInformation(const void *data, int sizeInBytes) override;
    };
}
#endif //BITKLAVIER2_AUDIOPROCESSORGRAPH_H
