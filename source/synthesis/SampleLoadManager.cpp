//
// Created by Davis Polito on 8/12/24.
//

#include "SampleLoadManager.h"
#include "Synthesiser/Sample.h"
#include "synth_base.h"
SampleLoadManager::SampleLoadManager(UserPreferences &preferences, SynthBase* synth_) : preferences(preferences) ,
audioFormatManager(new AudioFormatManager()),
synth(synth_)
{
    audioFormatManager->registerBasicFormats();
//    for(auto str : bitklavier::utils::samplepaths)
//    {
//        sampler_soundset[str];
//
//    }
}

SampleLoadManager::~SampleLoadManager()
{

}

void SampleLoadManager::handleAsyncUpdate()
{
    DBG("samples loaded, tell the audio thread its okay to change the move soundsets");
    DBG(String(sampler_soundset[globalSoundSet].size()));
    synth->processorInitQueue.try_enqueue([this]
                 {
//                     SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();
//                     object->setNodeAndPortInfo(_parent->getSynth()->addProcessor(std::move(object->getProcessorPtr())));
                        global_soundset = &sampler_soundset[globalSoundSet];
                     //last_proc.reset();
                 });
}

class MyComparator
{
public:
    static int compareElements (File first, File second) {
        if (first.getFileNameWithoutExtension() < second.getFileNameWithoutExtension())
            return -1;
        else if (first.getFileNameWithoutExtension() < second.getFileNameWithoutExtension())
            return 1;
        else
            return 0; //items are equal
    }
};
bool SampleLoadManager::loadSamples( int selection, bool isGlobal)
{

    MyComparator sorter;
   String samplePath = preferences.tree.getProperty("default_sample_path");

    samplePath.append(bitklavier::utils::samplepaths[selection], 10);
    File directory(samplePath);
    DBG(samplePath);
    Array<File> allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");
    allSamples.sort(sorter);
    auto r = new FileArrayAudioFormatReaderFactory(allSamples) ;
    //readerFactory =
    int i = 0;
    for ( auto file : allSamples)
    {
        DBG(file.getFullPathName() + " " + String(++i));
    }
    if (isGlobal)
    {
        globalSoundSet =  bitklavier::utils::samplepaths[selection];
    }

    if(!sampler_soundset.contains(bitklavier::utils::samplepaths[selection]))
    {
        sampleLoader.addJob(new SampleLoadJob(selection,
                                              std::unique_ptr<AudioFormatReaderFactory>(r),
                                                      audioFormatManager ,
                                                      &sampler_soundset[bitklavier::utils::samplepaths[selection]], this)
                            , true );
    }


}
static inline int noteNameToRoot(String name)
{
    int root = 0;
    if (name[0] == 'C') root = 0;
    else if (name[0] == 'D') root = 2;
    else if (name[0] == 'E') root = 4;
    else if (name[0] == 'F') root = 5;
    else if (name[0] == 'G') root = 7;
    else if (name[0] == 'A') root = 9;
    else if (name[0] == 'B') root = 11;

    if (name[1] == '#') root++;
    else if (name[1] == 'b') root--;

    root += 12 * name.getTrailingIntValue() + 12;

    return root;
}

juce::ThreadPoolJob::JobStatus SampleLoadJob::runJob() {


    if (loadType == bitklavier::utils::BKLoadHeavy)
    {
        loadMainPianoSamples<16>() ;
    }
    else if (loadType == bitklavier::utils::BKLoadMedium)
    {
        loadMainPianoSamples<8>() ;
    }
    else if (loadType == bitklavier::utils::BKLoadLite)
    {
        loadMainPianoSamples<4>() ;
    }
    else if (loadType == bitklavier::utils::BKLoadLitest)
    {
        loadMainPianoSamples<2>() ;
    }
    loadManager->triggerAsyncUpdate();
    return jobHasFinished;

}
template<int layers>
void SampleLoadJob::loadMainPianoSamples()
{

    const std::array<std::tuple<int, int>, layers> ranges = bitklavier::utils::VelocityRange<layers>::values;
    int  i = 0;
    float dbFSBelow  = -100.f;
    while (true) {

        auto [reader, filename] = sampleReader->make(*manager);
        // Break the loop if the reader is null
        if (!reader) {
            break;
        }

        auto sample = new Sample(*(reader.get()), 90);



        StringArray stringArray;
        stringArray.addTokens(filename, "v", "");
        String noteName = stringArray[0];
        String velLayer = stringArray[1];
        int midiNote = noteNameToRoot(noteName);
        int vel = velLayer.getIntValue();
        BigInteger midiNoteRange;
        int start = midiNote -1;

        //reset dbfs values
        if (vel == 1)
        {
            dbFSBelow = -100.f;
        }
        //setting end ranges
        if(midiNote == 9)
        {
            midiNoteRange.setRange(0, start + 3, true);
        } else if (midiNote == 93)
        {
            midiNoteRange.setRange(start, 128-start, true);
        } else midiNoteRange.setRange(start, 3, true);



        auto [begin, end] = ranges[vel -1];
        BigInteger velRange;
        velRange.setRange(begin, end - begin, true);
        auto sound = soundset->add(new BKSamplerSound(filename, std::shared_ptr<Sample<juce::AudioFormatReader>>(sample),
                                              midiNoteRange,
                                              midiNote,
                                              0,
                                              velRange,
                                              layers,
                                              (vel  + 1) / 2,
                                              dbFSBelow));

        dbFSBelow = sound->dbFSLevel;
        i++;

    }

}
