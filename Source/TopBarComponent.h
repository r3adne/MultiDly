/*
  ==============================================================================

    TopBarComponent.h
    Created: 1 Nov 2020 1:07:03am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TopBarComponent  : public juce::Component
{
public:
    TopBarComponent();
    ~TopBarComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopBarComponent)
};
