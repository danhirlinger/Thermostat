/*
  ==============================================================================

    Butterworth3rd.h
    Created: 11 Sep 2022 4:18:02pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>


class Butterworth3rd
{
public:
    Butterworth3rd(){
        updateCoefficients();
    }
    ~Butterworth3rd() = default;
    
private:
    enum FilterType {LPF, HPF};
    
    double gma;
    double D;
    double Fs = 48000.0;
    double freq = 1000.f;
    double ampdB = 0.0;
    double Q = 1.0;
    
    FilterType ft = LPF;
    
    double a[4] = {1.0,0.0,0.0,0.0};
    double b[4] = {0.0,0.0,0.0,0.0};
    
    //      [c][d]
    double xd[2][3] = {0.0};
    double yd[2][3] = {0.0};
    double x0[2] = {0.0};
    double y0[2] = {0.0};
    
public:
    
    void prepare(double Fs){
        setFs(Fs);
    };
    void setFs(double newFs){
        Fs = newFs;
        updateCoefficients();
    }
    void setFreq(double newFreq){
        freq = newFreq;
        updateCoefficients();
    }
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
    }
    
    double processSample(double x, int c){
        x0[c] = x;

        double bVal = (b[0]*x0[c]) + (b[1]*xd[c][0]) + (b[2]*xd[c][1]) + (b[3]*xd[c][2]);
        double aVal = (a[1]*yd[c][0]) + (a[2]*yd[c][1]) + (a[3]*yd[c][2]);
        y0[c] = bVal - aVal;

        xd[c][2] = xd[c][1];
        yd[c][2] = yd[c][1];
        xd[c][1] = xd[c][0];
        yd[c][1] = yd[c][0];
        xd[c][0] = x0[c];
        yd[c][0] = y0[c];
        
        return y0[c]*ampdB;
    }
    
private:
    void updateCoefficients(){
        gma = tan((M_PI*freq)/Fs);
        D = pow(gma,3.) + 2.*pow(gma,2.) + 2.*gma + 1.;

        if (ft == FilterType::LPF){
            b[0] = pow(gma,3.)/D;
            b[1] = (3.*pow(gma,3.))/D;
            b[2] = 3.*b[0];
            b[3] = b[0];
            a[1] = (3.*(pow(gma,3.) + 2.*pow(gma,2.) - (2.*gma) - 3.))/D;
            a[2] = (3.*(pow(gma,3.) - 2.*pow(gma,2.) - (2.*gma) + 3.))/D;
            a[3] = (pow(gma,3.) - 2.*pow(gma,2.) + (2.*gma) - 1.)/D;
        }
        if (ft == FilterType::HPF){
            b[0] = 1/D;
            b[1] = -3/D;
            b[2] = 3/D;
            b[3] = -1/D;
            a[1] = ((3.*pow(gma,3.)) + (2.*pow(gma,2.)) - (2.*gma) - 3.)/D;
            a[2] = ((3.*pow(gma,3.) - 2.*pow(gma,2.) - 2.*gma + 3.))/D;
            a[3] = (pow(gma,3.) - 2.*pow(gma,2.) + 2.*gma - 1.)/D;
        }
    }
};
