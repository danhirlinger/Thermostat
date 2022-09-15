/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./Components/EQStack_Component.h"

//==============================================================================
/**
*/
class ThermostatAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ThermostatAudioProcessorEditor (ThermostatAudioProcessor&);
    ~ThermostatAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void connectSliderAttachments();
    void connectComboBoxAttachments();
    void connectButtonAttachments();
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ThermostatAudioProcessor& audioProcessor;
    
    const double ratio = 300.0 / 500.0;
    
    EQStack_Component eq1;
    EQStack_Component eq2;
    EQStack_Component eq3;
    EQStack_Component eq4;
    EQStack_Component eq5;
    
    
public:
    
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>> comboBoxAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThermostatAudioProcessorEditor)
};
