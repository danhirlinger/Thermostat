/*
  ==============================================================================

    Butterworth.cpp
    Created: 17 Jul 2022 10:22:34am
    Author:  Dan Hirlinger

  ==============================================================================
*/

#include "Butterworth.h"

Butterworth::Butterworth(){
    
    updateParameters();
}

void Butterworth::process(juce::dsp::AudioBlock<float> samples){
    
    if (filterType != 0){
        int numSamples = (int) samples.getNumSamples();
        int numChannels = (int) samples.getNumChannels();
        
        for (uint16 channel = 0; channel < numChannels; ++channel){
            for (uint32 n = 0; n < numSamples; ++n)
            {
                double x = (double)samples.getSample(channel, n);
                x = processSample(x,channel,filterOrder);
                samples.setSample(channel, n, (float)x * ampLinear);
            }
        }
    }
};

double Butterworth::processSample(double x, int c, int o){
    double y;
    
    double bVal = b[0]*x;
    double aVal = 0.;
    
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
        case 2:
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    D = 1. / (gma*gma + sqrt(2.)*gma + 1.);
                    
                    b[0] = gma*gma * D;
                    b[1] = 2.* gma*gma * D;
                    b[2] = b[0];
                    a[1] = (2.*(gma*gma - 1.)) * D;
                    a[2] = (gma*gma - sqrt(2.)*gma + 1.) * D;
                    break;
                case 2: // HPF
                    D = 1. / (pow(gma,2) + sqrt(2)*gma + 1);
                    
                    b[0] = D;
                    b[1] = -2 * D;
                    b[2] = D;
                    a[1] = (2*(gma*gma - 1))/D;
                    a[2] = (gma*gma - sqrt(2)*gma + 1)/D;
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
        case 3:
            D = pow(gma,3.) + 2.*pow(gma,2.) + 2.*gma + 1.;
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    b[0] = pow(gma,3.)/D;
                    b[1] = (3.*pow(gma,3.))/D;
                    b[2] = 3.*b[0];
                    b[3] = b[0];
                    a[1] = (3.*(pow(gma,3.) + 2.*pow(gma,2.) - (2.*gma) - 3.))/D;
                    a[2] = (3.*(pow(gma,3.) - 2.*pow(gma,2.) - (2.*gma) + 3.))/D;
                    a[3] = (pow(gma,3.) - 2.*pow(gma,2.) + (2.*gma) - 1.)/D;
                    break;
                case 2: // HPF
                    b[0] = 1/D;
                    b[1] = -3/D;
                    b[2] = 3/D;
                    b[3] = -1/D;
                    a[1] = ((3.*pow(gma,3.)) + (2.*pow(gma,2.)) - (2.*gma) - 3.)/D;
                    a[2] = ((3.*pow(gma,3.) - 2.*pow(gma,2.) - 2.*gma + 3.))/D;
                    a[3] = (pow(gma,3.) - 2.*pow(gma,2.) + 2.*gma - 1.)/D;
                    break;
                default:
                    break;
            }
            break;
        case 4:
            D = pow(gma,3.) + 2.*pow(gma,2.) + 2.*gma + 1.;

            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    b[0] = pow(gma,3.)/D;
                    b[1] = (3.*pow(gma,3.))/D;
                    b[2] = 3.*b[0];
                    b[3] = b[0];
                    b[4] = b[0];
                    a[1] = (3.*(pow(gma,3.) + 2.*pow(gma,2.) - (2.*gma) - 3.))/D;
                    a[2] = (3.*(pow(gma,3.) - 2.*pow(gma,2.) - (2.*gma) + 3.))/D;
                    a[3] = (pow(gma,3.) - 2.*pow(gma,2.) + (2.*gma) - 1.)/D;
                    break;
                case 2: // HPF
                    b[0] = 1/D;
                    b[1] = -3/D;
                    b[2] = 3/D;
                    b[3] = -1/D;
                    a[1] = ((3.*pow(gma,3.)) + (2.*pow(gma,2.)) - (2.*gma) - 3.)/D;
                    a[2] = ((3.*pow(gma,3.) - 2.*pow(gma,2.) - 2.*gma + 3.))/D;
                    a[3] = (pow(gma,3.) - 2.*pow(gma,2.) + 2.*gma - 1.)/D;
                case 3: // BPF
                    
                    break;
                case 4: // BSF
                    
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
