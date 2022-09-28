/*
  ==============================================================================

    Butterworth.cpp
    Created: 17 Jul 2022 10:22:34am
    Author:  Dan Hirlinger

  ==============================================================================
*/

#include "Butterworth.h"

Butterworth::Butterworth(AudioProcessorValueTreeState &p){
    
    params.freq =          p.getRawParameterValue("FREQ1");
    params.q =             p.getRawParameterValue("Q1");
    params.ampdB =         p.getRawParameterValue("AMP1");
    params.analogQOn =     p.getRawParameterValue("ANALOG_Q_ON1");
    params.nonLinearOn =   p.getRawParameterValue("NONLINEAR_EQ_ON1");
    params.filterDesign =  p.getRawParameterValue("EQ_DESIGN1");
    params.filterOrder =   p.getRawParameterValue("EQ_ORDER1");
    params.filterType =    p.getRawParameterValue("EQ_TYPE1");
    
    updateParamsVT();
    updateParameters();
}

void Butterworth::process(juce::dsp::AudioBlock<float> samples){
    
    updateParamsVT();
    if (filterType != 0){
        int numSamples = (int) samples.getNumSamples();
        int numChannels = (int) samples.getNumChannels();
        
        for (uint16 channel = 0; channel < numChannels; ++channel){
            for (uint32 n = 0; n < numSamples; ++n)
            {
                double x = (double)samples.getSample(channel, n);
                x = processSample(x,channel,filterOrder);
                samples.setSample(channel, n, (float)x);
            }
        }
    }
};

double Butterworth::processSample(double x, int c, int o){
    double y;
    
    double bVal = b[0]*x;
    double aVal = 0.0;
    
    for (int n = 0 ; n < o ; n++){
        bVal = bVal + b[n+1]*xd[n][c];
        aVal = aVal + a[n+1]*yd[n][c];
    }
    
    y = bVal - aVal;
    
    for (int n = o-1 ; n > 0 ; n = n-1){
        xd[n][c] = xd[n-1][c];
        yd[n][c] = yd[n-1][c];
    }
    
    xd[0][c] = x;
    yd[0][c] = y;
    
    return y;
}

void Butterworth::updateParamsVT(){
    
    bool updateFlag = false;
    
    if (freq != *params.freq){
        freq = *params.freq;
        updateFlag = true;
    };
    if (abs(q - *params.q) > 0.001f){
        q = *params.q;
        updateFlag = true;
    };
    if (ampdB != *params.ampdB){
        ampdB = *params.ampdB;
        updateFlag = true;
    }
    if (isAnalogQOn != (*params.analogQOn > 0.5)) {
        isAnalogQOn = (*params.analogQOn > 0.5);
        updateFlag = true;
    }
    if (isNonLinearOn != (*params.nonLinearOn > 0.5)) {
        isNonLinearOn = (*params.nonLinearOn > 0.5);
        updateFlag = true;
    }
    if (filterOrder != (int)*params.filterOrder) {
        filterOrder = (int)*params.filterOrder;
        updateFlag = true;
    }
    if (filterType != (int)*params.filterType) {
        filterType = (int)*params.filterType;
        updateFlag = true;
    }
    
    if (updateFlag)
        updateParameters();
    
}

