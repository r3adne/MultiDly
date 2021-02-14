/*
  ==============================================================================

    TapEditorComponent.h
    Created: 1 Nov 2020 1:07:52am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DlyTapComponent.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class TapEditorComponent  : public juce::Component
{
public:
    TapEditorComponent(MultiDlyAudioProcessor& p);
    ~TapEditorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const MouseEvent& m) override;
    void mouseUp(const MouseEvent& m) override;
    void mouseDrag(const MouseEvent& m) override;

    void addTap(DlyTapComponent t, bool shouldGainFocus, bool shouldAlsoAddToEngine=false);
    void removeTap(DlyTapComponent* pt, bool shouldAlsoRemoveFromEngine=false);
    void setFocusedTap(DlyTapComponent* t);

private:
    std::vector<DlyTapComponent> taps;
    DlyTapComponent* focusedTap = nullptr;

    MultiDlyAudioProcessor& _p;

    SpinLock edL;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapEditorComponent)
};
