/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiDlyAudioProcessor::MultiDlyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )//,
#endif

{
}

MultiDlyAudioProcessor::~MultiDlyAudioProcessor()
{
}

//==============================================================================
const juce::String MultiDlyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultiDlyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultiDlyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultiDlyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultiDlyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultiDlyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultiDlyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultiDlyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MultiDlyAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultiDlyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MultiDlyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    auto ins = getTotalNumInputChannels();
    auto outs = getTotalNumOutputChannels();

    // a change in number of channels has occurred, we need to recreate the engine
    if (EngineChannels.load() != std::max(ins, outs))
    {
        const int a = std::max(ins, outs);
//        std::shared_ptr<MultiDlyEngine<PROCESSING_TYPE, a>> _engine = std::make_shared<MultiDlyEngine<PROCESSING_TYPE, a>>(sampleRate, samplesPerBlock);

        // TODO: Overhaul how engine handles channels



    }
    EngineChannels.store(std::max(ins, outs));


}

void MultiDlyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultiDlyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{

    if (layouts.getNumChannels(true, 0) != layouts.getNumChannels(false, 0)) return false;
    return true;
}
#endif

void MultiDlyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (totalNumInputChannels != totalNumOutputChannels) jassertfalse;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool MultiDlyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultiDlyAudioProcessor::createEditor()
{
    return new MultiDlyAudioProcessorEditor (*this);
}

//==============================================================================
void MultiDlyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultiDlyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultiDlyAudioProcessor();
}
