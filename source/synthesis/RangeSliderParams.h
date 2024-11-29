//
// Created by Dan Trueman on 11/28/24.
//

#ifndef BITKLAVIER2_RANGESLIDERPARAMS_H
#define BITKLAVIER2_RANGESLIDERPARAMS_H

#include <PreparationStateImpl.h>
#include <chowdsp_plugin_utils/chowdsp_plugin_utils.h>

struct RangeSliderParams : chowdsp::ParamHolder
{
    RangeSliderParams() : chowdsp::ParamHolder("SliderRange")
    {
        add(velocityParamMin, velocityParamMax);
    }

    // Velocity Min param
    chowdsp::FloatParameter::Ptr velocityParamMin {
        juce::ParameterID { "VelocityMin", 100 },
        "Velocity Min",
        chowdsp::ParamUtils::createNormalisableRange (0.0f, 128.0f, 63.f),
        0.0f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

    // Velocity Max param
    chowdsp::FloatParameter::Ptr velocityParamMax {
        juce::ParameterID { "VelocityMax", 100 },
        "Velocity Max",
        chowdsp::ParamUtils::createNormalisableRange (0.0f, 128.0f, 63.f),
        128.0f,
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

};

#endif //BITKLAVIER2_RANGESLIDERPARAMS_H
