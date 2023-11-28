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
#include "../framework/note_handler.h"
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace bitklavier {


  class SoundEngine : public NoteHandler {
    public:
      static constexpr int kDefaultOversamplingAmount = 2;
      static constexpr int kDefaultSampleRate = 44100;

      SoundEngine();
      virtual ~SoundEngine();

//      void init() override;
      void processWithInput(const mono_float* audio_in, int num_samples) {
          //BITKLAVIER_ASSERT(num_samples <= output()->buffer_size);

          juce::FloatVectorOperations::disableDenormalisedNumberSupport();






      }
      void process(int num_samples);

      //void correctToTime(double seconds) override;
      int getSampleRate()
      {
          return kDefaultSampleRate;
      }
      int getNumPressedNotes();

      int getNumActiveVoices();

      mono_float getLastActiveNote() const;

      void allSoundsOff() override;
      void allNotesOff(int sample) override;
      void allNotesOff(int sample, int channel) override;
      void allNotesOffRange(int sample, int from_channel, int to_channel);

      void noteOn(int note, mono_float velocity, int sample, int channel) override;
      void noteOff(int note, mono_float lift, int sample, int channel) override;
      void setModWheel(mono_float value, int channel);
      void setModWheelAllChannels(mono_float value);
      void setPitchWheel(mono_float value, int channel);
      void setZonedPitchWheel(mono_float value, int from_channel, int to_channel);


      //void setBpm(mono_float bpm);
      void setAftertouch(mono_float note, mono_float value, int sample, int channel);
      void setChannelAftertouch(int channel, mono_float value, int sample);
      void setChannelRangeAftertouch(int from_channel, int to_channel, mono_float value, int sample);
      void setChannelSlide(int channel, mono_float value, int sample);
      void setChannelRangeSlide(int from_channel, int to_channel, mono_float value, int sample);

      void sustainOn(int channel);
      void sustainOff(int sample, int channel);
      void sostenutoOn(int channel);
      void sostenutoOff(int sample, int channel);

      void sustainOnRange(int from_channel, int to_channel);
      void sustainOffRange(int sample, int from_channel, int to_channel);
      void sostenutoOnRange(int from_channel, int to_channel);
      void sostenutoOffRange(int sample, int from_channel, int to_channel);
      force_inline int getOversamplingAmount() const { return last_oversampling_amount_; }

      void checkOversampling();

    private:
      void setOversamplingAmount(int oversampling_amount, int sample_rate);

      int last_oversampling_amount_;
      int last_sample_rate_;
//      Value* oversampling_;
//      Value* bps_;
//      Value* legato_;
//      Decimator* decimator_;
//      PeakMeter* peak_meter_;


      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoundEngine)
  };
} // namespace vital

