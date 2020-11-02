/*
  ==============================================================================

    MultiDlyTap.cpp
    Created: 16 Aug 2020 10:45:45pm
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#include <cmath>
#include "MultiDlyTap.h"



template <class T, int C>
MultiDlyTap<T, C>::MultiDlyTap(MultiDlyEngine<T, C>& _engine, int _maxWriteIndexOffset) : maxWriteIndexOffset(_maxWriteIndexOffset), engine(_engine)
{
    timeMs = 0.;
    resetSmoothedValue();
}

template<class T, int C>
MultiDlyTap<T, C>::MultiDlyTap(const MultiDlyTap& other) : engine(other.engine), maxWriteIndexOffset(other.getMaxWriteIndexOffset())
{
    time = other.getTimeMs();
    resetSmoothedValue();
}


template<class T, int C>
MultiDlyTap<T, C>::MultiDlyTap(MultiDlyTap&& other) : engine(other.engine)
{
    if (this != &other) // this is a pointer comparison, not a contents comparison
    {
//        this->manager = other.manager;
        this->time = other.time;
        other.time = 0;
    }
    resetSmoothedValue();
}

template<class T, int C>
double MultiDlyTap<T, C>::getSampleRate() const
{
    return sr;
}

template<class T, int C>
void MultiDlyTap<T, C>::init()
{
    timeMs = 0.;
    sr = engine.getSampleRate();
    resetSmoothedValue();

    lpFilter = std::make_shared<dsp::StateVariableTPTFilter<T>>();
    hpFilter = std::make_shared<dsp::StateVariableTPTFilter<T>>();
    comp = std::make_shared<dsp::Compressor<T>>();
    waveshaper = std::make_shared<dsp::WaveShaper<T>>();

    lpFilter->setType(dsp::StateVariableTPTFilterType::lowpass);
    hpFilter->setType(dsp::StateVariableTPTFilterType::highpass);
    lpFilter->prepare({sr, MAX_BLOCK_SIZE, C * 2});
    hpFilter->prepare({sr, MAX_BLOCK_SIZE, C * 2});

    comp->prepare({sr, MAX_BLOCK_SIZE, C * 2});
    waveshaper->prepare({sr, MAX_BLOCK_SIZE, 1}); // waveshaper doesn't need multiple channels



}

template<class T, int C>
MultiDlyTap<T, C>::~MultiDlyTap()
{

}




template<class T, int C>
SmoothedValue<double, ValueSmoothingTypes::Linear>* MultiDlyTap<T, C>::getTimeMsSmoothedValue()
{
    return &timeMs;
}

template<class T, int C>
int MultiDlyTap<T, C>::getWriteIndexOffset()
{
    return (timeMs.getNextValue() * 1000.0f) * sr;
}

template<class T, int C>
double MultiDlyTap<T, C>::getFeedback() const
{
    return feedback;
}

template<class T, int C>
int MultiDlyTap<T, C>::getMaxWriteIndexOffset() const
{
    return maxWriteIndexOffset;
}


template<class T, int C>
void MultiDlyTap<T, C>::setSampleRate(double newSampleRate)
{
    sr = newSampleRate;
    resetSmoothedValue();
}

template<class T, int C>
void MultiDlyTap<T, C>::setFeedback(double newFeedback)
{
    feedback = newFeedback;
}

template<class T, int C>
void MultiDlyTap<T, C>::setTimeMs(double newTimeMs)
{
    timeMs.setTargetValue(newTimeMs);
    timeMsTargetValue = newTimeMs;
}

template<class T, int C>
void MultiDlyTap<T, C>::setTimeSamples(int newTimeSamples)
{
    setTimeMs((newTimeSamples/sr) * 0.001);
}

template<class T, int C>
void MultiDlyTap<T, C>::resetSmoothedValue()
{
    timeMs.reset(sr, engine.smoothingRampLength);
}

template<class T, int C>
double MultiDlyTap<T, C>::getMix() const
{
    return mix;
}

template<class T, int C>
void MultiDlyTap<T, C>::setMix(double newMix)
{
    mix = newMix;
}

template<class T, int C>
void MultiDlyTap<T, C>::setCompIn(bool compIn) { compin.store(compIn); }

template<class T, int C>
void MultiDlyTap<T, C>::setWSIn(bool wsIn) { wsin.store(wsIn); }

template<class T, int C>
void MultiDlyTap<T, C>::setCompFdbk(bool compFdbk) { compfdbk.store(compFdbk); }

template<class T, int C>
void MultiDlyTap<T, C>::setWSFdbk(bool wsFdbk) { wsfdbk.store(wsFdbk); }

template<class T, int C>
void MultiDlyTap<T, C>::setFiltPre(bool filtPre) { filtpre.store(filtPre); }


template<class T, int C>
bool MultiDlyTap<T, C>::getCompIn() { return compin.load(); }

template<class T, int C>
bool MultiDlyTap<T, C>::getWSIn() { return wsin.load(); }

template<class T, int C>
bool MultiDlyTap<T, C>::getCompFdbk() { return compfdbk.load(); }

template<class T, int C>
bool MultiDlyTap<T, C>::getWSFdbk() { return wsfdbk.load(); }

template<class T, int C>
bool MultiDlyTap<T, C>::getFiltPre() { return filtpre.load(); }



template<class T, int C>
void MultiDlyTap<T, C>::setWaveshaperType(MultiDlyTap<T, C>::WaveshaperFunctions newFunctionToUse)
{
    currentWSFunction = newFunctionToUse;

    if (newFunctionToUse == Sine)
    {
        waveshaper.functionToUse = [] (T x) { return std::sin(x); };
    }
    else if (newFunctionToUse == Tanh)
    {
        waveshaper.functionToUse = [] (T x) { return std::tanh(x); };
    }
    else if (newFunctionToUse == Signum)
    {
        waveshaper.functionToUse = [] (T x) { return (x < 0.0f ? 1 : -1); };
    }
}

template<class T, int C>
void MultiDlyTap<T, C>::setWaveshaperPostGain(double newPostGain) { WSPostGain = newPostGain; }

template<class T, int C>
void MultiDlyTap<T, C>::setWaveshaperPreGain(double newPreGain) { WSPreGain = newPreGain; }


template<class T, int C>
double MultiDlyTap<T, C>::getWSPreGain() const { return WSPreGain; }

template<class T, int C>
double MultiDlyTap<T, C>::getWSPostGain() const { return WSPostGain; }

template<class T, int C>
typename MultiDlyTap<T, C>::WaveshaperFunctions MultiDlyTap<T, C>::getWSType() const { return currentWSFunction; }

template<class T, int C>
ValueTree MultiDlyTap<T, C>::toVT()
{
    return ValueTree("MultiDlyTap", {{"hpFilterFreq", hpFilter->getCutoffFrequency()}, {"lpFilterFreq", lpFilter->getCutoffFrequency()}, {"hpFilterRes", hpFilter->getResonance()}, {"lpFilterRes", lpFilter->getResonance()}, {"compRatio", compRatio}, {"compThresh", compThresh}, {"compAtk", compAtk}, {"compRel", compRel}, {"compIn", compin.load()}, {"wsType", currentWSFunction}, {"wsPreGain", WSPreGain}, {"wsPostGain", WSPostGain}, {"wsIn", wsin.load()}, {"compFdbk", compfdbk.load()}, {"wsFdbk", wsfdbk.load()}, {"filtPre", filtpre.load()}, {"mix", mix}, {"feedback", feedback}, {"timeMs", timeMsTargetValue}});
}

template<class T, int C>
void MultiDlyTap<T, C>::fromVT(ValueTree vt, MultiDlyEngine<T, C>& engine)
{

    sr = engine.getSampleRate();
    resetSmoothedValue();
    init();

    fromVTWithoutReset(vt);

}

/**

 */
