//
// Created by Dan Trueman on 11/5/24.
//

#ifndef BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#define BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#include "OpenGlStackedSlider.h"
#include "ParametersView.h"
#include "TransposeParams.h"
#include "envelope_section.h"
#include "StackedSliderSection.h"
class DirectParametersView : public bitklavier::ParametersView
{
public:
    DirectParametersView(chowdsp::PluginState& pluginState, chowdsp::ParamHolder& params, OpenGlWrapper *open_gl) :
                                                                                                                     bitklavier::ParametersView(pluginState,params,open_gl)
    {
//        envelope = std::make_unique<EnvelopeSection>("ENV", "err");
//        addSubSection(envelope.get());
        auto& listeners = pluginState.getParameterListeners();
        params.doForAllParameterContainers(
                [this,&listeners](auto &paramVec) {

                },
                [this, &listeners, &pluginState](auto &paramHolder) {
                    DBG("xdirectparamview");
                    if(auto *transposeParam = dynamic_cast<TransposeParams*>(&paramHolder)) {
                        transpose  = std::make_unique<StackSliderSection>(transposeParam, listeners);
                        addSubSection(transpose.get());
                        //addAndMakeVisible(transpositionSlider.get());
                    }
                    DBG("paramholder name" + paramHolder.getName());
                    //addSubSection(parameters_view_detail::createEditorSection(paramHolder,paramListeners,*this).release());
                });





    }

    std::unique_ptr<StackSliderSection> transpose;
    chowdsp::ScopedCallbackList transposeCallbacks;


    void resized() override;

};

#endif //BITKLAVIER2_DIRECTPARAMETERSVIEW_H
