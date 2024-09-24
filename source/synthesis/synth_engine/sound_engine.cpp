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
    //SoundEngine::init();
    processorGraph->clear();
    processorGraph->enableAllBuses();
  }

  SoundEngine::~SoundEngine() {
    //voice_handler_->prepareDestroy();
  }

//  void SoundEngine::init() {
//
//
//
//
//    //SynthModule::init();
//
//    setOversamplingAmount(kDefaultOversamplingAmount, kDefaultSampleRate);
//  }



  int SoundEngine::getNumPressedNotes() {
    //return voice_handler_->getNumPressedNotes();
  }



  int SoundEngine::getNumActiveVoices() {
    //return voice_handler_->getNumActiveVoices();
  }



  mono_float SoundEngine::getLastActiveNote() const {
    //return voice_handler_->getLastActiveNote();
  }



  void SoundEngine::checkOversampling() {
    //int oversampling = oversampling_->value();
   // int oversampling_amount = 1 << oversampling;
    //int sample_rate = getSampleRate();
//    if (last_oversampling_amount_ != oversampling_amount || last_sample_rate_ != sample_rate)
//      setOversamplingAmount(oversampling_amount, sample_rate);
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

  void SoundEngine::process(int num_samples, AudioSampleBuffer &buffer )
  {
    //VITAL_ASSERT(num_samples <= output()->buffer_size);
    juce::FloatVectorOperations::disableDenormalisedNumberSupport();
    //voice_handler_->setLegato(legato_->value());
    // ProcessorRouter::process(num_samples);
    MidiBuffer midimessages;
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



//  void SoundEngine::correctToTime(double seconds) {
////    voice_handler_->correctToTime(seconds);
////    effect_chain_->correctToTime(seconds);
//  }

  void SoundEngine::allSoundsOff() {
//    voice_handler_->allSoundsOff();
//    effect_chain_->hardReset();
//    decimator_->hardReset();
  }

  void SoundEngine::allNotesOff(int sample) {
//    voice_handler_->allNotesOff(sample);
  }

  void SoundEngine::allNotesOff(int sample, int channel) {
//    voice_handler_->allNotesOff(channel);
  }

  void SoundEngine::allNotesOffRange(int sample, int from_channel, int to_channel) {
//    voice_handler_->allNotesOffRange(sample, from_channel, to_channel);
  }

  void SoundEngine::noteOn(int note, mono_float velocity, int sample, int channel) {
//    voice_handler_->noteOn(note, velocity, sample, channel);
  }

  void SoundEngine::noteOff(int note, mono_float lift, int sample, int channel) {
//    voice_handler_->noteOff(note, lift, sample, channel);
  }

  void SoundEngine::setModWheel(mono_float value, int channel) {
//    voice_handler_->setModWheel(value, channel);
  }

  void SoundEngine::setModWheelAllChannels(mono_float value) {
//    voice_handler_->setModWheelAllChannels(value);
  }
  
  void SoundEngine::setPitchWheel(mono_float value, int channel) {
//    voice_handler_->setPitchWheel(value, channel);
  }

  void SoundEngine::setZonedPitchWheel(mono_float value, int from_channel, int to_channel) {
//    voice_handler_->setZonedPitchWheel(value, from_channel, to_channel);
  }


  void SoundEngine::setAftertouch(mono_float note, mono_float value, int sample, int channel) {
//    voice_handler_->setAftertouch(note, value, sample, channel);
  }

  void SoundEngine::setChannelAftertouch(int channel, mono_float value, int sample) {
//    voice_handler_->setChannelAftertouch(channel, value, sample);
  }

  void SoundEngine::setChannelRangeAftertouch(int from_channel, int to_channel, mono_float value, int sample) {
//    voice_handler_->setChannelRangeAftertouch(from_channel, to_channel, value, sample);
  }

  void SoundEngine::setChannelSlide(int channel, mono_float value, int sample) {
//    voice_handler_->setChannelSlide(channel, value, sample);
  }

  void SoundEngine::setChannelRangeSlide(int from_channel, int to_channel, mono_float value, int sample) {
//    voice_handler_->setChannelRangeSlide(from_channel, to_channel, value, sample);
  }

//  void SoundEngine::setBpm(mono_float bpm) {
//    mono_float bps = bpm / 60.0f;
//    if (bps_->value() != bps)
//      bps_->set(bps);
//  }


//  Sample* SoundEngine::getSample() {
//    return voice_handler_->getSample();
//  }



  void SoundEngine::sustainOn(int channel) {
//    voice_handler_->sustainOn(channel);
  }

  void SoundEngine::sustainOff(int sample, int channel) {
//    voice_handler_->sustainOff(sample, channel);
  }

  void SoundEngine::sostenutoOn(int channel) {
//    voice_handler_->sostenutoOn(channel);
  }

  void SoundEngine::sostenutoOff(int sample, int channel) {
//    voice_handler_->sostenutoOff(sample, channel);
  }

  void SoundEngine::sustainOnRange(int from_channel, int to_channel) {
//    voice_handler_->sustainOnRange(from_channel, to_channel);
  }

  void SoundEngine::sustainOffRange(int sample, int from_channel, int to_channel) {
//    voice_handler_->sustainOffRange(sample, from_channel, to_channel);
  }

  void SoundEngine::sostenutoOnRange(int from_channel, int to_channel) {
//    voice_handler_->sostenutoOnRange(from_channel, to_channel);
  }

  void SoundEngine::sostenutoOffRange(int sample, int from_channel, int to_channel) {
//    voice_handler_->sostenutoOffRange(sample, from_channel, to_channel);
  }
} // namespace vital
