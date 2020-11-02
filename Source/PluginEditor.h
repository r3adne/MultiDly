/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MultiDlyDisplay.h"
#include "TopBarComponent.h"
#include "FXComponent.h"
#include "TapViewer.h"

//==============================================================================
/**
*/
class MultiDlyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MultiDlyAudioProcessorEditor (MultiDlyAudioProcessor&);
    ~MultiDlyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MultiDlyAudioProcessor& audioProcessor;

    MultiDlyDisplay display;
    TopBarComponent topBar;
    FXComponent FX;
    TapViewer tapViewer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiDlyAudioProcessorEditor)
};
