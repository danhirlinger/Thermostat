/*
  ==============================================================================

    EQStack.h
    Created: 26 Aug 2022 9:35:08am
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once
#include "EQ.h"
#include "Butterworth.h"
#include "Bessel.h"

class EQStack
{
public:
    
    enum DESIGN { BTW , BSL , LWR };
    enum ORDER { FIRST , SECOND , THIRD , FOURTH };
    enum TYPE { LPF , HPF , PKF };
    
    EQStack();
    
    ~EQStack();
    
    
    void process(juce::dsp::AudioBlock<float> samples, const int blockSize, const int channel);
    
    void prepare(double sampleRate, int bufferSize);
    
    void setFreq();
    void setQ();
    void setAmpdB();
    void updateParams();

    double freq;
    double Q;
    double ampdB;
    
    DESIGN design;
    ORDER order;
    TYPE type;
    
private:
    
    
    Butterworth filterDesign;
    
    EQ eq;

    float knobs[3] = {0.0};
    double Fs;
    int bufferSize;
    
};
