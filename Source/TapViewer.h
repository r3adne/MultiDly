/*
  ==============================================================================

    TapViewer.h
    Created: 1 Nov 2020 9:41:57pm
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TapViewer  : public juce::Component
{
public:
    TapViewer();
    ~TapViewer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapViewer)
};
