//
// Created by Dan Trueman on 11/5/24.
//

#ifndef BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#define BITKLAVIER2_DIRECTPARAMETERSVIEW_H
#include "ParametersView.h"
#include "envelope_section.h"
#include "TransposeParams.h"
#include "OpenGlTranspositionSlider.h"
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
//                    DBG("----paramvecdreictparamview----");
//                    for (auto &param: paramVec)
//                    {
////                        comps.push_back(parameters_view_detail::createParameterComp(paramListeners, param,*this));
////                        DBG("add" + param->paramID);
//                    }
                },
                [this, &listeners, &pluginState](auto &paramHolder) {
                    DBG("xdirectparamview");
                    if(auto *transposeParam = dynamic_cast<TransposeParams*>(&paramHolder)) {
                        transpositionSlider = std::make_unique<OpenGlTranspositionSlider>(transposeParam, listeners);
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
        //transpositionSlider->setValue(1., juce::dontSendNotification);
        addOpenGlComponent(transpositionSlider->getImageComponent());

    }
//    std::unique_ptr<EnvelopeSection> envelope;
    std::unique_ptr<OpenGlTranspositionSlider> transpositionSlider;
    void resized() override;
    chowdsp::ScopedCallbackList transposeCallbacks;

};

#endif //BITKLAVIER2_DIRECTPARAMETERSVIEW_H
