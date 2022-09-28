/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThermostatAudioProcessor::ThermostatAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),apvts(*this, nullptr, "Params", createParams()),
eq(apvts),
bw(apvts)
#endif
{
}

ThermostatAudioProcessor::~ThermostatAudioProcessor()
{
}

//==============================================================================
const juce::String ThermostatAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ThermostatAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ThermostatAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ThermostatAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ThermostatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ThermostatAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ThermostatAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ThermostatAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ThermostatAudioProcessor::getProgramName (int index)
{
    return {};
}

void ThermostatAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ThermostatAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = (juce::uint32) juce::jmax(getTotalNumInputChannels(),getTotalNumOutputChannels());

    eq.prepare(spec);
    bw.prepare(spec);
    
}

void ThermostatAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThermostatAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ThermostatAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);


    dsp::AudioBlock<float> block(buffer);
    
//    eq.process(block,numSamples);
    bw.process(block);
    
    spectrum.process(block); // not the buzzing problem
    
}

//==============================================================================
bool ThermostatAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ThermostatAudioProcessor::createEditor()
{
    return new ThermostatAudioProcessorEditor (*this);
}

//==============================================================================
void ThermostatAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ThermostatAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThermostatAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout ThermostatAudioProcessor::createParams(){
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    juce::StringArray filterDesignOptions = juce::StringArray("Design","Butterworth", "Bessel", "Linkwitz-Reilly");
    juce::StringArray filterOrderOptions = juce::StringArray("Order","1st", "2nd", "3rd", "4th");
    juce::StringArray filterTypeOptions = juce::StringArray("Type","LPF", "PKF", "HPF");
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ1","Center Frequency 1",20.f,20000.f,1000.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Q1","Q 1",.1f,20.f,1.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMP1","Amplitude 1",-40.f,12.f,0.f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ANALOG_Q_ON1", "Analog Q On 1", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("NONLINEAR_EQ_ON1", "NonLinear EQ On 1", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_DESIGN1", "EQ Design 1", filterDesignOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_ORDER1", "EQ Order 1", filterOrderOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_TYPE1", "EQ Type 1", filterTypeOptions, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ2","Center Frequency 2",20.f,20000.f,1000.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Q2","Q 2",.1f,20.f,1.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMP2","Amplitude 2",-40.f,12.f,0.f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ANALOG_Q_ON2", "Analog Q On 2", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("NONLINEAR_EQ_ON2", "NonLinear EQ On 2", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_DESIGN2", "EQ Design 2", filterDesignOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_ORDER2", "EQ Order 2", filterOrderOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_TYPE2", "EQ Type 2", filterTypeOptions, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ3","Center Frequency 3",20.f,20000.f,1000.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Q3","Q 3",.1f,20.f,1.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMP3","Amplitude 3",-40.f,12.f,0.f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ANALOG_Q_ON3", "Analog Q On 3", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("NONLINEAR_EQ_ON3", "NonLinear EQ On 3", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_DESIGN3", "EQ Design 3", filterDesignOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_ORDER3", "EQ Order 3", filterOrderOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_TYPE3", "EQ Type 3", filterTypeOptions, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ4","Center Frequency 4",20.f,20000.f,1000.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Q4","Q 4",.1f,20.f,1.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMP4","Amplitude 4",-40.f,12.f,0.f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ANALOG_Q_ON4", "Analog Q On 4", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("NONLINEAR_EQ_ON4", "NonLinear EQ On 4", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_DESIGN4", "EQ Design 4", filterDesignOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_ORDER4", "EQ Order 4", filterOrderOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_TYPE4", "EQ Type 4", filterTypeOptions, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ5","Center Frequency 5",20.f,20000.f,1000.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Q5","Q 5",.1f,20.f,1.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("AMP5","Amplitude 5",-40.f,12.f,0.f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ANALOG_Q_ON5", "Analog Q On 5", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("NONLINEAR_EQ_ON5", "NonLinear EQ On 5", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_DESIGN5", "EQ Design 5", filterDesignOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_ORDER5", "EQ Order 5", filterOrderOptions, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("EQ_TYPE5", "EQ Type 5", filterTypeOptions, 0));
    
    return {params.begin(), params.end()};
}
