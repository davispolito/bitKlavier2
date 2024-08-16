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

#include "utils.h"

namespace bitklavier {

  constexpr float kPcmScale = 32767.0f;
  constexpr float kComplexAmplitudePcmScale = 50.0f;
  constexpr float kComplexPhasePcmScale = 10000.0f;

  namespace utils {
    int RandomGenerator::next_seed_ = 0;

    mono_float encodeOrderToFloat(int* order, int size) {
      // Max array size you can encode in 32 bits.
      BITKLAVIER_ASSERT(size <= kMaxOrderLength);

      unsigned int code = 0;
      for (int i = 1; i < size; ++i) {

        int index = 0;
        for (int j = 0; j < i; ++j)
          index += order[i] < order[j];

        code *= i + 1;
        code += index;
      }

      return code;
    }

    void decodeFloatToOrder(int* order, mono_float float_code, int size) {
      // Max array size you can encode in 32 bits.
      BITKLAVIER_ASSERT(size <= kMaxOrderLength);

      int code = float_code;
      for (int i = 0; i < size; ++i)
        order[i] = i;

      for (int i = 0; i < size; ++i) {
        int remaining = size - i;
        int index = remaining - 1;
        int inversions = code % remaining;
        code /= remaining;

        int placement = order[index - inversions];
        for (int j = index - inversions; j < index; ++j)
          order[j] = order[j + 1];

        order[index] = placement;
      }
    }

    void floatToPcmData(int16_t* pcm_data, const float* float_data, int size) {
      for (int i = 0; i < size; ++i)
        pcm_data[i] = utils::clamp(float_data[i] * kPcmScale, -kPcmScale, kPcmScale);
    }

    void complexToPcmData(int16_t* pcm_data, const std::complex<float>* complex_data, int size) {
      for (int i = 0; i < size / 2; ++i) {
        float amp = std::abs(complex_data[i]);
        float phase = std::arg(complex_data[i]);
        pcm_data[i * 2] = utils::clamp(amp * kComplexAmplitudePcmScale, -kPcmScale, kPcmScale);
        pcm_data[i * 2 + 1] = utils::clamp(phase * kComplexPhasePcmScale, -kPcmScale, kPcmScale);
      }
    }

    void pcmToFloatData(float* float_data, const int16_t* pcm_data, int size) {
      for (int i = 0; i < size; ++i)
        float_data[i] = pcm_data[i] * (1.0f / kPcmScale);
    }

    void pcmToComplexData(std::complex<float>* complex_data, const int16_t* pcm_data, int size) {
      for (int i = 0; i < size / 2; ++i) {
        float amp = pcm_data[i * 2] * (1.0f / kComplexAmplitudePcmScale);
        float phase = pcm_data[i * 2 + 1] * (1.0f / kComplexPhasePcmScale);
        complex_data[i] = std::polar(amp, phase);
      }
    }

      // Define the tuples for N = 16
      template<>
      const std::array<std::tuple<int, int>, 16> VelocityRange<16>::values = {
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
      const std::array<std::tuple<int, int>, 8> VelocityRange<8>::values = {
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
      const std::array<std::tuple<int, int>, 4> VelocityRange<4>::values = {
              std::make_tuple(0, 42),
              std::make_tuple(42, 76),
              std::make_tuple(76, 104),
              std::make_tuple(104, 128)
      };


// Define the tuples for N = 2
      template<>
      const std::array<std::tuple<int, int>, 2> VelocityRange<2>::values = {
              std::make_tuple(0, 76),
              std::make_tuple(76, 128)
      };


// Define the tuples for N = 1
      template<>
      const std::array<std::tuple<int, int>, 1> VelocityRange<1>::values = {
              std::make_tuple(0, 128)
      };
  } // namespace utils
} // namespace vital
