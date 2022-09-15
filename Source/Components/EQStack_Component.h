/*
  ==============================================================================

    EQStack_Component.h
    Created: 3 Aug 2022 2:32:49pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ThermoDial.h"
#include "ThermoButton.h"
#include "ThermoLabel.h"
#include "ThermoComboBox.h"
#include "ThermoItemLabel.h"


class EQStack_Component : public juce::Component,
                          public Slider::Listener,
                          public TextEditor::Listener
{
    
public:
    EQStack_Component();
    ~EQStack_Component() = default;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // SLIDERS
    ThermoDial eqFreq;
    ThermoDial eqQ;
    ThermoDial eqAmpdB;
    
    // BUTTONS
    ThermoButton analogQOn;
    ThermoButton nonLinearOn;
    
    // VALUE LABELS
    ThermoLabel freqValue;
    ThermoLabel qValue;
    ThermoLabel ampdBValue;
    
    // TEXT LABELS
    ThermoItemLabel freqLabel;
    ThermoItemLabel qLabel;
    ThermoItemLabel ampdBLabel;
    ThermoItemLabel analogQOnLabel;
    ThermoItemLabel nonLinearOnLabel;
    
    // COMBO BOXES
    ThermoComboBox filterDesign;
    ThermoComboBox filterOrder;
    ThermoComboBox filterType;
        
    void setAllParams();
    
private:
    
    const int filterTypeRadioId = 1;
    juce::StringArray filterTypeOptions = juce::StringArray {"Type","LPF","HPF","BPF", "BSF"};
    
    const int filterOrderRadioId = 1;
    juce::StringArray filterOrderOptions = juce::StringArray {"Order","1st","2nd","3rd","4th"};
    
    const int filterDesignRadioId = 1;
    juce::StringArray filterDesignOptions = juce::StringArray {"Design","Butterworth","Bessel","Linkwitz-Reilly"};

    
    float compRatio = 300/500;
    
    void setSliderParams(juce::Slider &slider);
    void setButtonBarams(juce::Button &button);
    void setComboBoxParams(juce::ComboBox &comboBox);
    
    void sliderValueChanged(Slider *slider) override;
};
