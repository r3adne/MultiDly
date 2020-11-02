/*
  ==============================================================================

    MultiDlyTap.h
    Created: 16 Aug 2020 10:45:45pm
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#define MAX_BLOCK_SIZE 8192

#include <JuceHeader.h>
//#include "MultiDlyDisplayStateManager.h"

template<class T, int Ch> class MultiDlyEngine; // forward declaration fixes this
template<class T, int C> class MultiDlyDisplayStateManager; // forward declaration fixes this too



/// Represents a single tap for the multi-tap delay.

/**
 Represents a single tap for the multi-tap delay, and owns pointers to the tap's processors (comp, lpfilter, hpfilter, and waveshaper).

 While no processing is done directly by this class, some is done through it by calls to the aforementioned processors, so you should be careful to be threadsafe.

 @tparam T the to use for audio processing

 @tparam C the number of channels of audio used for input/output.
 */
template <class T, int C>
class MultiDlyTap
{
public:

    /// Used to represent the waveshaper's table function.
    enum WaveshaperFunctions
    {
        /// A simple sine shape, increases level for the middle of the range while decreasing level at the edge.
        Sine,
        /// A common tanh shape, which sounds like a light overdrive.
        Tanh,
        /// The sign of the input -- i.e. the output is either -1. or 1. Very harsh distortion.
        Signum
    };


    /// Constructor

    /**
     The constructor that should be normally used. Taps need references to their engines and need to be able to calulate the maximum delay time.

     @param _engine This tap's MultiDlyEngine.
     @param _maxWriteIndexOffset The highest offset between the write pointer and the read pointer. Used to calculate maximum time and sanitize time input values if necessary.
     */
    MultiDlyTap(MultiDlyEngine<T, C>& _engine, int _maxWriteIndexOffset);


    /// Copy constructor

    /**

     @param other The MultiDlyTap to copy from
     */
    MultiDlyTap(const MultiDlyTap<T, C>& other);

    /// Move constructor

    /**

     @param other the MultiDlyTap to move from
     */
    MultiDlyTap(MultiDlyTap<T, C>&& other);

    /// Destructor
    ~MultiDlyTap();

    /// Initializes the tap to default values.
    /**

     Called by all constructors, but can be used externally to return the state to default.

     Note: this specifies that the two filters and compressor should be prepared to handle <em> C * 2 </em> channels, because the feedback signal might need to be processed separately from the regular signal.
     */
    void init();


    /**
     Sets the time in milliseconds. This uses the internal SmoothedValue timeMs, which ensures that changes are ramped and don't cause audio glitches.

     @param newTimeMs The new delay length for the tap, in milliseconds.
     */
    void setTimeMs(double newTimeMs);

    /**
     Sets the time in samples, relative to the tap's sampling rate sr. This calls setTimeMs() because the smoothed value for time is stored in milliseconds.

     @param newTimeSamples The new delay length for the tap, in samples relative to sr.
     */
    void setTimeSamples(int newTimeSamples);


    /**
     Gets the number of samples behind the write pointer to read a sample. This value represents the time, and calls the timeMs SmoothedValue.
     */
    int getWriteIndexOffset();


    /**
     Gets a pointer to the SmoothedValue for the MultiDlyTap's time which is likely never necessary.
     */
    SmoothedValue<double, ValueSmoothingTypes::Linear>* getTimeMsSmoothedValue();


    /**
     Gets the highest write index offset value, which is a consequence of the longest possible delay (and the length of the delay buffer owned by the engine.
     */
    int getMaxWriteIndexOffset() const;



    /**
     Sets the feedback for the tap.

     @param newFeedback The new feedback for the tap.
     */
    void setFeedback(double newFeedback);

    /**
     Gets the feedback for the tap.
     */
    double getFeedback() const;



    /**
     Sets the mix for the tap, where 0.0 is no delay, and 1.0 is only delay.

     In the engine, the delayed signal should be multiplied by mix, whereas the dry signal should be multiplied by <em> (1.0 - mix) </em>.

     @param newMix The new mix value for the tap.
     */
    void setMix(double newMix);

    /**
     Gets the mix for the tap.
     */
    double getMix() const;


    /**
     Sets the sample rate of the processor. <b> If you don't call MultiDlyTap<T, C>::init() after this, behavior is undefined. Behavior is also undefined if the sample rate of the processor differs from the sample rate of its engine. </b>

     @param newSampleRate The new sample rate for the tap.
     */
    void setSampleRate(double newSampleRate);


    /**
     @brief Gets and returns the current sampling rate.
     */
    double getSampleRate() const;



    /**
     @brief Creates and returns a `ValueTree` representing the state of the tap.

     This can be used for saving, copying, etc. It pulls values from all necessary parameters atomically. The format of the output `ValueTree` is specified in this function's definition, but you <b> should never manually edit the `ValueTree` </b>

     @return The `ValueTree` which represents the current tap.
     */
    ValueTree toVT();


    /**
     @brief Reconstructs a tap from a ValueTree created by toVT().

     No `ValueTree` manually created or edited are guarenteed to work. This function only has well-defined behavior for a `ValueTree` created by toVT().

     This calls fromVTWithoutReset(), which keeps things like the engine and sample rate the same, while resetting other parameters.

     @param vt The `ValueTree`, created by toVT(), to recreate the tap from.
     @param engine The engine that this tap belongs to.
     */
    void fromVT(ValueTree vt, MultiDlyEngine<T, C>& engine);

    /**
     @brief Reconstructs a tap from a `ValueTree` created by toVT(), but doesn't use a new engine, sampling rate, etc.

     As with fromVT(), no `ValueTree` manually created or edited are guarenteed to work. This function only has well-defined behavior for a `ValueTree` created by toVT().

     @param vt The `ValueTree`, created by toVT(), to recreate the tap from.
     */
    void fromVTWithoutReset(ValueTree vt);


