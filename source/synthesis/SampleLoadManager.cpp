//
// Created by Davis Polito on 8/12/24.
//

#include "SampleLoadManager.h"
#include "Synthesiser/Sample.h"
#include "synth_base.h"


/**
 * ******* SampleLoadManager stuff *******
 */

SampleLoadManager::SampleLoadManager(UserPreferences &preferences, SynthBase* synth_) : preferences(preferences) ,
audioFormatManager(new AudioFormatManager()),
synth(synth_)
{
    audioFormatManager->registerBasicFormats();

    // compile list of all string names on the piano A0 through C8
    for (int i=0; i<=8; i++)
    {
        for (auto note : allPitchClasses)
        {
            String noteOctave = note + String(i);
            allPitches.add(noteOctave);
            // DBG(noteOctave);
        }
    }
}

SampleLoadManager::~SampleLoadManager()
{

}

void SampleLoadManager::handleAsyncUpdate()
{
    DBG("samples loaded, tell the audio thread its okay to change the move soundsets");
    DBG("samplerSoundset size = " + String(samplerSoundset[globalSoundset_name].size()));
    synth->processorInitQueue.try_enqueue([this]
                 {
                    //SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();
                    //object->setNodeAndPortInfo(_parent->getSynth()->addProcessor(std::move(object->getProcessorPtr())));

                    globalSoundset = &samplerSoundset[globalSoundset_name];
                    globalHammersSoundset = &samplerSoundset[globalHammersSoundset_name];
                    globalReleaseResonanceSoundset = &samplerSoundset[globalReleaseResonanceSoundset_name];
                    //last_proc.reset();
                 });
}

// sort array of sample files into arrays of velocities by pitch
// specific to filename format for bK sample libraries
Array<File> SampleLoadManager::samplesByPitch(String whichPitch, Array<File> inFiles)
{

    VelocityComparator vsorter;

    Array<File> outFiles;
    bool sharp = false;

    if (whichPitch.contains("#")) sharp = true;

    for ( auto file : inFiles)
    {
        String fileToPlace = file.getFileNameWithoutExtension();
        if (fileToPlace.contains("harm")) {
            fileToPlace = fileToPlace.replace("harm", "");
            //fileToPlace.removeCharacters("harm");
            //DBG("removing 'harm' from filename, new name = " + fileToPlace);
        }

        if (sharp)
        {
            if (fileToPlace.startsWith (whichPitch))
            {
                outFiles.add (file);
            }
        }
        else
        {
            if (fileToPlace.startsWith (whichPitch) && !fileToPlace.contains("#"))
            {
                outFiles.add (file);
            }
        }
    }

    // need to sort correctly by velocity....
    outFiles.sort(vsorter);
    return outFiles;
}

void SampleLoadManager::loadSamples_sub(bitklavier::utils::BKPianoSampleType thisSampleType)
{
    using namespace bitklavier::utils;

    // maybe better to do this with templates, but for now...
    String soundsetName;
    if (thisSampleType == BKPianoMain)
        soundsetName = globalSoundset_name;
    else if (thisSampleType == BKPianoHammer)
        soundsetName = globalHammersSoundset_name;
    else if (thisSampleType == BKPianoReleaseResonance)
        soundsetName = globalReleaseResonanceSoundset_name;
    else if (thisSampleType == BKPianoPedal)
        soundsetName = globalPedalsSoundset_name;

    DBG("loadSamples_sub " + soundsetName);

    String samplePath = preferences.tree.getProperty("default_sample_path");
    samplePath.append("/Default", 20);
    samplePath.append(BKPianoSampleType_string[thisSampleType], 20); // subfolders need to be named accordingly
    File directory(samplePath);
    Array<File> allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");

    // sort alphabetically
    MyComparator sorter;
    allSamples.sort(sorter);

    // loop through all 12 notes and octaves
    for (auto pitchName : allPitches)
    {
        DBG("*****>>>>> LOADING " + String(BKPianoSampleType_string[thisSampleType]) + " " + pitchName + " <<<<<*****");

        Array<File> onePitchSamples(allSamples);
        DBG("onePitchSamples.size = " + String(onePitchSamples.size()));
        if (thisSampleType == BKPianoMain || thisSampleType == BKPianoReleaseResonance)
            onePitchSamples = samplesByPitch(pitchName, allSamples);
        else if (thisSampleType == BKPianoHammer)
        {
            DBG ("noteNameToRoot = " + noteNameToRoot(pitchName));
            onePitchSamples.clear();
            for (auto thisFile : allSamples)
            {
                // isolate MIDI
                StringArray stringArray;
                stringArray.addTokens(thisFile.getFileName(), "l", "");
                int midiNote = stringArray[1].getIntValue();
                if (midiNote == noteNameToRoot(pitchName))
                {
                    onePitchSamples.add(thisFile);
                    break;
                }
            }
        }

        DBG("onePitchSamples.size = " + String(onePitchSamples.size()));
        if (onePitchSamples.size() <= 0) continue; // skip if no samples at this pitch

        auto r = new FileArrayAudioFormatReaderFactory(onePitchSamples) ;
        sampleLoader.addJob(new SampleLoadJob(
                                 thisSampleType,
                                 //bitklavier::utils::BKPianoMain,
                                 //allSamples.size(),
                                 onePitchSamples.size(),
                                 std::unique_ptr<AudioFormatReaderFactory>(r),
                                 audioFormatManager.get(),
                                 //&samplerSoundset[globalSoundset_name],
                                 &samplerSoundset[soundsetName],
                                 this),
            true );
    }
}

