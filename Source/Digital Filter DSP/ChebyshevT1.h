/*
  ==============================================================================

    ChebyshevT1.h
    Created: 28 Sep 2022 3:53:10pm
    Author:  Dan Hirlinger

  ==============================================================================
 
 what to do with R ????
 have Q knob be "R" knob?
*/

#pragma once
#include <JuceHeader.h>
#include "EQ.h"

class ChebyshevT1 : public EQ
{
  
public:
    ChebyshevT1();
    ~ChebyshevT1() {};
    
    void process(juce::dsp::AudioBlock<float> samples) override;
    
private:
    double alph;
    double bta;
    double gma;
    double kpa;
    double lmd;
    double v0;
    double D;
    double BW;
    
    double processSample(double x, int c, int o);
    
    void updateCoefficients() override;
};
