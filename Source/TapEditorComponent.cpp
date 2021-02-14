/*
  ==============================================================================

    TapEditorComponent.cpp
    Created: 1 Nov 2020 1:07:52am
    Author:  Zachary Lewis-Towbes

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TapEditorComponent.h"

//==============================================================================
TapEditorComponent::TapEditorComponent(MultiDlyAudioProcessor& p) : _p(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

TapEditorComponent::~TapEditorComponent()
{
}

void TapEditorComponent::paint (juce::Graphics& g)
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
    g.drawText ("TapEditorComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void TapEditorComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


void TapEditorComponent::mouseDown(const MouseEvent& m)
{
    const GenericScopedLock<SpinLock> sl ((edL));




}

void TapEditorComponent::mouseUp(const MouseEvent& m)
{
    if (! m.mouseWasDraggedSinceMouseDown())
    {
        for (DlyTapComponent a : taps)
        {
            // do hit test, change focus if necessary

            if (a.contains(m.getPosition()))
            {
                focusedTap = &a;
            }
        }
    }

}

void TapEditorComponent::mouseDrag(const MouseEvent& m)
{

}


void TapEditorComponent::addTap(DlyTapComponent t, bool shouldGainFocus, bool shouldAlsoAddToEngine)
{

    if (shouldAlsoAddToEngine && _p.getEngine() != nullptr)
    {
//        _p.getEngine()
    }



    if (shouldGainFocus)
    {

    }

}

void TapEditorComponent::removeTap(DlyTapComponent* pt, bool shouldAlsoRemoveFromEngine)
{
    
}

void TapEditorComponent::setFocusedTap(DlyTapComponent* t)
{

}
