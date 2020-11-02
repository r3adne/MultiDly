/*
  ==============================================================================

    multiDlyEngine.cpp
    Created: 1 Sep 2020 1:33:07am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/


#include "multiDlyEngine.h"

template<class T, int Ch>
MultiDlyEngine<T, Ch>::MultiDlyEngine(double sampleRate, int blockSize) : numChannels(Ch)
{
    assert(MAX_DELAY_TIME_SECONDS * 48000 <= pow(2.0f, sizeof(unsigned int) * 8)); // we need to make sure the indexes of the delay buffer are within int range

    data.setSize(Ch, DELAY_BUFFER_LENGTH);


}

template<class T, int Ch>
MultiDlyEngine<T, Ch>::~MultiDlyEngine()
{

}


template<class T, int Ch>
void MultiDlyEngine<T, Ch>::prepareToPlay(double sr, int block_size)
{

}


template<class T, int Ch>
void MultiDlyEngine<T, Ch>::processSamples(AudioBuffer<T>& samples)
{
    writeIncomingAudio(samples); // although the write happens here, the write index is incremented later.



    for (int samp = 0; samp < samples.getNumSamples(); ++samp)
    {
        for (int chan = 0; chan < samples.getNumChannels(); ++chan)
        {
            for (std::shared_ptr<MultiDlyTap<T,numChannels>> a : taps)
            {
                if (a == nullptr) continue; // weed out nullptr taps if applicable
                int tapSamps = (a->getTimeMsSmoothedValue()->getNextValue() * 1000) * sr; // gets the tap time in samples, incrementing the smoothing on the smoothvalue

                int readidx = (writeidx - tapSamps); // gets the read index
                if (readidx < 0) readidx = DELAY_BUFFER_LENGTH + readidx; // wraps the read index if necessary

                T outval = data.getSample(chan, readidx); // gets initial read value


                // called here because the filter could be switched to output by another thread during processing, however unlikely.
                const bool filtpre = a->getFiltPre();

                // FILTER //
                if (filtpre)
                {
                    outval = a->lpFilter.processSample(0, outval);
                    outval = a->hpFilter.processSample(0, outval);
                }

                T fdbkval = std::copy(outval);

                // WAVESHAPING //
                if (a->getWSIn())
                {
                    outval = a->pwaveshaper.load()->processSample(outval * a->getWSPreGain()) * a->getWSPostGain(); // runs the waveshaper on the outval

                    // conditionally run the feedback value through the waveshaper
                    if (a->getWSFdbk()) { fdbkval = a->pwaveshaper.load()->processSample(fdbkval * a->getWSPreGain()) * a->getWSPostGain(); }
                }

                // COMPRESSION //
                if (a->getCompIn())
                {
                    outval = a->comp.processSample(chan, outval); // runs the compressor on the outval

                    // process feedback data on chan 1 so that it doesn't interfere.
                    if (a->getCompFdbk()) { fdbkval = a->comp.processSample(chan*2, outval); }

                }

                // FILTER (if filter is in post)
                if (! filtpre)
                {
                    outval = a->lpFilter[chan].processSample(chan, outval);
                    outval = a->hpFilter[chan].processSample(chan, outval);
                    fdbkval = a->lpFilter[chan].processSample(chan * 2, fdbkval);
                    fdbkval = a->hpFilter[chan].processSample(chan * 2, fdbkval);
                }


                data.addSample(chan, writeidx, fdbkval * a->getFeedback()); // adds feedback value to circular buffer

                samples.addSample(chan, samp, (outval * a->getMix()) + (samples.getSample(chan, readidx) * (1.0-a->getMix()))); // just does the mix math

                // does denomral things
                a->lpfilter.snapToZero();
                a->hpfilter.snapToZero();
            }


        }
        ++writeidx;
    }

    writeidx += samples.getNumSamples(); // add through write index.
}


// assumes that the incomingAudio.getNumSamples() < data.getNumSamples()
template<class T, int Ch>
void MultiDlyEngine<T, Ch>::writeIncomingAudio(juce::AudioBuffer<float>& incomingAudio)
{
    if (incomingAudio.getNumSamples() <= 0) return; // do nothing if incoming buffer is empty
    assert(incomingAudio.getNumChannels() == data.getNumChannels()); // ensure number of channels is equal

    a = (writeidx + incomingAudio.getNumSamples()) % data.getNumSamples(); // first batch of samples
    b = incomingAudio.getNumSamples() - a; // second batch of samples

    for (int chan = 0; chan < incomingAudio.getNumChannels(); ++chan) // iterates through channels
    {
        // copies the first a samples, allowing us to copy b to
        data.addFrom(chan, writeidx, incomingAudio.getReadPointer(chan), a);

        // when there are are extra overlapping samples, this puts them in the right place.
        if (b != 0) data.addFrom(chan, 0, incomingAudio.getReadPointer(chan)+a, b);
    }
}


template<class T, int Ch>
bool MultiDlyEngine<T, Ch>::addDelayTap(std::shared_ptr<MultiDlyTap<T, Ch>> tapToAdd, unsigned int delayBufferSize)
{
    // checks to ensure that the tap will fit within the array
    if (num_taps == MAX_NUM_DLY_TAPS) return false;

    if (delayBufferSize == 0) delayBufferSize = DELAY_BUFFER_LENGTH;

    // add tap to taps -- is this the correct operation? does it add to the shared_ptr ref count?
    taps[num_taps] = tapToAdd;

    // tap has been added
    ++num_taps;

    // sorts taps based on their () operator, which gets time.
    std::sort(taps.begin(), taps.end(), MultiDlyTap<T, Ch>());
}

template<class T, int Ch>
std::shared_ptr<MultiDlyTap<T, Ch>> MultiDlyEngine<T, Ch>::createAndAddDelayTap(ValueTree delayTapParametersVT, unsigned int delayBufferSize)
{
    if (num_taps == MAX_NUM_DLY_TAPS) return std::make_shared<MultiDlyTap<T, Ch>>(nullptr); // returns a shared nullptr. comparison between shared_ptr<T> and nullptr, still supported in c++20 although other comparison operators are now removed


    if (delayBufferSize == 0) delayBufferSize = DELAY_BUFFER_LENGTH;

    std::shared_ptr<MultiDlyTap<T, Ch>> a = std::make_shared<MultiDlyTap<T, Ch>>(*this, delayBufferSize);
    a->fromVTWithoutReset(delayTapParametersVT);

    if (a != nullptr)
    {
        taps[num_taps] = a;
        ++num_taps;
    }

    // sorts taps based on their () operator, which gets time.
    std::sort(taps.begin(), taps.end(), MultiDlyTap<T, Ch>());

    return a; // returns even if a is nullptr
}

template<class T, int Ch>
void MultiDlyEngine<T, Ch>::setBlockSize(int newBlockSize)
{
    blocksize = newBlockSize;
}

template<class T, int Ch>
void MultiDlyEngine<T, Ch>::setSampleRate(double newSampleRate)
{
    sr = newSampleRate;
    for (std::shared_ptr<MultiDlyTap<T,numChannels>> a : taps)
    {
        a->init();
    }
}

template<class T, int Ch>
double MultiDlyEngine<T, Ch>::getSampleRate() const
{
    return sr;
}