bool SampleLoadManager::loadSamples( int selection, bool isGlobal)
{
    // save this set as the global set
    if (isGlobal)
    {
        globalSoundset_name =  bitklavier::utils::samplepaths[selection];
        globalHammersSoundset_name = bitklavier::utils::samplepaths[selection] + "Hammers";
        globalReleaseResonanceSoundset_name = bitklavier::utils::samplepaths[selection] + "ReleaseResonance";
    }

    /**
     * Load Main samples
     */

    String samplePath = preferences.tree.getProperty("default_sample_path");
    samplePath.append("/Default/main", 20);
    File directory(samplePath);
    Array<File> allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");

    // sort alphabetically
    MyComparator sorter;
    allSamples.sort(sorter);


    loadSamples_sub(bitklavier::utils::BKPianoMain);
    loadSamples_sub(bitklavier::utils::BKPianoHammer);
    loadSamples_sub(bitklavier::utils::BKPianoReleaseResonance);
    // loop through all 12 notes and octaves

    /*
    for (auto pitchName : allPitches)
    {
        DBG("*****>>>>> LOADING PITCH " + pitchName + " <<<<<*****");
        Array<File> onePitchSamples = samplesByPitch(pitchName, allSamples);
        if (onePitchSamples.size() <= 0) continue; // skip if no samples at this pitch
        
        auto r = new FileArrayAudioFormatReaderFactory(onePitchSamples) ;
        sampleLoader.addJob(new SampleLoadJob(
                                 bitklavier::utils::BKPianoMain,
                                 //selection,
                                 onePitchSamples.size(),
                                 std::unique_ptr<AudioFormatReaderFactory>(r),
                                 audioFormatManager.get(),
                                 //&samplerSoundset[bitklavier::utils::samplepaths[selection]],
                                 &samplerSoundset[globalSoundset_name],
                                 this),
            true );
    }
     */


    /**
     * Load Hammer Samples
     */

    /*
    samplePath = preferences.tree.getProperty("default_sample_path");
    samplePath.append("/Default/hammer", 20);
    directory = samplePath;
    allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");
    allSamples.sort(sorter);

    //auto r = new FileArrayAudioFormatReaderFactory(allSamples);
    for (auto pitchName : allPitches)
    {
        DBG("*****>>>>> LOADING HAMMER " + pitchName + " <<<<<*****");

        if (allSamples.size() <= 0) continue; // skip if no samples at this pitch
        for (auto thisFile : allSamples)
        {
            // isolate MIDI
            StringArray stringArray;
            stringArray.addTokens(thisFile.getFileName(), "l", "");
            int midiNote = stringArray[1].getIntValue();
            if (midiNote == noteNameToRoot(pitchName))
            {
                DBG ("thisFile " + thisFile.getFileName() + " matches " + pitchName);

                auto r = new FileArrayAudioFormatReaderFactory (thisFile);
                sampleLoader.addJob (new SampleLoadJob (
                                         //selection,
                                         bitklavier::utils::BKPianoHammer,
                                         1, // only one velocity layer for these
                                         std::unique_ptr<AudioFormatReaderFactory> (r),
                                         audioFormatManager.get(),
                                         //&samplerSoundset[bitklavier::utils::samplepaths[selection]],
                                         &samplerSoundset[globalHammersSoundset_name],
                                         this),
                    true);
            }

        }
        */

        /*
        auto r = new FileArrayAudioFormatReaderFactory(allSamples) ;
        sampleLoader.addJob(new SampleLoadJob(
                                 //selection,
                                 bitklavier::utils::BKPianoHammer,
                                 1, // only one velocity layer for these
                                 std::unique_ptr<AudioFormatReaderFactory>(r),
                                 audioFormatManager.get(),
                                 //&samplerSoundset[bitklavier::utils::samplepaths[selection]],
                                 &samplerSoundset[globalHammersSoundset_name],
                                 this),
            true );

    }
         */


    /**
     * then do the same for releaseResonance and pedal samples
     */
}

