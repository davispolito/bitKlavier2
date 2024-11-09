//
// Created by Dan Trueman on 11/7/24.
//

#ifndef BITKLAVIER2_BKADSR_H
#define BITKLAVIER2_BKADSR_H

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_audio_formats/juce_audio_formats.h>

//==============================================================================
/**
Based on the JUCE ADSR Class, with curvature additions

 performance should be the same as JUCE::ADSR for curvature values of 0
 otherwise, will have some performance impact

**/

class BKADSR
{
public:
    //==============================================================================
    BKADSR()
    {
        recalculateRates();
        lastOut.reset(sampleRate, 0.005);
        k_curvature.reset(sampleRate, 0.005);
    }

    //==============================================================================
    /**
        Holds the parameters being used by an ADSR object.

        @tags{Audio}
    */
    struct Parameters
    {
        Parameters() = default;

        Parameters (float attackTimeSeconds,
            float decayTimeSeconds,
            float sustainLevel,
            float releaseTimeSeconds)
            : attack (attackTimeSeconds),
              decay (decayTimeSeconds),
              sustain (sustainLevel),
              release (releaseTimeSeconds)
        {
        }

        Parameters (float attackTimeSeconds,
            float decayTimeSeconds,
            float sustainLevel,
            float releaseTimeSeconds,
            float attackPower,
            float decayPower,
            float releasePower)
            : attack (attackTimeSeconds),
              decay (decayTimeSeconds),
              sustain (sustainLevel),
              release (releaseTimeSeconds),
              attackPower (attackPower),
              decayPower (decayPower),
              releasePower (releasePower)
        {
        }

        float attack = 0.1f, decay = 0.1f, sustain = 1.0f, release = 0.1f;
        float attackPower = 0.0f, decayPower = 0.0f, releasePower = 0.0f;

    };

    /** Sets the parameters that will be used by an ADSR object.

        You must have called setSampleRate() with the correct sample rate before
        this otherwise the values may be incorrect!

        @see getParameters
    */
    void setParameters (const Parameters& newParameters)
    {
        // need to call setSampleRate() first!
        jassert (sampleRate > 0.0);

        parameters = newParameters;
        recalculateRates();
    }

    /** Returns the parameters currently being used by an ADSR object.

        @see setParameters
    */
    const Parameters& getParameters() const noexcept  { return parameters; }

    /** Returns true if the envelope is in its attack, decay, sustain or release stage. */
    bool isActive() const noexcept                    { return state != State::idle; }

    //==============================================================================
    /** Sets the sample rate that will be used for the envelope.

        This must be called before the getNextSample() or setParameters() methods.
    */
    void setSampleRate (double newSampleRate) noexcept
    {
        jassert (newSampleRate > 0.0);
        sampleRate = newSampleRate;
    }

    //==============================================================================
    /** Resets the envelope to an idle state. */
    void reset() noexcept
    {
        DBG("reset");
        envelopeVal = 0.0f;
        lastOut = 0.0f;
        //lastOut.setCurrentAndTargetValue(0.0f);
        //lastOut.setTargetValue(0.0f);
        state = State::idle;
    }

    /** Starts the attack phase of the envelope. */
    void noteOn() noexcept
    {
        if (attackRate > 0.0f)
        {
            state = State::attack;
        }
        else if (decayRate > 0.0f)
        {
            envelopeVal = 1.0f;
            state = State::decay;
        }
        else
        {
            envelopeVal = parameters.sustain;
            state = State::sustain;
        }
    }

    /** Starts the release phase of the envelope. */
    void noteOff() noexcept
    {
        if (state != State::idle)
        {
            if (parameters.release > 0.0f)
            {
                releaseRate = (float) (envelopeVal / (parameters.release * sampleRate));
                state = State::release;
            }
            else
            {
                reset();
            }
        }
    }

