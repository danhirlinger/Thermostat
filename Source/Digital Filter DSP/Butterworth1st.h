/*
  ==============================================================================

    Butterworth1st.h
    Created: 28 Jul 2022 3:12:31pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>

class Butterworth1st
{
public:
    Butterworth1st(){
        updateCoefficients();
    };
    ~Butterworth1st() = default;
    
private:
    enum FilterType {LPF, HPF};
    double gma;
    double D;
    double Fs = 48000.0;
    double freq = 1000.0;
    double ampdB = 0.0;
    
    FilterType ft = LPF;
    
    double a[2] = {1.0,0.0};
    double b[2] = {0.0,0.0};
    
    double x0[2] = {0.0};
    double y0[2] = {0.0};
    double x1[2] = {0.0};
    double y1[2] = {0.0};
    
public:
    
    void prepare(double Fs){
        setFs(Fs);
    }
    
    void setFs(double newFs){
        Fs = newFs;
        updateCoefficients();
    };
    void setFreq(double newFreq){
        freq = newFreq;
        updateCoefficients();
    };
    void setAmpdB(double newAmpdB){
        ampdB = pow(10.0,newAmpdB/20.0);
        updateCoefficients();
    }
    void setFilterType(int newFT){
        if (newFT == 1)
            ft = FilterType::LPF;
        if (newFT == 2)
            ft = FilterType::HPF;
        updateCoefficients();
    };
    
    
    void processBlock(dsp::AudioBlock<float> samples){
        int numSamples = (int) samples.getNumSamples();
        int numChannels = (int) samples.getNumChannels();
        
        for (uint16 channel = 0; channel < numChannels; ++channel){
            for (uint32 n = 0; n < numSamples; ++n)
            {
                double x = (double)samples.getSample(channel, n);
                x = processSample(x,channel);
                samples.setSample(channel, n, (float)x);
            }
        }
    };
    
    double processSample(double x, int c){
        x0[c] = x;
        // best i've gotten it. still a buzz. TBD
        double bVal = (b[0] * x0[c]) + (b[1] * x1[c]);
        double aVal = (a[1] * y1[c]);
        y0[c] = bVal - aVal;
        
//        y0[c] = (b[0] * x0[c]) + (b[1] * x1[c]) + (-a[1] * y1[c]);

        x1[c] = x;
        y1[c] = y0[c];

        return y0[c]*ampdB;
    };
    
    
private:
    void updateCoefficients(){
        gma = tan((M_PI*freq)/Fs);
        D = 1.0 + gma;
        
        if (ft == FilterType::LPF){
            b[0] = gma/D;
            b[1] = gma/D;
            a[1] = (gma-1.0)/D;
        }
        if (ft == FilterType::HPF){
            b[0] = 1.0/D;
            b[1] = -1.0/D;
            a[1] = (gma-1.0)/D;
        }
    };
};
