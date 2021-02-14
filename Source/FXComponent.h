/*
  ==============================================================================

    FXComponent.h
    Created: 1 Nov 2020 1:07:21am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class FXComponent;

class CompressorComponent : public juce::Component, public juce::Slider::Listener, public juce::Button::Listener
{
public:
    CompressorComponent(MultiDlyAudioProcessor& p);
    ~CompressorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider*) override;

    void buttonClicked(Button*) override;

private:
    MultiDlyAudioProcessor& _p;

    Slider ratioSlider, thresholdSlider, attackSlider, releaseSlider;
    TextButton fdbkButton, outputButton, inButton, outButton, preButton, postButton;
    Label ratioLabel, thresholdSliderLabel, attackSliderLabel, releaseSliderLabel;

    std::array<Component*, 14> components = {&ratioSlider, &thresholdSlider, &attackSlider, &releaseSlider, &fdbkButton, &outputButton, &inButton, &outButton, &preButton, &postButton, &ratioLabel, &thresholdSliderLabel, &attackSliderLabel, &releaseSliderLabel};


}; // class CompressorComponent

class WaveshaperComponent : public juce::Component, public juce::Slider::Listener, public juce::Button::Listener
{
public:
    WaveshaperComponent(MultiDlyAudioProcessor& p);
    ~WaveshaperComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider*) override;

    void buttonClicked(Button*) override;
    

private:
    MultiDlyAudioProcessor& _p;

    Slider WSAmt, WSGain;
    ComboBox WSType;
    TextButton fdbkButton, outputButton, inButton, outButton, preButton, postButton;
    Label typeLabel, amtLabel, gainLabel;

    std::array<Component*, 12> components = {&WSAmt, &WSGain, &WSType, &fdbkButton, &outputButton, &inButton, &outButton, &preButton, &postButton, &typeLabel, &amtLabel, &gainLabel};


}; // class WaveshaperComponent


class FilterComponent : public juce::Component, public juce::Slider::Listener
{
public:
    FilterComponent(MultiDlyAudioProcessor& p);
    ~FilterComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider*) override;

private:
    MultiDlyAudioProcessor& _p;

    Slider LPFFrequencySlider, HPFFrequencySlider, LPFOrderIncDec, HPFOrderIncDec;
    Label LPFFrequencyLabel, HPFFrequencyLabel, LPFOrderLabel, HPFOrderLabel;

    std::array<Component*, 8> components ={&LPFFrequencySlider, &HPFFrequencySlider, &LPFOrderIncDec, &HPFOrderIncDec, &LPFFrequencyLabel, &HPFFrequencyLabel, &LPFOrderLabel, &HPFOrderLabel};



}; // class FilterComponent



//==============================================================================
/*
*/
class FXComponent  : public juce::Component
{
public:
    FXComponent(MultiDlyAudioProcessor& p);
    ~FXComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MultiDlyAudioProcessor& _p;

    FilterComponent filterComponent;
    WaveshaperComponent waveshaperComponent;
    CompressorComponent compressorComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXComponent)
};
