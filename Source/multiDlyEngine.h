/*
  ==============================================================================

    multiDlyEngine.h
    Created: 1 Sep 2020 1:33:07am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#define MAX_NUM_DLY_TAPS 32
#define MAX_DELAY_TIME_SECONDS 20
//#define INTERNAL_BLOCK_SIZE 32 // not needed currently, but might be if internal sub-block processing is necessary (to account for the smoothed value
#define DELAY_BUFFER_LENGTH (MAX_DELAY_TIME_SECONDS * 48000)


#include "MultiDlyTap.h"
#include <JuceHeader.h>



class EngineBase {};

/**
 @brief The engine that runs delay processing, owns MultiDlyTaps, and represents the entire audio backend of the plugin.

 @tparam T The type to perform audio processing with
 @tparam Ch The number of channels to perform audio processing on (both input and output channels)
 */
template <class T, int Ch>
class MultiDlyEngine : public EngineBase
{
    // stores shared ptrs to the taps, as they will also be owned by the display managerclass.
    std::array<std::shared_ptr<MultiDlyTap<T, Ch>>, MAX_NUM_DLY_TAPS> taps;
    unsigned int num_taps = 0; // used to check if the max has been reached

    double sr;
    int blocksize;
    int a, b; // used for copying samples

    const int numChannels;



//    std::array<T, MAX_DELAY_TIME_SECONDS * 48000> data;

    AudioBuffer<T> data;

    unsigned int writeidx = 0; // the index of data that incoming audio is written to

public:



    /**
     @brief The only valid constructor for MultiDlyEngine.
     @param sampleRate The sampling rate for the engine in Hz.
     @param blockSize The number of audio samples to expect per block.
     */
    MultiDlyEngine(double sampleRate, int blockSize);

    /**
     @brief Destructor.
     */
    ~MultiDlyEngine();

    double smoothingRampLength = 0.1;


    // unnecessary
    MultiDlyEngine(MultiDlyEngine& other)=delete;
    MultiDlyEngine(MultiDlyEngine&& other)=delete;


    /**
     @brief Called before the first playback

     Called before playback in order to set Sampling Rate and Block Size. This is not necessarily called before each playback, but is always called before the first callback to processSamples().

     @param sr The sampling rate to play back at, in Hz.
     @param block_size The expected block size, in samples.
     */
    void prepareToPlay(double sr, int block_size);

    /**
     @brief Main callback for processing samples

     Replaces input signals with output signals.
     @param samples The buffer to process inputs from/fill with correct output samples.
     */
    void processSamples(AudioBuffer<T>& samples);

    /**
     Attempts to add a MultiDlyTap to the engine, returning true if the tap is successfully added and false if it is unsuccessful.

     @param tapToAdd The shared_ptr to the tap that should be added. It is not necessary for the owner to continue to own their copy of tapToAdd after this callback completes.
     @param delayBufferSize The size, in samples, of the delay buffer. This is currently defined internally to the engine, so callers may leave it as zero so that the engine can fill in the correct value.
     */
    bool addDelayTap(std::shared_ptr<MultiDlyTap<T, Ch>> tapToAdd, unsigned int delayBufferSize = 0);

    // creates a MultiDlyTap from a ValueTree of the tap's parameters, returning the tap if and only if the tap is successfully created and its parameters filled, and the tap is successfully added to the engine through a call to addDelayTap(). If either of these steps is unsuccessful, this function should return nullptr.
    std::shared_ptr<MultiDlyTap<T, Ch>> createAndAddDelayTap(ValueTree delayTapParametersVT, unsigned int delayBufferSize = 0);


    /**
     @brief Sets the sample rate for the engine and all its taps.

     @param newSampleRate The new sampling rate in Hz.
     */
    void setSampleRate(double newSampleRate);

    /**
     @brief Sets the block size for processing.

     At this point the taps don't actually care about the block size, but at some point, sub-block processing should be implemented here so that the effects in each tap don't have to process single samples. This is probably more efficient and certainly allows for more chance of SIMD optimization, from JUCE, the compiler, or us.

     @param newBlockSize The new block size for processing.
     */
    void setBlockSize(int newBlockSize);


    /**
     @brief Returns the current sample rate.
     */
    double getSampleRate() const;



    //! writes new audio from host to circular buffer

    /**
     @brief Writes new audio data to the circular buffer for delay processing.

     This is currently public, but it should probably only be used internally. Until I know for sure there is no good reason to do this from the outside, I'll leave it public.

     This function doesn't actually iterate the write index, because offsets for delay length need to be calculated from the write index's location relative to the current sample, not the sample `data.length()` ahead. The write index is iterated by processSamples().

     @param data The data to add to the delay buffer.
     */
    void writeIncomingAudio(juce::AudioBuffer<float>& data);


    /**
     @brief removes a tap from the sorted array.
     */
    void removeTap(int index);


    /**
     @brief Gets a shared_ptr to the tap at index.
     @param index The index to fetch a tap from.
     */
    std::shared_ptr<MultiDlyTap<T, Ch>> getTap(int index);

};
