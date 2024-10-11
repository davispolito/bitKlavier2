//
// Created by Davis Polito on 8/12/24.
//

#include "SampleLoadManager.h"
#include "Synthesiser/Sample.h"
#include "synth_base.h"

// move all of this stuff somewhere else?
namespace bitklavier {
    namespace utils {
        // Define the tuples for N = 16
        template<>
        constexpr std::array<std::tuple<int, int>, 16> VelocityRange<16>::values = {
            std::make_tuple(0, 8),
            std::make_tuple(8, 16),
            std::make_tuple(16, 24),
            std::make_tuple(24, 32),
            std::make_tuple(32, 40),
            std::make_tuple(40, 48),
            std::make_tuple(48, 56),
            std::make_tuple(56, 64),
            std::make_tuple(64, 72),
            std::make_tuple(72, 80),
            std::make_tuple(80, 88),
            std::make_tuple(88, 96),
            std::make_tuple(96, 104),
            std::make_tuple(104, 112),
            std::make_tuple(112, 120),
            std::make_tuple(120, 128)
        };
        // Define the tuples for N = 8
        template<>
        constexpr std::array<std::tuple<int, int>, 8> VelocityRange<8>::values = {
            std::make_tuple(0, 30),
            std::make_tuple(30, 50),
            std::make_tuple(50, 68),
            std::make_tuple(68, 84),
            std::make_tuple(84, 98),
            std::make_tuple(98, 110),
            std::make_tuple(110, 120),
            std::make_tuple(120, 128)
        };

        // Define the tuples for N = 4
        template<>
        constexpr std::array<std::tuple<int, int>, 4> VelocityRange<4>::values = {
            std::make_tuple(0, 42),
            std::make_tuple(42, 76),
            std::make_tuple(76, 104),
            std::make_tuple(104, 128)
        };


        // Define the tuples for N = 2
        template<>
        constexpr std::array<std::tuple<int, int>, 2> VelocityRange<2>::values = {
            std::make_tuple(0, 76),
            std::make_tuple(76, 128)
        };


        // Define the tuples for N = 1
        template<>
        constexpr std::array<std::tuple<int, int>, 1> VelocityRange<1>::values = {
            std::make_tuple(0, 128)
        };
    }
}

// standard filename comparator, though doesn't handle velocities well
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

// finds velocities in sample names and uses those to sort
class VelocityComparator
{
public:
    static int compareElements (File first, File second) {
        // assumes sample names of form [PitchLetter(s)][octave]v[velocity].wav
        //      so A3v13.wav, of C#0v2.wav
        //      with only sharps, and ABCDEFG
        // find where the 'v' is, and where the '.' is for the suffix
        auto vIndexFirst    = first.getFileName().indexOfChar('v') + 1;
        auto vIndexSecond   = second.getFileName().indexOfChar('v') + 1;
        auto vEndIndexFirst = first.getFileName().indexOfChar('.');
        auto vEndIndexSecond= second.getFileName().indexOfChar('.');

        // get the velocity as an int
        int velocityFirst = first.getFileName().substring(vIndexFirst, vEndIndexFirst).getIntValue();
        int velocitySecond = second.getFileName().substring(vIndexSecond, vEndIndexSecond).getIntValue();

        // compare as ints
        if (velocityFirst < velocitySecond)
            return -1;
        else if (velocityFirst < velocitySecond)
            return 1;
        else
            return 0; //items are equal
    }
};

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
                    //last_proc.reset();
                 });
}

// sort array of sample files into arrays of velocities by pitch
Array<File> SampleLoadManager::samplesByPitch(String whichPitch, Array<File> inFiles)
{
    VelocityComparator vsorter;

    Array<File> outFiles;
    bool sharp = false;

    if (whichPitch.contains("#")) sharp = true;

    for ( auto file : inFiles)
    {
        String fileToPlace = file.getFileNameWithoutExtension();
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

bool SampleLoadManager::loadSamples( int selection, bool isGlobal)
{
    // open sample directory
    String samplePath = preferences.tree.getProperty("default_sample_path");
    samplePath.append("/Yamaha/_main", 20);
    File directory(samplePath);
    Array<File> allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");

    // sort alphabetically
    MyComparator sorter;
    allSamples.sort(sorter);

    // save this set as the global set
    if (isGlobal)
    {
        globalSoundset_name =  bitklavier::utils::samplepaths[selection];
        globalHammersSoundset_name = bitklavier::utils::samplepaths[selection] + "Hammers";
    }

    /**
     * Load Main samples
     */

    // loop through all 12 notes and octaves
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

    /**
     * Load Hammer Samples
     */

    // now load hammer release samples ("rel")
    // refactor this and above, if possible
    samplePath = preferences.tree.getProperty("default_sample_path");
    samplePath.append("/Yamaha/_rel", 20);
    directory = samplePath;
    allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");
    //allSamples.sort(sorter);

    //need to sort out filename ordering and so on.
    auto r = new FileArrayAudioFormatReaderFactory(allSamples) ;
    for (auto pitchName : allPitches)
    {
        DBG("*****>>>>> LOADING HAMMER " + pitchName + " <<<<<*****");
        if (allSamples.size() <= 0) continue; // skip if no samples at this pitch

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
        DBG("new velocity layer start: " + String(begin));
        DBG("new velocity layer end: " + String(end));
    }

    return layersToReturn;
}

void SampleLoadJob::loadSamples()
{
    using namespace bitklavier::utils;

    DBG("thisSample Type = " + String(thisSampleType));

    if      (   thisSampleType == BKPianoMain               ) loadMainSamplesByPitch();
    else if (   thisSampleType == BKPianoHammer             ) loadHammerSamples();
    else if (   thisSampleType == BKPianoReleaseResonance   ) loadReleaseResonanceSamples();
    else if (   thisSampleType == BKPianoPedal              ) loadPedalSamples();
}

void SampleLoadJob::loadHammerSamples()
{
    //for v1 samples, to define bottom threshold.
    //  quietest Yamaha sample is about -41dbFS, so -50 seems safe and reasonable
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