    //==============================================================================
    /** Returns the next sample value for an ADSR object.

        @see applyEnvelopeToBuffer
    */
    float getNextSample() noexcept
    {
        switch (state)
        {
            case State::idle:
            {
                return 0.0f;
            }

            case State::attack:
            {
                envelopeVal += attackRate;
                lastOut.setTargetValue(powerScale(envelopeVal, parameters.attackPower));
                float returnSample = lastOut.getNextValue();

                if (envelopeVal >= 1.0f)
                {
                    envelopeVal = 1.0f;
                    goToNextState();
                }

                return returnSample;
            }

            case State::decay:
            {
                envelopeVal -= decayRate;
                lastOut.setTargetValue(powerScale(envelopeVal, parameters.decayPower));
                float returnSample = lastOut.getNextValue();

                if (envelopeVal <= parameters.sustain)
                {
                    envelopeVal = parameters.sustain;
                    goToNextState();
                }

                return returnSample;
            }

            case State::sustain:
            {
                envelopeVal = parameters.sustain;
                lastOut.setTargetValue(envelopeVal);
                return lastOut.getNextValue();
            }

            case State::release:
            {
                envelopeVal -= releaseRate;
                lastOut.setTargetValue(powerScale(envelopeVal, parameters.releasePower));
                float returnSample = lastOut.getNextValue();

                if (envelopeVal <= 0.0f)
                {
                    goToNextState();
                }

                return returnSample;
            }
        }

        lastOut.setTargetValue(0.0f);
        return lastOut.getNextValue();
    }

    /** This method will conveniently apply the next numSamples number of envelope values
        to an AudioBuffer.

        @see getNextSample
    */
    template <typename FloatType>
    void applyEnvelopeToBuffer (juce::AudioBuffer<FloatType>& buffer, int startSample, int numSamples)
    {
        jassert (startSample + numSamples <= buffer.getNumSamples());

        if (state == State::idle)
        {
            buffer.clear (startSample, numSamples);
            return;
        }

        if (state == State::sustain)
        {
            buffer.applyGain (startSample, numSamples, parameters.sustain);
            return;
        }

        auto numChannels = buffer.getNumChannels();

        while (--numSamples >= 0)
        {
            auto env = getNextSample();

            for (int i = 0; i < numChannels; ++i)
                buffer.getWritePointer (i)[startSample] *= env;

            ++startSample;
        }
    }

private:
    //==============================================================================
    void recalculateRates() noexcept
    {
        DBG("recalculateRates");
        auto getRate = [] (float distance, float timeInSeconds, double sr)
        {
            return timeInSeconds > 0.0f ? (float) (distance / (timeInSeconds * sr)) : -1.0f;
        };

        attackRate  = getRate (1.0f, parameters.attack, sampleRate);
        decayRate   = getRate (1.0f - parameters.sustain, parameters.decay, sampleRate);
        releaseRate = getRate (parameters.sustain, parameters.release, sampleRate);

        if (   (state == State::attack && attackRate <= 0.0f)
            || (state == State::decay && (decayRate <= 0.0f || envelopeVal <= parameters.sustain))
            || (state == State::release && releaseRate <= 0.0f))
        {
            goToNextState();
        }
    }

    void goToNextState() noexcept
    {
        if (state == State::attack)
        {
            state = (decayRate > 0.0f ? State::decay : State::sustain);
            return;
        }

        if (state == State::decay)
        {
            state = State::sustain;
            return;
        }

        if (state == State::release)
        {
            reset();
        }
    }

    /**
     * powerScale, from Vital/Tytel
     * for creating a curved table of values between 0 and 1
     *
     * @param value = [0, 1] linear input
     * @param power = exponent coefficient [0 => linear, generally between +/- 2-10 for useful curves]
     * @return output = [0, 1] curved output
     *
     * could replace with lookup table to see if that is better for performance
     *
     * k_curvature (power) is smoothed to afford click-less transitions from one curvature to another
     */
    force_inline float powerScale(float value, float power) {

        k_curvature.setTargetValue(power);
        float temp_power = k_curvature.getNextValue();

        if (fabsf(temp_power) <= kMinPower) return value;

        float numerator = exp(temp_power * value) - 1.0f;
        float denominator = exp(temp_power) - 1.0f;
        return numerator / denominator;
    }

    //==============================================================================
    enum class State { idle, attack, decay, sustain, release };

    State state = State::idle;
    Parameters parameters;

    double sampleRate = 44100.0;
    juce::SmoothedValue<float> lastOut = 0.0f;
    float envelopeVal = 0.0f;
    float attackRate = 0.0f, decayRate = 0.0f, releaseRate = 0.0f;

    static constexpr float kMinPower = 0.01f;
    std::vector<float> attackCurve, decayCurve, releaseCurve;
    juce::SmoothedValue<float> k_curvature = 0.0f;
};

#endif //BITKLAVIER2_BKADSR_H