/**
 * ******* SampleLoadJob Stuff *******
 */

juce::ThreadPoolJob::JobStatus SampleLoadJob::runJob()
{
    loadSamples();
    loadManager->triggerAsyncUpdate();
    return jobHasFinished;
}

// divide up into velocity layers; create tuples with min/max velocities, based on howmany
Array<std::tuple<int, int>> SampleLoadJob::getVelLayers (int howmany)
{
    Array<std::tuple<int, int>> layersToReturn;

    // load from custom velocity layers
    if (howmany == 1)
    {
        layersToReturn.add(bitklavier::utils::VelocityRange<1>().values[0]);
    }
    else if (howmany == 2)
    {
        for ( auto vtuple : bitklavier::utils::VelocityRange<2>().values)
        {
            layersToReturn.add(vtuple);
        }
    }
    else if (howmany == 4)
    {
        for ( auto vtuple : bitklavier::utils::VelocityRange<4>().values)
        {
            layersToReturn.add(vtuple);
        }
    }
    else if (howmany == 8)
    {
        for ( auto vtuple : bitklavier::utils::VelocityRange<8>().values)
        {
            layersToReturn.add(vtuple);
        }
    }
    else //otherwise, divide up the velocity range evenly
    {
        int velThreshold = 0;
        int velIncrement = 128 / howmany;

        while (velThreshold < 128)
        {
            auto vtuple = std::make_tuple(velThreshold, velThreshold + velIncrement);
            velThreshold += velIncrement;
            layersToReturn.add(vtuple);
        }
    }

    for ( auto vtuple : layersToReturn)
    {
        auto [begin, end] = vtuple;
    }

    return layersToReturn;
}

void SampleLoadJob::loadSamples()
{
    using namespace bitklavier::utils;
    DBG("loadSamples called for " + String(BKPianoSampleType_string[thisSampleType]));

    if      (   thisSampleType == BKPianoMain               ) loadMainSamplesByPitch();
    else if (   thisSampleType == BKPianoHammer             ) loadHammerSamples();
    else if (   thisSampleType == BKPianoReleaseResonance   ) loadReleaseResonanceSamples();
    else if (   thisSampleType == BKPianoPedal              ) loadPedalSamples();
}

void SampleLoadJob::loadHammerSamples()
{
    //for v1 samples, to define bottom threshold.
    //  for hammers, just to define a baseline, since we don't have velocity layers here
    float dBFSBelow  = -50.f;

    while (true)
    {
        auto [reader, filename] = sampleReader->make(*manager);
        if (!reader) break; // Break the loop if the reader is null

        auto sample = new Sample(*(reader.get()), 90);

        // isolate MIDI
        StringArray stringArray;
        stringArray.addTokens(filename, "l", "");
        int midiNote = stringArray[1].getIntValue();

        DBG("**** loading hammer sample: " + filename + " " + String(midiNote));

        //setting end ranges
        BigInteger midiNoteRange;
        if(midiNote == 9) midiNoteRange.setRange(0, midiNote, true);
        else if (midiNote == 88) midiNoteRange.setRange(midiNote, 128-midiNote, true);
        else midiNoteRange.setRange(midiNote, 1, true);

        BigInteger velRange;
        velRange.setRange(0, 128, true);

        auto sound = soundset->add(new BKSamplerSound(filename, std::shared_ptr<Sample<juce::AudioFormatReader>>(sample),
            midiNoteRange,
            midiNote,
            0,
            velRange,
            1,
            dBFSBelow));
    }

    DBG("done loading hammer samples");
}

