//
// Created by Davis Polito on 11/8/23.
//

#ifndef BITKLAVIER2_BITKLAVIERGUIINTERFACE_H
#define BITKLAVIER2_BITKLAVIERGUIINTERFACE_H
#include <juce_audio_devices/juce_audio_devices.h>
class FullInterface;

class BitKlavierGuiInterface
{
public:
    BitKlavierGuiInterface(bool use_gui = true);

    virtual ~BitKlavierGuiInterface();

    virtual juce::AudioDeviceManager* getAudioDeviceManager() {return nullptr;}
    virtual void updateFullGui();

protected:
    //SynthBase* synth_;

    std::unique_ptr<FullInterface> gui_;
};

#endif //BITKLAVIER2_BITKLAVIERGUIINTERFACE_H