void Butterworth::updateCoefficients(){
    
    gma = tan((M_PI*freq)*Ts);
    BW = q/freq;
    
    switch (filterOrder) {
        case 0:
            break;
        case 1: // 1st order
            D = 1.0/(1.0 + gma);
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    b[0] = gma * 1.0 * D;
                    b[1] = gma * 1.0 * D;
                    a[1] = (gma-1.0) * D;
                    break;
                case 2: // HPF
                    b[0] = 1.0 * D;
                    b[1] = -1.0 * D;
                    a[1] = (gma-1.0) * D;
                    break;
                default:
                    break;
            }
            break;
        case 2: // 2nd order
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    D = 1. / (gma*gma + M_SQRT2*gma + 1.);
                    
                    b[0] = gma*gma * D;
                    b[1] = 2.* gma*gma * D;
                    b[2] = b[0];
                    a[1] = (2.*(gma*gma - 1.)) * D;
                    a[2] = (gma*gma - M_SQRT2*gma + 1.) * D;
                    break;
                case 2: // HPF
                    D = 1. / ( gma*gma + M_SQRT2*gma + 1);
                    
                    b[0] = D;
                    b[1] = -2 * D;
                    b[2] = D;
                    a[1] = (2*(gma*gma - 1)) * D;
                    a[2] = (gma*gma - M_SQRT2*gma + 1) * D;
                    break;
                case 3: // BPF
                    D = 1. / ((1+gma*gma)*freq + gma*BW);
                    
                    b[0] = BW*gma * D;
                    b[1] = 0;
                    b[2] = -1*BW*gma * D;
                    a[1] = 2*freq*(gma*gma - 1) * D;
                    a[2] = ((1+gma*gma)*freq - gma*BW) * D;
                    break;
                case 4: // BSF
                    D = 1. / ((1+gma*gma)*freq + gma*BW);
                    
                    b[0] = freq*(gma*gma+1) * D;
                    b[1] = 2*freq*(gma*gma-1) * D;
                    b[2] = freq*(gma*gma+1) * D;
                    a[1] = 2*freq*(gma*gma-1) * D;
                    a[2] = ((1+gma*gma)*freq - gma*BW) * D;
                    break;
                default:
                    break;
            }
            break;
        case 3: // 3rd order
            D = 1. / (gma*gma*gma + 2.*gma*gma + 2.*gma + 1.);
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    b[0] = gma*gma*gma * D;
                    b[1] = (3.*gma*gma*gma) * D;
                    b[2] = 3.*b[0];
                    b[3] = b[0];
                    a[1] = (3.*gma*gma*gma + 2.*gma*gma - 2.*gma - 3.) * D;
                    a[2] = (3.*gma*gma*gma - 2.*gma*gma - (2.*gma) + 3.) * D;
                    a[3] = (gma*gma*gma - 2.*gma*gma + (2.*gma) - 1.) * D;
                    break;
                case 2: // HPF
                    b[0] = D;
                    b[1] = -3 * D;
                    b[2] = 3 * D;
                    b[3] = -1 * D;
                    a[1] = (3.*gma*gma*gma + (2.*gma*gma) - (2.*gma) - 3.) * D;
                    a[2] = (3.*gma*gma*gma - 2.*gma*gma - 2.*gma + 3.) * D;
                    a[3] = (gma*gma*gma - 2.*gma*gma + 2.*gma - 1.) * D;
                    break;
                default:
                    break;
            }
            break;
        case 4: // 4th order
            alph = -2*(cos((5.*M_PI)/8.) + cos((7.*M_PI)/8));
            bta = 2*(1+2*(cos((5.*M_PI)/8)*cos((7*M_PI)/8)));
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    D = 1. / (gma*gma*gma*gma + alph*gma*gma*gma + bta*gma*gma + alph*gma + 1);
                    b[0] = gma*gma*gma*gma*D;
                    b[1] = 4*b[0];
                    b[2] = 6*b[0];
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 2*(2*gma*gma*gma*gma + alph*gma*gma*gma - alph*gma - 2) * D;
                    a[2] = 2*(3*gma*gma*gma*gma - bta*gma*gma + 3) * D;
                    a[3] = 2*(2*gma*gma*gma*gma - alph*gma*gma*gma + alph*gma - 2) * D;
                    a[4] = (gma*gma*gma*gma - alph*gma*gma*gma + bta*gma*gma - alph*gma + 1) * D;
                    break;
                case 2: // HPF
                    D = 1. / (gma*gma*gma*gma + alph*gma*gma*gma + bta*gma*gma + alph*gma + 1);
                    b[0] = D;
                    b[1] = -4 * D;
                    b[2] = 6 * D;
                    b[3] = b[1];
                    b[4] = D;
                    a[1] = 2*(2*gma*gma*gma*gma + alph*gma*gma*gma - alph*gma - 2) * D;
                    a[2] = 2*(3*gma*gma*gma*gma - bta*gma*gma + 3) * D;
                    a[3] = 2*(2*gma*gma*gma*gma - alph*gma*gma*gma + alph*gma - 2) * D;
                    a[4] = (gma*gma*gma*gma - alph*gma*gma*gma + bta*gma*gma - alph*gma + 1) * D;
                    break;
                case 3: // BPF
                    D = 1. / (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) + M_SQRT2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma);
                    b[0] = (BW*BW*gma*gma) * D;
                    b[1] = 0;
                    b[2] = -2 * b[0];
                    b[3] = 0;
                    b[4] = b[0];
                    a[1] = 2 * (2*freq*freq*(gma*gma*gma*gma - 1) + M_SQRT2*BW*freq*gma*(gma*gma - 1)) * D;
                    a[2] = 2 * (3*freq*freq*(gma*gma*gma*gma + 1) - gma*gma*(2*freq*freq + BW*BW)) * D;
                    a[3] = 2 * (2*freq*freq*(gma*gma*gma*gma - 1) + M_SQRT2*BW*freq*gma*(1 - gma*gma)) * D;
                    a[4] = (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) - M_SQRT2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma) * D;
                    break;
                case 4: // BSF
                    D = 1. / (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) + M_SQRT2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma);
                    b[0] = (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1)) * D;
                    b[1] = (4*freq*freq*(gma*gma*gma*gma - 1)) * D;
                    b[2] = (2*freq*freq*(3*gma*gma*gma*gma - 2*gma*gma + 3)) * D;
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 2 * (2*freq*freq*(gma*gma*gma*gma - 1) + M_SQRT2*BW*freq*gma*(gma*gma - 1)) * D;
                    a[2] = 2 * (3*freq*freq*(gma*gma*gma*gma + 1) - gma*gma*(2*freq*freq + BW*BW)) * D;
                    a[3] = 2 * (2*freq*freq*(gma*gma*gma*gma - 1) + M_SQRT2*BW*freq*gma*(1 - gma*gma)) * D;
                    a[4] = (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) - M_SQRT2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma) * D;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
