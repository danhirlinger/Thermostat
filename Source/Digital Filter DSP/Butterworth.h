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
    Butterworth(AudioProcessorValueTreeState &p);
    ~Butterworth() {};
    
    void process(juce::dsp::AudioBlock<float> samples) override;
    
private:
    
    struct {
        std::atomic<float>* freq;
        std::atomic<float>* q;
        std::atomic<float>* ampdB;
        std::atomic<float>* analogQOn;
        std::atomic<float>* nonLinearOn;
        std::atomic<float>* filterDesign;
        std::atomic<float>* filterOrder;
        std::atomic<float>* filterType;
    } params;
    
    double alph;
    double bta;
    double gma;
    double D;
    double BW;
    
    double processSample(double x, int c, int o);
    
    void updateCoefficients() override;
    
    void updateParamsVT();
    
    
};
