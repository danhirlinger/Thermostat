/*
  ==============================================================================

    Bessel.cpp
    Created: 26 Aug 2022 11:06:53am
    Author:  Dan Hirlinger

  ==============================================================================
*/

#include "Bessel.h"


Bessel::Bessel(){
    
    updateParameters();
}


void Bessel::process(juce::dsp::AudioBlock<float> samples){
    
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
}

double Bessel::processSample(double x, int c, int o){
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


void Bessel::updateCoefficients(){
    
    gma = tan((M_PI*freq)*Ts);
    BW = freq/q;
    
    switch (filterOrder) {
        case 0:
            break;
        case 1: // 1st order ; equivalent to Butterworth 1st order
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
                    D = 1. / (3.*gma*gma*gma + 3.*gma + 1.);
                    
                    b[0] = 3.*gma*gma * D;
                    b[1] = 2.* b[0];
                    b[2] = b[0];
                    a[1] = (2.*(3.*gma*gma - 1.)) * D;
                    a[2] = (3.*gma*gma - 3.*gma + 1.) * D;
                    break;
                case 2: // HPF
                    D = 1. / ( gma*gma + M_SQRT2*gma + 1);
                    
                    b[0] = 3.*D;
                    b[1] = -6. * D;
                    b[2] = 3.*D;
                    a[1] = (2.*(gma*gma - 3.)) * D;
                    a[2] = (gma*gma - 3.*gma + 3) * D;
                    break;
                case 3: // BPF ; equivalent to Butterworth 2nd BPF
                    D = 1. / ((1+gma*gma)*freq + gma*BW);
                    
                    b[0] = BW*gma * D;
                    b[1] = 0;
                    b[2] = -1*BW*gma * D;
                    a[1] = 2*freq*(gma*gma - 1) * D;
                    a[2] = ((1+gma*gma)*freq - gma*BW) * D;
                    break;
                case 4: // BSF ; equivalent to Butterworth 2nd BSF
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
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    D = 1. / (15.*gma*gma*gma + 15.*gma*gma + 6.*gma + 1.);
                    b[0] = 15.*gma*gma*gma * D;
                    b[1] = 3*b[0];
                    b[2] = b[1];
                    b[3] = b[0];
                    a[1] = 3.*(15.*gma*gma*gma + 5.*gma*gma - 2.*gma - 1.) * D;
                    a[2] = 3.*(15.*gma*gma*gma + 5.*gma*gma - 2.*gma + 1.) * D;
                    a[3] = (15.*gma*gma*gma - 15.*gma*gma + 6.*gma - 1.) * D;
                    break;
                case 2: // HPF
                    D = 1. / (gma*gma*gma + 6.*gma*gma + 15.*gma + 15.);
                    b[0] = 15.*D;
                    b[1] = -45. * D;
                    b[2] = 45. * D;
                    b[3] = -15. * D;
                    a[1] = 3.*(gma*gma*gma + 2.*gma*gma - 5.*gma - 15.) * D;
                    a[2] = 3.*(gma*gma*gma - 2.*gma*gma - 5.*gma + 15.) * D;
                    a[3] = (gma*gma*gma - 6.*gma*gma + 15.*gma - 15.) * D;
                    break;
                default:
                    break;
            }
            break;
        case 4: // 4th order
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    D = 1. / (105.*gma*gma*gma*gma + 105.*gma*gma*gma + 45.*gma*gma + 10.*gma + 1);
                    
                    b[0] = 105.*gma*gma*gma*gma*D;
                    b[1] = 4*b[0];
                    b[2] = 6*b[0];
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 2*(210.*gma*gma*gma*gma + 105.*gma*gma*gma - 10.*gma - 2) * D;
                    a[2] = 6*(105.*gma*gma*gma*gma - 15.*gma*gma + 1) * D;
                    a[3] = 2*(210.*gma*gma*gma*gma - 105.*gma*gma*gma + 10.*gma - 2) * D;
                    a[4] = (105.*gma*gma*gma*gma - 105.*gma*gma*gma + 45.*gma*gma - 10.*gma + 1) * D;
                    break;
                case 2: // HPF
                    D = 1. / (gma*gma*gma*gma + 10.*gma*gma*gma + 45.*gma*gma + 105.*gma + 105.);
                    b[0] = 105.*D;
                    b[1] = -420. * D;
                    b[2] = 630. * D;
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 2*(2.*gma*gma*gma*gma + 10.*gma*gma*gma - 105.*gma - 210.) * D;
                    a[2] = 6*(6.*gma*gma*gma*gma - 15.*gma*gma + 105.) * D;
                    a[3] = 2*(2.*gma*gma*gma*gma - 10.*gma*gma*gma + 105.*gma - 210.) * D;
                    a[4] = (gma*gma*gma*gma - 10.*gma*gma*gma + 45.*gma*gma - 105.*gma + 105.) * D;
                    break;
                case 3: // BPF
                    D = 1. / (freq*freq*gma*gma*gma*gma + 3*BW*freq*gma*gma*gma + (2*freq*freq + 3*BW*BW)*gma*gma + 3*BW*freq*gma + freq*freq);
                    b[0] = 3*BW*BW*gma*gma*D;
                    b[1] = 0;
                    b[2] = -2 * b[0];
                    b[3] = 0;
                    b[4] = b[0];
                    a[1] = 2*freq * (2*freq*gma*gma*gma*gma + 3*BW*(gma*gma*gma - gma) - 2*freq) * D;
                    a[2] = 2 * (3*freq*freq*gma*gma*gma*gma - (2*freq*freq + 3*BW*BW)*gma*gma + 3*freq*freq) * D;
                    a[3] = 2*freq * (2*freq*gma*gma*gma*gma - 3*BW*(gma*gma*gma - gma) - 2*freq) * D;
                    a[4] = (freq*freq*gma*gma*gma*gma - 3*BW*freq*gma*gma*gma + (2*freq*freq + 3*BW*BW)*gma*gma - 3*BW*freq*gma + freq*freq) * D;
                    break;
                case 4: // BSF
                    D = 1. / (3*freq*freq*gma*gma*gma*gma + 3*BW*freq*gma*gma*gma + (6*freq*freq + BW*BW)*gma*gma + 3*BW*freq*gma + 3*freq*freq);
                    b[0] = (3*freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1)) * D;
                    b[1] = (12*freq*freq*(gma*gma*gma*gma - 1)) * D;
                    b[2] = (6*freq*freq*(3*gma*gma*gma*gma - 2*gma*gma + 3)) * D;
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 6*freq * (2*freq*gma*gma*gma*gma + BW*(gma*gma*gma - gma) - 2*freq) * D;
                    a[2] = 2 * (9*freq*freq*gma*gma*gma*gma - (6*freq*freq + 3*BW*BW)*gma*gma + 9*freq*freq) * D;
                    a[3] = 6*freq * (2*freq*gma*gma*gma*gma - BW*(gma*gma*gma - gma) - 2*freq) * D;
                    a[4] = (3*freq*freq*gma*gma*gma*gma - 3*BW*freq*gma*gma*gma + (6*freq*freq + BW*BW)*gma*gma - 3*BW*freq*gma + 3*freq*freq) * D;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