void SampleLoadJob::loadReleaseResonanceSamples()
{
    DBG("loadReleaseResonanceSamples called");
    auto ranges = getVelLayers(velocityLayers);
    int layers = ranges.size();
    int currentVelLayer = 0;

    //for v1 samples, to define bottom threshold.
    //  quietest Yamaha sample is about -41dbFS, so -50 seems safe and reasonable
    float dBFSBelow  = -50.f;

    while (true)
    {
        auto [reader, filename] = sampleReader->make(*manager);
        if (!reader) break; // Break the loop if the reader is null

        DBG("**** loading resonance sample: " + filename);
        auto sample = new Sample(*(reader.get()), 90);

        StringArray stringArray;
        stringArray.addTokens(filename, "v", "");
        String noteName = stringArray[0].removeCharacters("harm");
        String velLayer = stringArray[1];
        int midiNote = noteNameToRoot(noteName);
        int vel = velLayer.getIntValue();
        BigInteger midiNoteRange;
        int start = midiNote - 1;

        //setting end ranges
        // **** need to generalize this if we want to be able to open any random collection and assign to an appropriate range
        if(midiNote == 9) midiNoteRange.setRange(0, start + 3, true);
        else if (midiNote == 93) midiNoteRange.setRange(start, 128-start, true);
        else midiNoteRange.setRange(start, 3, true);

        //auto [begin, end] = ranges.values[vel];
        auto [begin, end] = ranges.getUnchecked(currentVelLayer++);
        //DBG("velocity min/max = " + String(begin) + "/" + String(end));
        BigInteger velRange;
        velRange.setRange(begin, end - begin, true);

        auto sound = soundset->add(new BKSamplerSound(filename, std::shared_ptr<Sample<juce::AudioFormatReader>>(sample),
            midiNoteRange,
            midiNote,
            0,
            velRange,
            layers,
            dBFSBelow));

        dBFSBelow = sound->dBFSLevel; // to pass on to next sample, which should be the next velocity layer above

        DBG("**** loading resonance sample: " + filename);
        DBG("");

    }
    DBG("done loading resonance samples, with this many velocity layers " + String(currentVelLayer));
}

void SampleLoadJob::loadPedalSamples()
{

}

// load all the velocity layers samples for a particular pitch/key
void SampleLoadJob::loadMainSamplesByPitch()
{
    auto ranges = getVelLayers(velocityLayers);
    int layers = ranges.size();
    int currentVelLayer = 0;

    //for v1 samples, to define bottom threshold.
    //  quietest Yamaha sample is about -41dbFS, so -50 seems safe and reasonable
    float dBFSBelow  = -50.f;

    while (true)
    {
        auto [reader, filename] = sampleReader->make(*manager);
        if (!reader) break; // Break the loop if the reader is null

        DBG("**** loading sample: " + filename);
        auto sample = new Sample(*(reader.get()), 90);

        StringArray stringArray;
        stringArray.addTokens(filename, "v", "");
        String noteName = stringArray[0];
        String velLayer = stringArray[1];
        int midiNote = noteNameToRoot(noteName);
        int vel = velLayer.getIntValue();
        BigInteger midiNoteRange;
        int start = midiNote -1;

        //setting end ranges
        if(midiNote == 9) midiNoteRange.setRange(0, start + 3, true);
        else if (midiNote == 93) midiNoteRange.setRange(start, 128-start, true);
        else midiNoteRange.setRange(start, 3, true);

        //auto [begin, end] = ranges.values[vel];
        auto [begin, end] = ranges.getUnchecked(currentVelLayer++);
        //DBG("velocity min/max = " + String(begin) + "/" + String(end));
        BigInteger velRange;
        velRange.setRange(begin, end - begin, true);

        auto sound = soundset->add(new BKSamplerSound(filename, std::shared_ptr<Sample<juce::AudioFormatReader>>(sample),
            midiNoteRange,
            midiNote,
            0,
            velRange,
            layers,
            dBFSBelow));

        dBFSBelow = sound->dBFSLevel; // to pass on to next sample, which should be the next velocity layer above

        DBG("**** loading sample: " + filename);
        DBG("");

    }
    DBG("done loading string samples");
}