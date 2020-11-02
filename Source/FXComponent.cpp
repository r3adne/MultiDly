/*
  ==============================================================================

    FXComponent.cpp
    Created: 1 Nov 2020 1:07:21am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FXComponent.h"

//==============================================================================
CompressorComponent::CompressorComponent(MultiDlyAudioProcessor& p) : _p(p)
{


    for (auto c : components) { addAndMakeVisible(c); }
}

CompressorComponent::~CompressorComponent()
{

}

void CompressorComponent::paint(juce::Graphics& g)
{

}

void CompressorComponent::resized()
{

}

void CompressorComponent::sliderValueChanged(Slider* s)
{

}

void CompressorComponent::buttonClicked(Button* b)
{

}
//==============================================================================

WaveshaperComponent::WaveshaperComponent(MultiDlyAudioProcessor& p) : _p(p)
{


    for (auto c : components) { addAndMakeVisible(c); }
}

void WaveshaperComponent::paint(juce::Graphics& g)
{

}

void WaveshaperComponent::resized()
{

}

void WaveshaperComponent::sliderValueChanged(Slider* s)
{

}

void WaveshaperComponent::buttonClicked(Button*)
{

}

//==============================================================================
FilterComponent::FilterComponent(MultiDlyAudioProcessor& p) : _p(p)
{

    for (auto c : components) { addAndMakeVisible(c); }
}

FilterComponent::~FilterComponent()
{

}

void FilterComponent::paint(juce::Graphics& g)
{

}

void FilterComponent::resized()
{

}

void FilterComponent::sliderValueChanged(Slider *)
{

}



//==============================================================================
FXComponent::FXComponent(MultiDlyAudioProcessor& p) : _p(p), filterComponent(p), waveshaperComponent(p), compressorComponent(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    

}

FXComponent::~FXComponent()
{
}

void FXComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("FXComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void FXComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
