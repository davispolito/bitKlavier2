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

    for (int i=0; i<=8; i++)
    {
        for (auto note : allPitchClasses)
        {
            String noteOctave = note + String(i);
            allPitches.add(noteOctave);
            DBG(noteOctave);
        }
    }
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

    DBG("*** sounds of this pitch " + whichPitch);
    for ( auto file : outFiles)
    {
        DBG(file.getFullPathName());
    }
    DBG("*** done with sounds of this pitch " + whichPitch);

    return outFiles;
}

bool SampleLoadManager::loadSamples( int selection, bool isGlobal)
{
    MyComparator sorter;
    String samplePath = preferences.tree.getProperty("default_sample_path");

    //samplePath.append(bitklavier::utils::samplepaths[selection], 10); // change to "orig" to test that way
    //samplePath.append("/orig", 10);
    samplePath.append("/Yamaha", 10);
    DBG("sample path = " + samplePath);
    File directory(samplePath);
    DBG(samplePath);
    Array<File> allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");
    allSamples.sort(sorter);

    //auto r = new FileArrayAudioFormatReaderFactory(allSamples) ;

    if (isGlobal)
    {
        globalSoundSet =  bitklavier::utils::samplepaths[selection];
    }

    // loop through all 12 notes and octaves
    for (auto pitchName : allPitches)
    {
        DBG("*****>>>>> LOADING PITCH " + pitchName + " <<<<<*****");
        Array<File> onePitchSamples = samplesByPitch(pitchName, allSamples);
        if (onePitchSamples.size() <= 0) continue;
        
        auto r = new FileArrayAudioFormatReaderFactory(onePitchSamples) ;
        sampleLoader.addJob(new SampleLoadJob(
                                 selection,
                                 onePitchSamples.size(),
                                 std::unique_ptr<AudioFormatReaderFactory>(r),
                                 audioFormatManager.get(),
                                 &sampler_soundset[bitklavier::utils::samplepaths[selection]],
                                 this),
            true );
    }

    /*
    // loop through all 12 notes and octaves
    Array<File> onePitchSamples = samplesByPitch("A4", allSamples);
    auto r = new FileArrayAudioFormatReaderFactory(onePitchSamples) ;
    sampleLoader.addJob(new SampleLoadJob(
                             selection,
                             onePitchSamples.size(),
                             std::unique_ptr<AudioFormatReaderFactory>(r),
                             audioFormatManager.get(),
                             &sampler_soundset[bitklavier::utils::samplepaths[selection]],
                             this),
        true );

    onePitchSamples = samplesByPitch("C5", allSamples);
    r = new FileArrayAudioFormatReaderFactory(onePitchSamples) ;
    sampleLoader.addJob(new SampleLoadJob(
                             selection,
                             onePitchSamples.size(),
                             std::unique_ptr<AudioFormatReaderFactory>(r),
                             audioFormatManager.get(),
                             &sampler_soundset[bitklavier::utils::samplepaths[selection]],
                             this),
        true );
    */
    /*
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
        sampleLoader.addJob(new SampleLoadJob(
                                 selection,
                                 std::unique_ptr<AudioFormatReaderFactory>(r),
                                 audioFormatManager.get(),
                                 &sampler_soundset[bitklavier::utils::samplepaths[selection]],
                                 this),
                                 true );
    }
     */
}
/*
bool SampleLoadManager::loadSamples( int selection, bool isGlobal)
{

    MyComparator sorter;
    String samplePath = preferences.tree.getProperty("default_sample_path");

    samplePath.append(bitklavier::utils::samplepaths[selection], 10); // change to "orig" to test that way
    //samplePath.append("/orig", 10);
    DBG("sample path = " + samplePath);
    File directory(samplePath);
    DBG(samplePath);
    Array<File> allSamples = directory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");
    allSamples.sort(sorter);
    // look for how many of each type there are: 3 A0 samples, 7 C3 samples, 0 D# samples, 1 C# sample, etc...
    // divide up the velocity range depending on how many there are, and/or using dBFS
    // load accordingly
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
        sampleLoader.addJob(new SampleLoadJob(
                                 selection,
                                 std::unique_ptr<AudioFormatReaderFactory>(r),
                                 audioFormatManager.get() ,
                                 &sampler_soundset[bitklavier::utils::samplepaths[selection]],
                                 this)
                            , true );
    }


}
 */

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

    loadSamplesByPitch();
    /*
    if (loadType == bitklavier::utils::BKLoadHeavy)
    {
        loadMainPianoSamples<8>() ;
    }
    else if (loadType == bitklavier::utils::BKLoadMedium)
    {
        loadMainPianoSamples<4>() ;
    }
    else if (loadType == bitklavier::utils::BKLoadLite)
    {
        loadMainPianoSamples<2>() ;
    }
    else if (loadType == bitklavier::utils::BKLoadLitest)
    {
        loadMainPianoSamples<1>() ;
    }
     */

    loadManager->triggerAsyncUpdate();
    return jobHasFinished;

}

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

