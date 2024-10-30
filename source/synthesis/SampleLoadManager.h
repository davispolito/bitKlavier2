//
// Created by Davis Polito on 8/12/24.
//

#ifndef BITKLAVIER2_AUDIOFILEMANAGER_H
#define BITKLAVIER2_AUDIOFILEMANAGER_H

#include "UserPreferences.h"
#include "utils.h"
#include <juce_events/juce_events.h>
template <typename T>
class BKSamplerSound;

class AudioFormatReaderFactory
{
public:
    AudioFormatReaderFactory() = default;
    AudioFormatReaderFactory (const AudioFormatReaderFactory&) = default;
    AudioFormatReaderFactory (AudioFormatReaderFactory&&) = default;
    AudioFormatReaderFactory& operator= (const AudioFormatReaderFactory&) = default;
    AudioFormatReaderFactory& operator= (AudioFormatReaderFactory&&) = default;

    virtual ~AudioFormatReaderFactory() noexcept = default;

    virtual std::pair<std::unique_ptr<juce::AudioFormatReader>, juce::String> make (juce::AudioFormatManager&) const = 0;
    virtual std::unique_ptr<AudioFormatReaderFactory> clone() const = 0;
};

inline std::unique_ptr<juce::AudioFormatReader> makeAudioFormatReader (juce::AudioFormatManager& manager,
    const void* sampleData,
    size_t dataSize)
{
    return std::unique_ptr<juce::AudioFormatReader> (manager.createReaderFor (std::make_unique<juce::MemoryInputStream> (sampleData,
        dataSize,
        false)));
}

inline std::unique_ptr<juce::AudioFormatReader> makeAudioFormatReader (juce::AudioFormatManager& manager,
    const juce::File& file)
{
    return std::unique_ptr<juce::AudioFormatReader> (manager.createReaderFor (file));
}

class FileArrayAudioFormatReaderFactory : public AudioFormatReaderFactory
{
public:
    explicit FileArrayAudioFormatReaderFactory (juce::Array<juce::File> filesIn)
        : files (std::move (filesIn)), currentFile (0)
    {
    }

    std::pair<std::unique_ptr<juce::AudioFormatReader>, juce::String> make (juce::AudioFormatManager& manager) const override
    {
        // Check if we have reached the end of the array
        if (currentFile >= files.size())
            return { nullptr, "done" };

        return { makeAudioFormatReader (manager, files.getUnchecked (currentFile)), files.getUnchecked (currentFile++).getFileNameWithoutExtension() };
    }

    std::unique_ptr<AudioFormatReaderFactory> clone() const override
    {
        return std::unique_ptr<AudioFormatReaderFactory> (new FileArrayAudioFormatReaderFactory (*this));
    }

private:
    juce::Array<juce::File> files;
    //mutable int so that const make can modify vairable
    mutable int currentFile;
};
////==============================================================================
//class FileAudioFormatReaderFactory : public AudioFormatReaderFactory
//{
//public:
//    explicit FileAudioFormatReaderFactory(juce::File fileIn)
//            : file(std::move(fileIn))
//    {}
//
//    std::unique_ptr<juce::AudioFormatReader> make(juce::AudioFormatManager& manager) const override
//    {
//        return makeAudioFormatReader(manager, file);
//    }
//
//    std::unique_ptr<AudioFormatReaderFactory> clone() const override
//    {
//        return std::unique_ptr<AudioFormatReaderFactory>(new FileAudioFormatReaderFactory(*this));
//    }
//
//private:
//    juce::File file;
//};

class SynthBase;
class SampleLoadManager : public juce::AsyncUpdater
{
public:
    SampleLoadManager (UserPreferences& preferences, SynthBase* synth_);
    ~SampleLoadManager();

    juce::ThreadPool sampleLoader;
    std::map<juce::String, juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>> samplerSoundset;

    bool loadSamples (int selection, bool isGlobal);
    void loadSamples_sub (bitklavier::utils::BKPianoSampleType thisSampleType);
    juce::Array<juce::File> samplesByPitch (juce::String whichPitch, juce::Array<juce::File> inFiles);

    // maybe this repetition can be cleaned up, but maybe not so important.
    juce::String globalSoundset_name;
    juce::String globalHammersSoundset_name;
    juce::String globalReleaseResonanceSoundset_name;
    juce::String globalPedalsSoundset_name;
    juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* globalSoundset = nullptr;
    juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* globalHammersSoundset = nullptr;
    juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* globalReleaseResonanceSoundset = nullptr;
    juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* globalPedalsSoundset = nullptr;

    UserPreferences& preferences;
    void handleAsyncUpdate() override;

    SynthBase* synth;
    std::unique_ptr<juce::AudioFormatManager> audioFormatManager;
    std::unique_ptr<AudioFormatReaderFactory> readerFactory;

    // perhaps these should be moved to utils or something
    juce::Array<juce::String> allPitches;
    juce::Array<juce::String> allPitchClasses = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
    juce::Array<int> allKeysWithSamples; // array that keeps track of which keys have samples, for building start/end ranges in keymap
    juce::BigInteger getMidiRange (juce::String pitchName);
};

/* SampleLoadJob is setup to load the standard bK sample setup, with main, hammer (rel), release resonance Harm, and pedal samples
 * all named consistently like the original Yamaha/Salamander samples used in bitKlavier
 *
 * loadMainSamplesByPitch() is for loading all the velocity layers for a particular string/key
 * layer numbers to NOT have be consecutive
 * but they do need to be ascending, with quieter samples having lower numbers than louder samples
 * so, v1, v4, v5, v23 from softest to loudest is fine... but v4, v1, v23, v5 from softest to loudest is not
 */
class SampleLoadJob : public juce::ThreadPoolJob
{
public:
    SampleLoadJob ( //int loadType,
        int sampleType,
        int numLayers,
        juce::BigInteger newMidiRange,
        std::unique_ptr<AudioFormatReaderFactory> ptr,
        juce::AudioFormatManager* manager,
        juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* soundset,
        juce::AsyncUpdater* loadManager) : juce::ThreadPoolJob ("sample_loader"),
                                           soundset (soundset),
                                           loadManager (loadManager),
                                           sampleReader (std::move (ptr)),
                                           //loadType(loadType),
                                           manager (manager)
    {
        thisSampleType = sampleType;
        velocityLayers = numLayers;
        thisMidiRange = newMidiRange;
    };

    JobStatus runJob() override;

    void loadSamples(); // calls one of the following, depending on context
    void loadMainSamplesByPitch();
    void loadHammerSamples();
    void loadReleaseResonanceSamples();
    void loadPedalSamples();

    int thisSampleType;
    int velocityLayers; // how many velocity layers for this particular string
    juce::BigInteger thisMidiRange;
    juce::Array<std::tuple<int, int>> getVelLayers (int howmany);

    juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* soundset;
    std::unique_ptr<AudioFormatReaderFactory> sampleReader;
    juce::AudioFormatManager* manager;
    juce::AsyncUpdater* loadManager;
};

#endif //BITKLAVIER2_AUDIOFILEMANAGER_H
