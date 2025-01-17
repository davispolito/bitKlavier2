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

#include "sound_engine.h"
#include "AudioProcessorGraph.h"


namespace bitklavier {

  SoundEngine::SoundEngine() : /*voice_handler_(nullptr),*/
                                last_oversampling_amount_(-1), last_sample_rate_(-1),
                                processorGraph(new juce::AudioProcessorGraph())
                                {

    processorGraph->clear();
    processorGraph->enableAllBuses();
  }

  SoundEngine::~SoundEngine() {

  }


  void SoundEngine::setOversamplingAmount(int oversampling_amount, int sample_rate) {
    static constexpr int kBaseSampleRate = 44100;
    
    int oversample = oversampling_amount;
    int sample_rate_mult = sample_rate / kBaseSampleRate;
    while (sample_rate_mult > 1 && oversample > 1) {
      sample_rate_mult >>= 1;
      oversample >>= 1;
    }
    //voice_handler_->setOversampleAmount(oversample);

    last_oversampling_amount_ = oversampling_amount;
    last_sample_rate_ = sample_rate;
  }

  void SoundEngine::process(int num_samples, juce::AudioSampleBuffer &buffer )
  {
    //VITAL_ASSERT(num_samples <= output()->buffer_size);
    juce::FloatVectorOperations::disableDenormalisedNumberSupport();

    juce::MidiBuffer midimessages;
    for(auto proc : processors)
    {
        proc->processBlock(buffer, midimessages);
    }
    if (getNumActiveVoices() == 0)
    {
      //      CircularQueue<ModulationConnectionProcessor*>& connections = voice_handler_->enabledModulationConnection();
      //      for (ModulationConnectionProcessor* modulation : connections) {
      //        if (!modulation->isInputSourcePolyphonic())
      //          modulation->process(num_samples);
      //      }
    }
  }



} // namespace vital
