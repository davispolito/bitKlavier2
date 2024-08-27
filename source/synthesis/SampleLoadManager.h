//
// Created by Davis Polito on 8/12/24.
//

#ifndef BITKLAVIER2_AUDIOFILEMANAGER_H
#define BITKLAVIER2_AUDIOFILEMANAGER_H
#include <juce_events/juce_events.h>
#include "UserPreferences.h"
#include "utils.h"
template<typename T>
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

    virtual std::pair<std::unique_ptr<AudioFormatReader>, juce::String> make(AudioFormatManager&) const = 0;
    virtual std::unique_ptr<AudioFormatReaderFactory> clone() const = 0;
};

inline std::unique_ptr<AudioFormatReader> makeAudioFormatReader(AudioFormatManager& manager,
                                                                const void* sampleData,
                                                                size_t dataSize)
{
    return std::unique_ptr<AudioFormatReader>(manager.createReaderFor(std::make_unique<MemoryInputStream>(sampleData,
                                                                                                          dataSize,
                                                                                                          false)));
}

inline std::unique_ptr<AudioFormatReader> makeAudioFormatReader(AudioFormatManager& manager,
                                                                const File& file)
{
    return std::unique_ptr<AudioFormatReader>(manager.createReaderFor(file));
}

class FileArrayAudioFormatReaderFactory : public AudioFormatReaderFactory
{
public:
    explicit FileArrayAudioFormatReaderFactory(Array<File> filesIn)
            : files(std::move(filesIn)), currentFile(0)
    {}

    std::pair<std::unique_ptr<AudioFormatReader>, juce::String> make(AudioFormatManager& manager) const override
    {
        // Check if we have reached the end of the array
        if (currentFile >= files.size())
            return {nullptr, "done"};


        return {makeAudioFormatReader(manager, files.getUnchecked(currentFile)), files.getUnchecked(currentFile++).getFileNameWithoutExtension()} ;
    }

    std::unique_ptr<AudioFormatReaderFactory> clone() const override
    {
        return std::unique_ptr<AudioFormatReaderFactory>(new FileArrayAudioFormatReaderFactory(*this));
    }

private:
    Array<File> files;
    //mutable int so that const make can modify vairable
    mutable int currentFile;
};
////==============================================================================
//class FileAudioFormatReaderFactory : public AudioFormatReaderFactory
//{
//public:
//    explicit FileAudioFormatReaderFactory(File fileIn)
//            : file(std::move(fileIn))
//    {}
//
//    std::unique_ptr<AudioFormatReader> make(AudioFormatManager& manager) const override
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
//    File file;
//};

class SynthBase;
class SampleLoadManager : public juce::AsyncUpdater{
public:
    SampleLoadManager(UserPreferences &preferences, SynthBase* synth_);
    ~SampleLoadManager();

    bool loadSamples(int selection, bool isGlobal);
    juce::ThreadPool sampleLoader;
    std::map<juce::String, juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>> sampler_soundset;
    juce::String globalSoundSet;
    juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>>* global_soundset = nullptr;
    UserPreferences &preferences;
    void handleAsyncUpdate() override;

    SynthBase* synth;
    std::unique_ptr<AudioFormatManager> audioFormatManager;
    std::unique_ptr<AudioFormatReaderFactory> readerFactory;
//    std::unique_ptr<AudioFormatReader> getSampleReader() const
//    {
//        return readerFactory != nullptr ? readerFactory.get()->make(*audioFormatManager) : nullptr;
//    }
};

class SampleLoadJob : public juce::ThreadPoolJob
{
public:
    SampleLoadJob(int loadType, std::unique_ptr<AudioFormatReaderFactory> ptr, AudioFormatManager* manager,
                  juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>> *soundset,
                  juce::AsyncUpdater* loadManager) :
                  juce::ThreadPoolJob("sample_loader"),
                  soundset(soundset),
                  loadManager(loadManager),
    sampleReader(std::move(ptr)), loadType(loadType), manager(manager)
    {
        int numSamplesPerLayer = 29;
        int numHarmSamples = 69;
        int numResSamples = 88;

                progressInc =
                1.0f / ((loadType == bitklavier::utils::BKLoadHeavy)  ? (numSamplesPerLayer * 8 + (numResSamples + numHarmSamples)) :
                        (loadType == bitklavier::utils::BKLoadMedium) ? (numSamplesPerLayer * 4) :
                        (loadType == bitklavier::utils::BKLoadLite)   ? (numSamplesPerLayer * 2) :
                        (loadType == bitklavier::utils::BKLoadLitest) ? (numSamplesPerLayer * 1) :
                         0.0);

    }

    double progressInc;
    JobStatus runJob() override;
    template <int layers>
    void loadMainPianoSamples();
    juce::ReferenceCountedArray<BKSamplerSound<juce::AudioFormatReader>> *soundset;
    std::unique_ptr<AudioFormatReaderFactory> sampleReader;
    AudioFormatManager* manager;
    juce::AsyncUpdater* loadManager;
    int loadType;
};

#endif //BITKLAVIER2_AUDIOFILEMANAGER_H
