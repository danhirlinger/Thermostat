/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThermostatAudioProcessorEditor::ThermostatAudioProcessorEditor (ThermostatAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    
//    setResizable(true, true);
//    setResizeLimits(300, juce::roundToInt(300.0/ratio), 700, juce::roundToInt(700/ratio));
//    getConstrainer()->setFixedAspectRatio(ratio);
    
    
    // can fit 5 EQ bands (width = 300 ; y = 0-450)
    // spectral visualization on right side (width = 450 ; height = 450)
    setSize (750,450);
    
    connectSliderAttachments();
    connectButtonAttachments();
    connectComboBoxAttachments();
    
    addAndMakeVisible(audioProcessor.spectrum);
    addAndMakeVisible(eq1);
    addAndMakeVisible(eq2);
    addAndMakeVisible(eq3);
    addAndMakeVisible(eq4);
    addAndMakeVisible(eq5);
    
}

ThermostatAudioProcessorEditor::~ThermostatAudioProcessorEditor()
{
}

//==============================================================================
void ThermostatAudioProcessorEditor::paint (juce::Graphics& g)
{
    PixelARGB argb { 204, 116, 199, 204};
    Colour c{argb};
    g.fillAll (c);
}

void ThermostatAudioProcessorEditor::resized()
{
    audioProcessor.spectrum.setBounds(30, 30, 390, 390);
    eq1.setBounds(450,0,300,90);
    eq2.setBounds(450,90,300,90);
    eq3.setBounds(450,180,300,90);
    eq4.setBounds(450,270,300,90);
    eq5.setBounds(450,360,300,90);
    
    eq1.resized();
    eq2.resized();
    eq3.resized();
    eq4.resized();
    eq5.resized();
}

void ThermostatAudioProcessorEditor::connectSliderAttachments(){
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"FREQ1",eq1.eqFreq));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"Q1",eq1.eqQ));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"AMP1",eq1.eqAmpdB));

    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"FREQ2",eq2.eqFreq));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"Q2",eq2.eqQ));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"AMP2",eq2.eqAmpdB));
    
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"FREQ3",eq3.eqFreq));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"Q3",eq3.eqQ));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"AMP3",eq3.eqAmpdB));
    
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"FREQ4",eq4.eqFreq));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"Q4",eq4.eqQ));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"AMP4",eq4.eqAmpdB));
    
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"FREQ5",eq5.eqFreq));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"Q5",eq5.eqQ));
    sliderAttachments.emplace_back(new SliderAttachment(audioProcessor.apvts,"AMP5",eq5.eqAmpdB));
}

void ThermostatAudioProcessorEditor::connectButtonAttachments(){
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"ANALOG_Q_ON1",eq1.analogQOn));
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"NONLINEAR_EQ_ON1",eq1.nonLinearOn));

    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"ANALOG_Q_ON2",eq2.analogQOn));
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"NONLINEAR_EQ_ON2",eq2.nonLinearOn));
    
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"ANALOG_Q_ON3",eq3.analogQOn));
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"NONLINEAR_EQ_ON3",eq3.nonLinearOn));
    
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"ANALOG_Q_ON4",eq4.analogQOn));
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"NONLINEAR_EQ_ON4",eq4.nonLinearOn));
    
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"ANALOG_Q_ON5",eq5.analogQOn));
    buttonAttachments.emplace_back(new ButtonAttachment(audioProcessor.apvts,"NONLINEAR_EQ_ON5",eq5.nonLinearOn));
}



void ThermostatAudioProcessorEditor::connectComboBoxAttachments(){
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_DESIGN1",eq1.filterDesign));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_ORDER1",eq1.filterOrder));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_TYPE1",eq1.filterType));
    
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_DESIGN2",eq2.filterDesign));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_ORDER2",eq2.filterOrder));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_TYPE2",eq2.filterType));
    
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_DESIGN3",eq3.filterDesign));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_ORDER3",eq3.filterOrder));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_TYPE3",eq3.filterType));
    
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_DESIGN4",eq4.filterDesign));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_ORDER4",eq4.filterOrder));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_TYPE4",eq4.filterType));
    
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_DESIGN5",eq5.filterDesign));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_ORDER5",eq5.filterOrder));
    comboBoxAttachments.emplace_back(new ComboBoxAttachment(audioProcessor.apvts,"EQ_TYPE5",eq5.filterType));
}
