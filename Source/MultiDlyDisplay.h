/*
  ==============================================================================

    MultiDlyDisplay.h
    Created: 15 Aug 2020 1:19:30pm
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MultiDlyTap.h"

//==============================================================================
/*
*/
class MultiDlyDisplay  : public Component
{
public:
    MultiDlyDisplay(MultiDlyAudioProcessor& _p);
    ~MultiDlyDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown (const MouseEvent& event) override;
    void mouseDrag (const MouseEvent& event) override;
    void mouseUp (const MouseEvent& event) override;


private:
    MultiDlyAudioProcessor& p;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiDlyDisplay)
};
