//
// Created by Dan Trueman on 11/5/24.
//

#ifndef BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#define BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#include "ParametersView.h"
#include "envelope_section.h"
class DirectParametersView : public bitklavier::ParametersView
{
public:
    DirectParametersView(chowdsp::PluginState& pluginState, chowdsp::ParamHolder& params, OpenGlWrapper *open_gl) :
                                                                                                                     bitklavier::ParametersView(pluginState,params,open_gl)
    {
        //envelope = std::make_unique<EnvelopeSection>("ENV", "err");
        //addSubSection(envelope.get());
    }
    //std::unique_ptr<EnvelopeSection> envelope;
    void resized() override;
};

#endif //BITKLAVIER2_DIRECTPARAMETERSVIEW_H
