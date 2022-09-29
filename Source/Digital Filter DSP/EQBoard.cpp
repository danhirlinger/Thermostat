/*
  ==============================================================================

    EQBoard.cpp
    Created: 24 Aug 2022 12:59:19pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#include "EQBoard.h"


EQBoard::EQBoard(AudioProcessorValueTreeState &p)
{
    
    params.freq1 =          p.getRawParameterValue("FREQ1");
    params.q1 =             p.getRawParameterValue("Q1");
    params.ampdB1 =         p.getRawParameterValue("AMP1");
    params.analogQOn1 =     p.getRawParameterValue("ANALOG_Q_ON1");
    params.nonLinearOn1 =   p.getRawParameterValue("NONLINEAR_EQ_ON1");
    params.filterDesign1 =  p.getRawParameterValue("EQ_DESIGN1");
    params.filterOrder1 =   p.getRawParameterValue("EQ_ORDER1");
    params.filterType1 =    p.getRawParameterValue("EQ_TYPE1");
    
    params.freq2 =          p.getRawParameterValue("FREQ2");
    params.q2 =             p.getRawParameterValue("Q2");
    params.ampdB2 =         p.getRawParameterValue("AMP2");
    params.analogQOn2 =     p.getRawParameterValue("ANALOG_Q_ON2");
    params.nonLinearOn2 =   p.getRawParameterValue("NONLINEAR_EQ_ON2");
    params.filterDesign2 =  p.getRawParameterValue("EQ_DESIGN2");
    params.filterOrder2 =   p.getRawParameterValue("EQ_ORDER2");
    params.filterType2 =    p.getRawParameterValue("EQ_TYPE2");
    
    params.freq3 =          p.getRawParameterValue("FREQ3");
    params.q3 =             p.getRawParameterValue("Q3");
    params.ampdB3 =         p.getRawParameterValue("AMP3");
    params.analogQOn3 =     p.getRawParameterValue("ANALOG_Q_ON3");
    params.nonLinearOn3 =   p.getRawParameterValue("NONLINEAR_EQ_ON3");
    params.filterDesign3 =  p.getRawParameterValue("EQ_DESIGN3");
    params.filterOrder3 =   p.getRawParameterValue("EQ_ORDER3");
    params.filterType3 =    p.getRawParameterValue("EQ_TYPE3");
    
    params.freq4 =          p.getRawParameterValue("FREQ4");
    params.q4 =             p.getRawParameterValue("Q4");
    params.ampdB4 =         p.getRawParameterValue("AMP4");
    params.analogQOn4 =     p.getRawParameterValue("ANALOG_Q_ON4");
    params.nonLinearOn4 =   p.getRawParameterValue("NONLINEAR_EQ_ON4");
    params.filterDesign4 =  p.getRawParameterValue("EQ_DESIGN4");
    params.filterOrder4 =   p.getRawParameterValue("EQ_ORDER4");
    params.filterType4 =    p.getRawParameterValue("EQ_TYPE4");
    
    params.freq5 =          p.getRawParameterValue("FREQ5");
    params.q5 =             p.getRawParameterValue("Q5");
    params.ampdB5 =         p.getRawParameterValue("AMP5");
    params.analogQOn5 =     p.getRawParameterValue("ANALOG_Q_ON5");
    params.nonLinearOn5 =   p.getRawParameterValue("NONLINEAR_EQ_ON5");
    params.filterDesign5 =  p.getRawParameterValue("EQ_DESIGN5");
    params.filterOrder5 =   p.getRawParameterValue("EQ_ORDER5");
    params.filterType5 =    p.getRawParameterValue("EQ_TYPE5");
    
    // Create empty EQ
    for (int i = 0; i < numEQs; ++i)
    {
        eq[i] = new EQ();
    }
    
    updateParameters();

}

EQBoard::~EQBoard(){
    
}

void EQBoard::prepare(juce::dsp::ProcessSpec spec){
    
    eqSpec = spec;
    
    Fs = spec.sampleRate;
    bufferSize = spec.maximumBlockSize;
}

void EQBoard::process(juce::dsp::AudioBlock<float> samples, const int numSamples){
    
    updateParameters();

    for (auto eq : eq)
    {
        for (int c = 0; c < samples.getNumChannels(); c++){
            for (int n = 0; n < numSamples; n++){
                float x = samples.getSample(c, n);
                x = eq->processSample(x, c, eq->filterOrder);
                samples.setSample(c, n, x);
            }
        }
    }
}

void EQBoard::updateParameters(){
    
    bool updateFlag = false;
    int selection = changeInFilterDesign(0);
    if (selection > -1)
        setFilterDesign(0, selection);
    
    if (eq[0]->freq != *params.freq1){
        eq[0]->freq = *params.freq1;
        updateFlag = true;
    };
    if (abs(eq[0]->q - *params.q1) > 0.001f){
        eq[0]->q = *params.q1;
        updateFlag = true;
    };
    if (eq[0]->ampdB != *params.ampdB1){
        eq[0]->ampdB = *params.ampdB1;
        updateFlag = true;
    }
    if (eq[0]->isAnalogQOn != (*params.analogQOn1 > 0.5)) {
        eq[0]->isAnalogQOn = (*params.analogQOn1 > 0.5);
        updateFlag = true;
    }
    if (eq[0]->isNonLinearOn != (*params.nonLinearOn1 > 0.5)) {
        eq[0]->isNonLinearOn = (*params.nonLinearOn1 > 0.5);
        updateFlag = true;
    }
    if (eq[0]->filterOrder != (int)*params.filterOrder1) {
        eq[0]->filterOrder = (int)*params.filterOrder1;
        updateFlag = true;
    }
    if (eq[0]->filterType != (int)*params.filterType1) {
        eq[0]->filterType = (int)*params.filterType1;
        updateFlag = true;
    }
    
    if (updateFlag)
        eq[0]->updateParameters();
    
//    updateFlag = false;
    
}

void EQBoard::setFilterDesign(int eqIndex, int comboBoxIndex){
    delete eq[eqIndex];
    
    switch (comboBoxIndex) {
        case 0:
            eq[eqIndex] = new EQ();
            break;
        case 1:
            eq[eqIndex] = new Butterworth();
            break;
        case 2:
            eq[eqIndex] = new Bessel();
            break;
        default:
            eq[eqIndex] = new EQ();
            break;
    }
    
    
    eq[eqIndex]->prepare(eqSpec);
}

int EQBoard::changeInFilterDesign(int eqIndex){
    
    int selection = -1;
    switch (eqIndex) {
        case 0:
            if (eq[eqIndex]->filterDesign != (int)*params.filterDesign1){
                eq[eqIndex]->filterDesign = (int)*params.filterDesign1;
                selection = (int)*params.filterDesign1;
            }
            break;
        case 1:
            if (eq[eqIndex]->filterDesign != (int)*params.filterDesign2){
                eq[eqIndex]->filterDesign = (int)*params.filterDesign2;
                selection = (int)*params.filterDesign2;
            }
        case 2:
            if (eq[eqIndex]->filterDesign != (int)*params.filterDesign3){
                eq[eqIndex]->filterDesign = (int)*params.filterDesign3;
                selection = (int)*params.filterDesign3;
            }
        case 3:
            if (eq[eqIndex]->filterDesign != (int)*params.filterDesign4){
                eq[eqIndex]->filterDesign = (int)*params.filterDesign4;
                selection = (int)*params.filterDesign4;
            }
        case 4:
            if (eq[eqIndex]->filterDesign != (int)*params.filterDesign5){
                eq[eqIndex]->filterDesign = (int)*params.filterDesign5;
                selection = (int)*params.filterDesign5;
            }
        default:
            break;
    }
    
    return selection;
}
