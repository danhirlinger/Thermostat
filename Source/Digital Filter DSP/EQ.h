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
#ifndef __APPLE__
#define M_PI 3.14159265358979323846
#endif

class EQ
{
public:
    EQ(){
        filterDesign = 0;
        filterOrder = 0;
        filterType = 0;
    };
    
    virtual ~EQ(){};
    
    virtual void prepare(juce::dsp::ProcessSpec spec){
        
        Fs = spec.sampleRate;
        setFs(Fs);
    }
    
    virtual void process(juce::dsp::AudioBlock<float> samples){}
    
    virtual void setFs(double newFs){
        Fs = newFs;
        Ts = 1./Fs;
        updateCoefficients();
    }
    
    virtual void setFreq(double newFreq){
        freq = newFreq;
    };
    virtual void setQ(double newQ){
        q = newQ;
    };
    virtual void setAmpdB(double newAmpdB){
        ampdB = newAmpdB;
        ampLinear = pow(10.0,ampdB/20.0);
    };
    virtual void setFilterOrder(int newFilterOrder){
        filterOrder = newFilterOrder;
    };
    virtual void setFilterType(int newFilterType){
        filterType = newFilterType;
    };
    
    double Fs = 48000;
    double Ts = 1./Fs;
    double freq = 1000;
    double q = 1;
    double ampdB = 0;
    double ampLinear;
    bool isAnalogQOn = false;
    bool isNonLinearOn = false;
    int filterDesign;
    int filterOrder;
    int filterType;
    
    void updateParameters(){
        setFreq(freq);
        setQ(q);
        setAmpdB(ampdB);
        setFilterOrder(filterOrder);
        setFilterType(filterType);
//        reset();
        updateCoefficients();
    }
    
protected:
    virtual void updateCoefficients(){};
    
    double a[5] = {1.0,0.0,0.0,0.0,0.0};
    double b[5] = {0.0,0.0,0.0,0.0,0.0};
    //       [d][c]
    double xd[4][2] = {0.0};
    double yd[4][2] = {0.0};
    
private:
    
    void reset(){
        a[0] = 1.0;
        b[0] = 0.0;
        for (int n = 1 ; n < 4; n++){
            a[n] = 0.0;
            b[n] = 0.0;
        }
        
        for (int c = 0; c < 2; c++){
            for (int n = 0; n < 4; n++){
                xd[n][c] = 0.0;
                yd[n][c] = 0.0;
            }
        }

    };
    
};
