/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cmath>
#include <complex>
#include <cstdlib>
#include <random>

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "common.h"
#define NAME_AND_VALUE(x) #x, x

  namespace bitklavier::utils {


    constexpr mono_float kDbGainConversionMult = 20.0f;
    constexpr int kMaxOrderLength = 10;
    constexpr mono_float kLogOf2 = 0.69314718056f;
    constexpr mono_float kInvLogOf2 = 1.44269504089f;

    constexpr int factorial(int value) {
      int result = 1;
      for (int i = 2; i <= value; ++i)
        result *= i;

      return result;
    }

    typedef union {
      int i;
      mono_float f;
    } int_float;

    class RandomGenerator {
      public:
        static int next_seed_;
          
        RandomGenerator(mono_float min, mono_float max) : engine_(next_seed_++), distribution_(min, max) { }
        RandomGenerator(const RandomGenerator& other) :
            engine_(next_seed_++), distribution_(other.distribution_.min(), other.distribution_.max()) { }

        force_inline mono_float next() {
          return distribution_(engine_);
        }

//        force_inline poly_float polyNext() {
//          poly_float result;
//          for (int i = 0; i < poly_float::kSize; ++i)
//            result.set(i, next());
//          return result;
//        }
//
//        force_inline poly_float polyVoiceNext() {
//          poly_float result;
//          for (int i = 0; i < poly_float::kSize; i += 2) {
//            mono_float value = next();
//            result.set(i, value);
//            result.set(i + 1, value);
//          }
//          return result;
//        }
//
//        force_inline poly_float polyNext(poly_mask mask) {
//          poly_float result = 0.0f;
//          for (int i = 0; i < poly_float::kSize; ++i) {
//            if (mask[i])
//              result.set(i, next());
//          }
//          return result;
//        }

        force_inline void seed(int new_seed) {
          engine_.seed(new_seed);
        }

      private:
        std::mt19937 engine_;
        std::uniform_real_distribution<mono_float> distribution_;

        JUCE_LEAK_DETECTOR(RandomGenerator)
    };

    force_inline mono_float intToFloatBits(int i) {
      int_float convert;
      convert.i = i;
      return convert.f;
    }

    force_inline int floatToIntBits(mono_float f) {
      int_float convert;
      convert.f = f;
      return convert.i;
    }

    force_inline mono_float min(mono_float one, mono_float two) {
      return fmin(one, two);
    }

    force_inline mono_float max(mono_float one, mono_float two) {
      return fmax(one, two);
    }

    force_inline mono_float clamp(mono_float value, mono_float min, mono_float max) {
      return fmin(max, fmax(value, min));
    }

    template<class T>
    force_inline T pass(T input) {
      return input;
    }

    force_inline int imax(int one, int two) {
      return (one > two) ? one : two;
    }

    force_inline int imin(int one, int two) {
      return (one > two) ? two : one;
    }

    force_inline double interpolate(double from, double to, double t) {
      return t * (to - from) + from;
    }

    force_inline mono_float interpolate(mono_float from, mono_float to, mono_float t) {
      return from + t * (to - from);
    }
    
    force_inline mono_float mod(double value, double* divisor) {
      return modf(value, divisor);
    }

    force_inline mono_float mod(float value, float* divisor) {
      return modff(value, divisor);
    }

    force_inline int iclamp(int value, int min, int max) {
      return value > max ? max : (value < min ? min : value);
    }

    force_inline int ilog2(int value) {
    #if defined(__GNUC__) || defined(__clang__)
      constexpr int kMaxBitIndex = sizeof(int) * 8 - 1;
      return kMaxBitIndex - __builtin_clz(std::max(value, 1));
    #elif defined(_MSC_VER)
      unsigned long result = 0;
      _BitScanReverse(&result, value);
      return result;
    #else
      int num = 0;
      while (value >>= 1)
        num++;
      return num;
    #endif
    }

    force_inline bool closeToZero(mono_float value) {
      return value <= kEpsilon && value >= -kEpsilon;
    }

    force_inline mono_float magnitudeToDb(mono_float magnitude) {
      return kDbGainConversionMult * log10f(magnitude);
    }

    force_inline mono_float dbToMagnitude(mono_float decibels) {
      return powf(10.0f, decibels / kDbGainConversionMult);
    }

    force_inline mono_float centsToRatio(mono_float cents) {
      return powf(2.0f, cents / kCentsPerOctave);
    }

    force_inline mono_float noteOffsetToRatio(mono_float cents) {
      return powf(2.0f, cents / kNotesPerOctave);
    }

    force_inline mono_float ratioToMidiTranspose(mono_float ratio) {
      return logf(ratio) * (kInvLogOf2 * kNotesPerOctave);
    }

    force_inline mono_float midiCentsToFrequency(mono_float cents) {
      return kMidi0Frequency * centsToRatio(cents);
    }

    force_inline mono_float midiNoteToFrequency(mono_float note) {
      return midiCentsToFrequency(note * kCentsPerNote);
    }

    force_inline mono_float frequencyToMidiNote(mono_float frequency) {
      return kNotesPerOctave * logf(frequency / kMidi0Frequency) * kInvLogOf2;
    }

    force_inline mono_float frequencyToMidiCents(mono_float frequency) {
      return kCentsPerNote * frequencyToMidiNote(frequency);
    }

    force_inline int nextPowerOfTwo(float value) {
      return roundf(powf(2.0f, ceilf(logf(value) * kInvLogOf2)));
    }

    force_inline bool isSilent(const mono_float* buffer, int length) {
      for (int i = 0; i < length; ++i) {
        if (!closeToZero(buffer[i]))
          return false;
      }
      return true;
    }

    force_inline mono_float rms(const mono_float* buffer, int num) {
      mono_float square_total = 0.0f;
      for (int i = 0; i < num; ++i)
        square_total += buffer[i] * buffer[i];

      return sqrtf(square_total / num);
    }

    force_inline mono_float inversePowerScale(mono_float t) {
      return 2.0f * logf((-t + 1.0f) / t);
    }

    force_inline mono_float inverseFltScale(mono_float t) {
      return (t - 1.0f) / t;
    }

    mono_float encodeOrderToFloat(int* order, int size);
    void decodeFloatToOrder(int* order, mono_float float_code, int size);
    void floatToPcmData(int16_t* pcm_data, const float* float_data, int size);
    void complexToPcmData(int16_t* pcm_data, const std::complex<float>* complex_data, int size);
    void pcmToFloatData(float* float_data, const int16_t* pcm_data, int size);
    void pcmToComplexData(std::complex<float>* complex_data, const int16_t* pcm_data, int size);

    typedef enum BKSampleLoadType
    {
        BKLoadDefault = 0,
        BKLoadSampleSet2,
        BKLoadSampleSet3,
        BKLoadSampleSet4,
        BKNumSampleLoadTypes
    } BKSampleLoadType;

    const std::string samplepaths[BKSampleLoadType::BKNumSampleLoadTypes]
        {
         "/default",
         "/sampleset2", // made up for now...
         "/sampleset3",
         "/sampleset4"
        };

    typedef enum BKPianoSampleType
    {
        BKPianoMain = 0,
        BKPianoHammer,
        BKPianoReleaseResonance,
        BKPianoPedal,
        BKPianoSampleTypes
    } BKPianoSampleType;

    const std::string BKPianoSampleType_string[BKPianoSampleType::BKPianoSampleTypes]
    {
        "/main",
        "/hammer",
        "/resonance",
        "/pedal"
    };

// Template structure to define velocity ranges based on the number
      // Primary template declaration
      template<size_t N>
      struct VelocityRange
      {
          static constexpr std::array<std::tuple<int, int>, N> values; // Declaration
      };

// Initialize the primary template's static member (empty in this case)
//      template<int N>
//      constexpr std::array<std::tuple<int, int>, N> VelocityRange<N>::values = {};


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
  



  } // namespace bitklavier::utils

// namespace vital

// standard filename comparator, though doesn't handle velocities well
class MyComparator
{
public:
    static int compareElements (juce::File first, juce::File second) {
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
    static int compareElements (juce::File first, juce::File second) {
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

static inline int noteNameToRoot(juce::String name)
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

static double mtof( double f )
{
    if( f <= -1500 ) return (0);
    else if( f > 1499 ) return (mtof(1499));
    // else return (8.17579891564 * exp(.0577622650 * f));
    // TODO: optimize
    else return ( pow(2, (f - 69) / 12.0) * 440.0 );
}