/*
  ==============================================================================

    DlyTapComponent.h
    Created: 1 Nov 2020 1:10:21am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "MultiDlyTap.h"
#include "multiDlyEngine.h"

//==============================================================================
/*
*/
class DlyTapComponent  : public juce::Component
{
public:
    DlyTapComponent();
    DlyTapComponent(DlyTapComponent& other);
    ~DlyTapComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

  int linkedTapIdx;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DlyTapComponent)
};
