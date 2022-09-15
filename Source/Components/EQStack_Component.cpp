/*
  ==============================================================================

    EQStack_Component.cpp
    Created: 3 Aug 2022 2:32:49pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#include "EQStack_Component.h"


EQStack_Component::EQStack_Component(){

    setAllParams();
    
    filterDesign.clear();
    for (int i = 0; i < filterDesignOptions.size() ; ++i){
        filterDesign.addItem(filterDesignOptions[i],i+1);
    }
    filterOrder.clear();
    for (int i = 0; i < filterOrderOptions.size() ; ++i){
        filterOrder.addItem(filterOrderOptions[i],i+1);
    }
    filterType.clear();
    for (int i = 0; i < filterTypeOptions.size() ; ++i){
        filterType.addItem(filterTypeOptions[i],i+1);
    }
    
    freqValue.setText((juce::String)eqFreq.getValue(), NotificationType::sendNotification);
    qValue.setText((juce::String)eqQ.getValue(), NotificationType::sendNotification);
    ampdBValue.setText((juce::String)eqAmpdB.getValue(), NotificationType::sendNotification);

}

void EQStack_Component::paint (juce::Graphics &g){
    g.setColour(juce::Colours::white);
    g.drawLine(this->getX(), this->getBottom(), this->getRight(), this->getBottom(),4.f);
}

void EQStack_Component::resized(){

    eqFreq.setBounds        (10, 20, 40, 40);
    eqQ.setBounds           (65, 20, 40, 40);
    eqAmpdB.setBounds       (120, 20, 40, 40);
    
    analogQOn.setBounds     (175, 20, 30, 15);
    nonLinearOn.setBounds   (175, 60, 30, 15);
    
    freqValue.setBounds     (10, 65, 40, 18);
    qValue.setBounds        (65, 65, 40, 18);
    ampdBValue.setBounds    (120, 65, 40, 18);
    
    freqLabel.setBounds(10, 5, 40, 15);
    qLabel.setBounds(65, 5, 40, 15);
    ampdBLabel.setBounds(120, 5, 40, 15);
    analogQOnLabel.setBounds(160, 5, 60, 15);
    nonLinearOnLabel.setBounds(160, 43, 60, 15);
    
    filterDesign.setBounds  (220, 10, 70, 18);
    filterOrder.setBounds   (220, 37, 70, 18);
    filterType.setBounds    (220, 64, 70, 18);
    
    setAllParams();
}

void EQStack_Component::setAllParams(){
    
    setSliderParams(eqFreq);
    setSliderParams(eqQ);
    setSliderParams(eqAmpdB);
    
    // BUTTONS
    addAndMakeVisible(analogQOn);
    addAndMakeVisible(nonLinearOn);
    
    // VALUE LABELS
    addAndMakeVisible(freqValue);
    freqValue.onTextChange = [this] {
        eqFreq.setValue(freqValue.getText().getDoubleValue());
    };
    addAndMakeVisible(qValue);
    addAndMakeVisible(ampdBValue);
    
    // TEXT LABELS
    freqLabel.setText("freq", NotificationType::dontSendNotification);
    addAndMakeVisible(freqLabel);
    qLabel.setText("Q", NotificationType::dontSendNotification);
    addAndMakeVisible(qLabel);
    ampdBLabel.setText("amp", NotificationType::dontSendNotification);
    addAndMakeVisible(ampdBLabel);
    analogQOnLabel.setText("auto-Q", NotificationType::dontSendNotification);
    addAndMakeVisible(analogQOnLabel);
    nonLinearOnLabel.setText("non-linear", NotificationType::dontSendNotification);
    addAndMakeVisible(nonLinearOnLabel);


    
    // COMBO BOXES
    filterDesign.setTextWhenNothingSelected("Design");
//    filterDesign.setSelectedId(0);
    setComboBoxParams(filterDesign);
    filterOrder.setTextWhenNothingSelected("Order");
//    filterOrder.setSelectedId(0);
    setComboBoxParams(filterOrder);
    filterType.setTextWhenNothingSelected("Type");
//    filterType.setSelectedId(0);
    setComboBoxParams(filterType);
}

void EQStack_Component::setSliderParams(juce::Slider &slider){
    
    slider.addListener(this);
    addAndMakeVisible(slider);
}

void EQStack_Component::setComboBoxParams(juce::ComboBox &comboBox){
    
    addAndMakeVisible(comboBox);
}
// comboBox.setItemEnabled(int itemId, bool shouldBeEnabled)

void EQStack_Component::sliderValueChanged(Slider *slider)
{
    if (slider == &eqFreq)
    {
        freqValue.setText((juce::String)slider->getValue(), NotificationType::sendNotification);
    }
    
    if (slider == &eqQ)
    {
        qValue.setText((juce::String)slider->getValue(), NotificationType::sendNotification);
    }
    
    if (slider == &eqAmpdB)
    {
        ampdBValue.setText((juce::String)slider->getValue(), NotificationType::sendNotification);
    }
}



// GUI Stuff to do
//  Add labels for buttons, knobs
//  +/- component in corner (further, ability to add/remove EQ stack
//  Spectrum visualization
//
