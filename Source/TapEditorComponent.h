/*
  ==============================================================================

    TapEditorComponent.h
    Created: 1 Nov 2020 1:07:52am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TapEditorComponent  : public juce::Component
{
public:
    TapEditorComponent();
    ~TapEditorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapEditorComponent)
};
