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
    
    void process(juce::dsp::AudioBlock<float> samples) override;
    
    double processSample(double x, int c, int o) override;

private:
    double gma;
    double D;
    double BW;
    
    
    void updateCoefficients() override;
};
    
