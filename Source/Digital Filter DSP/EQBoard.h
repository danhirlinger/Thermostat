/*
  ==============================================================================

    EQBoard.h
    Created: 24 Aug 2022 12:59:19pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once
#include "EQStack.h"
#include "Butterworth.h"
#include "Bessel.h"

class EQBoard
{
public:
    
    EQBoard(AudioProcessorValueTreeState &p);
    
    ~EQBoard();
    
    void prepare(juce::dsp::ProcessSpec spec);
    
    void process(juce::dsp::AudioBlock<float> samples, const int numSamples);
    
    void setFilterDesign(int eqIndex, int comboBoxIndex);
    
private:
    
    struct {
        std::atomic<float>* freq1;
        std::atomic<float>* q1;
        std::atomic<float>* ampdB1;
        std::atomic<float>* analogQOn1;
        std::atomic<float>* nonLinearOn1;
        std::atomic<float>* filterDesign1;
        std::atomic<float>* filterOrder1;
        std::atomic<float>* filterType1;
        
        std::atomic<float>* freq2;
        std::atomic<float>* q2;
        std::atomic<float>* ampdB2;
        std::atomic<float>* analogQOn2;
        std::atomic<float>* nonLinearOn2;
        std::atomic<float>* filterDesign2;
        std::atomic<float>* filterOrder2;
        std::atomic<float>* filterType2;
        
        std::atomic<float>* freq3;
        std::atomic<float>* q3;
        std::atomic<float>* ampdB3;
        std::atomic<float>* analogQOn3;
        std::atomic<float>* nonLinearOn3;
        std::atomic<float>* filterDesign3;
        std::atomic<float>* filterOrder3;
        std::atomic<float>* filterType3;
        
        std::atomic<float>* freq4;
        std::atomic<float>* q4;
        std::atomic<float>* ampdB4;
        std::atomic<float>* analogQOn4;
        std::atomic<float>* nonLinearOn4;
        std::atomic<float>* filterDesign4;
        std::atomic<float>* filterOrder4;
        std::atomic<float>* filterType4;
        
        std::atomic<float>* freq5;
        std::atomic<float>* q5;
        std::atomic<float>* ampdB5;
        std::atomic<float>* analogQOn5;
        std::atomic<float>* nonLinearOn5;
        std::atomic<float>* filterDesign5;
        std::atomic<float>* filterOrder5;
        std::atomic<float>* filterType5;
        
    } params;
    
    dsp::ProcessSpec eqSpec;
    
    double Fs = 48000;
    int bufferSize = 512;
    
    const static int numEQs = 5;
    
    bool eqOn[numEQs] = {true};
    
    EQ *eq[numEQs];
    
    int changeInFilterDesign(int eqIndex);
    void updateParameters();
    
};