    /**
     @brief Sets whether the waveshaper is enabled.

     @param wsIn The new value for whether the waveshaper should be enabled.
     */
    void setWSIn(bool wsIn);

    /**
     @brief Gets a bool representing whether the waveshaper is enabled.
     */
    bool getWSIn();



    /**
     @brief Sets whether the waveshaper should be applied to the feedback of the delay.

     @param wsFdbk The new value for whether the waveshaper should be enabled in the feedback loop.
     */
    void setWSFdbk(bool wsFdbk);

    /**
     @brief Gets whether the waveshaper should be applied to the feedback of the delay.
     */
    bool getWSFdbk();


    /**
     @brief Sets the function used by the waveshaper using one of the WaveshaperFunctions.

     @param functionToUse The function to use for waveshaping.
     */
    void setWaveshaperType(WaveshaperFunctions functionToUse);

    /// @brief Gets the current waveshaping type.
    WaveshaperFunctions getWSType() const;


    /**
     @brief Sets the gain applied to the signal after waveshaping.

     @param newPostGain The new post-processing gain value in linear value.
     */
    void setWaveshaperPostGain(double newPostGain);

    /// @brief Gets the post-processing gain value, in linear units.
    double getWSPostGain() const;



    /**
     @brief Sets the gain applied to the signal before waveshaping.

     @param newPreGain The new pre-processing gain value in linear value.
     */
    void setWaveshaperPreGain(double newPreGain);

    /// @brief Gets the pre-processing gain value, in linear units.
    double getWSPreGain() const;



    /**
     @brief Sets whether the compressor should be applied to the feedback of the delay.

     @param compFdbk The new value for the compressor's feedback.
     */
    void setCompFdbk(bool compFdbk);

    /// @brief Gets whether the compressor should be applied to the feedback of the delay.
    bool getCompFdbk();




    /**
     @brief Sets whether the compressor should be enabled.

     This is type T because the underlying compressor uses the same type for audio processing and parameter setting.

     @param compIn The new value for whether the compressor should be enabled.
     */
    void setCompIn(bool compIn);

    /// @brief Gets whether the compressor is enabled.
    bool getCompIn();


    /**
     @brief Sets the compressor's ratio.

     This is type T because the underlying compressor uses the same type for audio processing and parameter setting.

     @param newRatio The new ratio for the compressor. Must be >= 1.0
     */
    void setCompRatio(T newRatio);

    /// @brief Gets the current ratio of the compressor.
    T getCompRatio();


    /**
     @brief Sets the compressor's threshold in dB.

     This is type T because the underlying compressor uses the same type for audio processing and parameter setting.

     @param newThresh The new threshold for the compressor, in dB.
     */
    void setCompThresh(T newThresh);

    /// @brief Gets the current threshold of the compressor.
    T getCompThresh();



    /**
     @brief Sets the compressor's attack in milliseconds.

     This is type T because the underlying compressor uses the same type for audio processing and parameter setting.

     @param newAtk The new attack time for the compressor, in milliseconds.
     */
    void setCompAtk(T newAtk);

    ///@brief Gets the compressor's current attack, in milliseconds.
    T getCompAtk();

    /**
     @brief Sets the compressor's release in milliseconds.

     This is type T because the underlying compressor uses the same type for audio processing and parameter setting.

     @param newRel The new release time for the compressor, in milliseconds.
     */
    void setCompRel(T newRel);

    ///@brief Gets the compressor's current release, in milliseconds.
    T getCompRel();



     /// @brief Gets whether the filter processing happens before or after waveshaping and compression.
    bool getFiltPre();


    /**
     @brief Sets whether the filter processing happens before or after waveshaping and compression.

     This value should be queried once per signal pass (either sample or block), because otherwise one risks a change to this value occurring between the pre-filter and the post-filter. A correctly implemented MultiDlyEngine must achieve this.

     @param filtPre The new value for when filter processing should happen
     */
    void setFiltPre(bool filtPre);

    /**
     @brief Gets the target value of the time SmoothedValue.
     */
    double getTimeMsTargetValue()
    {
        return timeMsTargetValue();
    }

    /**
     As the engine needs to maintain a sorted list of taps in order to achieve proper sub-block feedback values, the MultiDlyTap must provide a call operator which compares the delay time of the two taps.

     @param a The first tap
     @param b The second tap
     */
    bool operator ()(MultiDlyTap<T, C> a, MultiDlyTap<T, C> b)
    {
        return (a.getTimeMsTargetValue() < b.getTimeMsTargetValue());
    }

private:

    void resetSmoothedValue();
    WaveshaperFunctions currentWSFunction;
    double WSPreGain, WSPostGain;

    T compRatio, compThresh, compAtk, compRel;

    double sr, feedback, mix, timeMsTargetValue;

    SmoothedValue<double, ValueSmoothingTypes::Linear> timeMs;

    const int maxWriteIndexOffset;

    std::atomic<bool> compin, wsin, compfdbk, wsfdbk, filtpre;

    std::shared_ptr<juce::dsp::Compressor<T>> comp;
    std::shared_ptr<juce::dsp::StateVariableTPTFilter<T>> lpFilter;
    std::shared_ptr<juce::dsp::StateVariableTPTFilter<T>> hpFilter;
    std::shared_ptr<juce::dsp::WaveShaper<T>> waveshaper; // can be shared by channels becasue waveshaping is memoryless.

//    MultiDlyDisplayStateManager& manager; // is this necessary?

    MultiDlyEngine<T, C>& engine; // the engine owns the input samples so it needs a ref here. -- wait it might not.


};
