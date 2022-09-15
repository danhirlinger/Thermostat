/*
  ==============================================================================

    EQ.h
    Created: 26 Aug 2022 2:09:35pm
    Author:  Dan Hirlinger

  ==============================================================================
 
 Empty EQ 
*/



#pragma once
#include <JuceHeader.h>


class EQ
{
public:
    EQ(){};
    
    virtual ~EQ() {};
    
    virtual void prepare(juce::dsp::ProcessSpec spec){
        
        Fs = spec.sampleRate;
    }
    
    virtual void process(juce::dsp::AudioBlock<float> samples){
        
    }
    
    virtual void setFreq(double newFreq){
        freq = newFreq;
    };
    virtual void setQ(double newQ){
        q = newQ;
    };
    virtual void setAmpdB(double newAmpdB){
        ampdB = newAmpdB;
    };
    virtual void setFilterOrder(int newFilterOrder){
        filterOrder = newFilterOrder;
    };
    virtual void setFilterType(int newFilterType){
        filterType = newFilterType;
    };
    
    double Fs = 48000;
    double freq = 1000;
    double q = 1;
    double ampdB = 0;
    bool isAnalogQOn = false;
    bool isNonLinearOn = false;
    int filterDesign = 0;
    int filterOrder = 0;
    int filterType = 0;
    
    void updateParameters(){
        // virtual??
        setFreq(freq);
        setQ(q);
        setAmpdB(ampdB);
        setFilterOrder(filterOrder);
        setFilterType(filterType);
    }
    
private:
    
};
