/*
  ==============================================================================

    Butterworth.h
    Created: 17 Jul 2022 10:22:34am
    Author:  Dan Hirlinger

  ==============================================================================
 
 LPF, HPF do not have a Q factor
 LPF, HPF can have amplitude change,  which moves shelf up/down
 BPF, BSF have Q option
 BPF, BSF have amp option??????
 
*/

#pragma once
#include <JuceHeader.h>
#include "EQ.h"
#include "Butterworth1st.h"
#include "Butterworth2nd.h"
#include "Butterworth3rd.h"

class Butterworth : public EQ
{
    
public:

    Butterworth();
    ~Butterworth() {};
    
    void prepare(juce::dsp::ProcessSpec spec) override;
    
    void process(juce::dsp::AudioBlock<float> samples) override;
    
    Butterworth1st bw1;
    Butterworth2nd bw2;
    Butterworth3rd bw3;
    
private:
    
    double Fs = 48000;
    
    void updateParameters();
};


