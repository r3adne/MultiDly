/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiDlyAudioProcessorEditor::MultiDlyAudioProcessorEditor (MultiDlyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), display(p), FX(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    setSize (400, 300);

//    resiz
//    addChildAndSetID(&display, "displayComponent");

    addAndMakeVisible(display);
    addAndMakeVisible(topBar);
    addAndMakeVisible(tapViewer);
    addAndMakeVisible(FX);
}

MultiDlyAudioProcessorEditor::~MultiDlyAudioProcessorEditor()
{
}

//==============================================================================
void MultiDlyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MultiDlyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    Rectangle<int> local = getLocalBounds();
    topBar.setBounds(local.removeFromTop(40));
    Rectangle<int> bottom = local.removeFromBottom(120);
    display.setBounds(local);
    tapViewer.setBounds(bottom.removeFromLeft(proportionOfWidth(0.3333f)));
    FX.setBounds(bottom);
//    display.setBounds();

}
