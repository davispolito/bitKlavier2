//
// Created by Davis Polito on 11/13/24.
//

#ifndef BITKLAVIER2_TRANSPOSEPARAMS_H
#define BITKLAVIER2_TRANSPOSEPARAMS_H
#include <PreparationStateImpl.h>
#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>
struct TransposeParams : chowdsp::ParamHolder
{
    TransposeParams() : chowdsp::ParamHolder("TRANSPOSE")
    {
//        add(initialparam);
    }
//    std::vector<chowdsp::FloatParameter::Ptr> transposeVec = {chowdsp::FloatParameter::Ptr initialparam(
//    chowdsp::SemitonesParameter::Ptr initialparam {
//            juce::ParameterID{"t0", 100},
//            "t0",
//            chowdsp::ParamUtils::createNormalisableRange(-12.0f,12.0f,0.0f), // FIX
//           0.0f
//    };
    chowdsp::SemitonesParameter::Ptr addNewSliderParam() {
        chowdsp::SemitonesParameter::Ptr ptr{juce::ParameterID{"t" + juce::String(numSlidersAdded), 100},
                                             "t" + juce::String(numSlidersAdded++),
                                             chowdsp::ParamUtils::createNormalisableRange(-12.0f, 12.0f, 0.0f), // FIX
                                             0.0f};


        add(ptr);
        return ptr;
    }
    int numSlidersAdded =0;
};
#endif //BITKLAVIER2_TRANSPOSEPARAMS_H
