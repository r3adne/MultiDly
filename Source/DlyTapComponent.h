/*
  ==============================================================================

    DlyTapComponent.h
    Created: 1 Nov 2020 1:10:21am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DlyTapComponent  : public juce::Component
{
public:
    DlyTapComponent();
    ~DlyTapComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DlyTapComponent)
};