template<class T, int C>
void MultiDlyTap<T, C>::fromVTWithoutReset(ValueTree vt)
{
    hpFilter->setCutoffFrequency(vt.getProperty("hpFilterFreq"));
    lpFilter->setCutoffFrequency(vt.getProperty("lpFilterFreq"));
    hpFilter->setResonance(vt.getProperty("hpFilterRes"));
    lpFilter->setResonance(vt.getProperty("lpFilterRes"));
    setFiltPre(vt.getProperty("filtPre"));

    setCompRatio(vt.getProperty("compRatio"));
    setCompAtk(vt.getProperty("compAtk"));
    setCompThresh(vt.getProperty("compThresh"));
    setCompRel(vt.getProperty("compRel"));
    setCompIn(vt.getProperty("compIn"));
    setCompFdbk(vt.getProperty("compFdbk"));

    setWSIn(vt.getProperty("wsIn"));
    setWSFdbk(vt.getProperty("wsFdbk"));
    setWaveshaperType(vt.getProperty("wsType"));
    setWaveshaperPreGain(vt.getProperty("wsPreGain"));
    setWaveshaperPostGain(vt.getProperty("wsPostGain"));

    setMix(vt.getProperty("mix"));
    setFeedback(vt.getProperty("feedback"));
    setTimeMs(vt.getProperty("timeMs"));
}

template<class T, int C>
void MultiDlyTap<T, C>::setCompRatio(T newRatio)
{
    compRatio = newRatio;
    comp->setRatio(newRatio);
}

template<class T, int C>
T MultiDlyTap<T, C>::getCompRatio() { return compRatio; }

template<class T, int C>
void MultiDlyTap<T, C>::setCompThresh(T newThresh)
{
    compThresh = newThresh;
    comp->setThresh(newThresh);
}

template<class T, int C>
T MultiDlyTap<T, C>::getCompThresh() { return compThresh; }

template<class T, int C>
void MultiDlyTap<T, C>::setCompAtk(T newAtk)
{
    compAtk = newAtk;
    comp->setAtk(newAtk);
}

template<class T, int C>
T MultiDlyTap<T, C>::getCompAtk() { return compAtk; }

template<class T, int C>
void MultiDlyTap<T, C>::setCompRel(T newRel)
{
    compRel = newRel;
    comp->setRel(newRel);
}

template<class T, int C>
T MultiDlyTap<T, C>::getCompRel() { return compRel; }
