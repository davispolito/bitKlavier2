//
// Created by Dan Trueman on 11/5/24.
//

#ifndef BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#define BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#include "OpenGlStackedSlider.h"
#include "ParametersView.h"
#include "TransposeParams.h"
#include "envelope_section.h"

class DirectParametersView : public bitklavier::ParametersView
{
public:
    DirectParametersView(chowdsp::PluginState& pluginState, chowdsp::ParamHolder& params, OpenGlWrapper *open_gl) :
                                                                                                                     bitklavier::ParametersView(pluginState,params,open_gl)
    {
        //envelope = std::make_unique<EnvelopeSection>("ENV", "err");
        //addSubSection(envelope.get());
        auto& listeners = pluginState.getParameterListeners();
        params.doForAllParameterContainers(
                [this,&listeners](auto &paramVec) {

                },
                [this, &listeners, &pluginState](auto &paramHolder) {
                    DBG("xdirectparamview");
                    if(auto *transposeParam = dynamic_cast<TransposeParams*>(&paramHolder)) {
                        transpositionSlider = std::make_unique<OpenGlStackedSlider>(transposeParam, listeners);
                        transposeParam->doForAllParameters([this, &pluginState] (auto& param, size_t indexInParamHolder)
                        {
                            transposeCallbacks += { pluginState.addParameterListener(param,
                                   chowdsp::ParameterListenerThread::MessageThread,
                                   [this] {


                                       DBG ("transposeslider" + this->getName());

                                   })};
                        });
                    }
                    DBG("paramholder name" + paramHolder.getName());
                    //addSubSection(parameters_view_detail::createEditorSection(paramHolder,paramListeners,*this).release());
                });


        addAndMakeVisible(*transpositionSlider);
        addOpenGlComponent(transpositionSlider->getImageComponent());

        // extract special components from vector of general components
        auto it = std::find_if(
            comps.begin(),
            comps.end(),
            [](const std::unique_ptr<juce::Component>& p) { return p->getName() == "UseTuning"; }
            );

        _ASSERT(it != comps.end());
        transpose_uses_tuning = std::move(*it);
        comps.erase(it);
    }

    std::unique_ptr<juce::Component> transpose_uses_tuning;
    std::unique_ptr<OpenGlStackedSlider> transpositionSlider;
    chowdsp::ScopedCallbackList transposeCallbacks;


    void resized() override;

};

#endif //BITKLAVIER2_DIRECTPARAMETERSVIEW_H
