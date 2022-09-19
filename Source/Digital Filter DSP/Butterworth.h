/*
  ==============================================================================

    Butterworth.h
    Created: 17 Jul 2022 10:22:34am
    Author:  Dan Hirlinger

  ==============================================================================
 
 LPF, HPF do not have a Q factor
 LPF, HPF can have amplitude change,  which moves shelf up/down
 BPF, BSF have Q option
 BPF, BSF also have amp option
 
*/

#pragma once
#include <JuceHeader.h>
#include "EQ.h"

class Butterworth : public EQ
{
    
public:
    Butterworth();
    ~Butterworth() {};
    
    void process(juce::dsp::AudioBlock<float> samples) override;
    
private:
    
    double gma;
    double D;
    double BW;
    
    double processSample(double x, int c, int o);
    
    double a[5] = {1.0,0.0,0.0,0.0,0.0};
    double b[5] = {0.0,0.0,0.0,0.0,0.0};
    
    //       [c][d]
    double xd[2][4] = {0.0};
    double yd[2][4] = {0.0};

    
    void updateCoefficients() override;
};