void SampleLoadJob::loadSamplesByPitch()
{
    auto ranges = getVelLayers(velocityLayers);
    int layers = ranges.size();
    int currentVelLayer = 0;

    //for v1 samples.
        // quietest Yamaha sample is about -41dbFS
    float dBFSBelow  = -50.f;

    while (true)
    {
        auto [reader, filename] = sampleReader->make(*manager);
        // Break the loop if the reader is null
        if (!reader) {
            break;
        }

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
        if(midiNote == 9)
        {
            midiNoteRange.setRange(0, start + 3, true);
        } else if (midiNote == 93)
        {
            midiNoteRange.setRange(start, 128-start, true);
        } else midiNoteRange.setRange(start, 3, true);

        //auto [begin, end] = ranges.values[vel];
        auto [begin, end] = ranges.getUnchecked(currentVelLayer++);
        DBG("velocity min/max = " + String(begin) + "/" + String(end));
        BigInteger velRange;
        velRange.setRange(begin, end - begin, true);

        auto sound = soundset->add(new BKSamplerSound(filename, std::shared_ptr<Sample<juce::AudioFormatReader>>(sample),
            midiNoteRange,
            midiNote,
            0,
            velRange,
            layers,
            (vel  + 1) / 2,
            dBFSBelow));

        dBFSBelow = sound->dBFSLevel; // to pass on to next sample, which should be the next velocity layer above

        DBG("**** loading sample: " + filename);
        DBG("");

    }
    DBG("done loading string samples");

}

template<int layers>
void SampleLoadJob::loadMainPianoSamples()
{

    auto ranges = getVelLayers(velocityLayers);

    //auto ranges = bitklavier::utils::VelocityRange<layers>();
    int  i = 0;
    float dBFSBelow  = -100.f;
    while (true)
    {
        auto [reader, filename] = sampleReader->make(*manager);
        // Break the loop if the reader is null
        if (!reader) {
            break;
        }

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

        //reset dbfs values
        if (vel == 1) dBFSBelow = -100.f;

        //setting end ranges
        if(midiNote == 9)
        {
            midiNoteRange.setRange(0, start + 3, true);
        } else if (midiNote == 93)
        {
            midiNoteRange.setRange(start, 128-start, true);
        } else midiNoteRange.setRange(start, 3, true);

        //auto [begin, end] = ranges.values[vel];
        auto [begin, end] = ranges.getUnchecked(i);
        DBG("velocity min/max = " + String(begin) + "/" + String(end));
        BigInteger velRange;
        velRange.setRange(begin, end - begin, true);
        auto sound = soundset->add(new BKSamplerSound(filename, std::shared_ptr<Sample<juce::AudioFormatReader>>(sample),
                                              midiNoteRange,
                                              midiNote,
                                              0,
                                              velRange,
                                              layers,
                                              (vel  + 1) / 2,
                                              dBFSBelow));

        dBFSBelow = sound->dBFSLevel;
        i++;

        DBG("**** loading sample: " + filename);
        DBG("");

    }
    DBG("done loading samples");

}
