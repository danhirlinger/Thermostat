/*
  ==============================================================================

    Bessel.h
    Created: 26 Aug 2022 11:06:53am
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once
#include "EQ.h"
#include <JuceHeader.h>

class Bessel : public EQ
{
public:
    Bessel();
    
    ~Bessel() {};
    
    void prepare(juce::dsp::ProcessSpec spec) override;
    
    void process(juce::dsp::AudioBlock<float> samples) override;
    
//    void setFreq(double newFreq);
    
private:
    
    void updateParameters();
};
    
