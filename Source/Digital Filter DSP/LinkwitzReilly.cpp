/*
  ==============================================================================

    LinkwitzReilly.cpp
    Created: 28 Sep 2022 2:25:31pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#include "LinkwitzReilly.h"

LinkwitzReilly::LinkwitzReilly(){
    
    updateParameters();
}

void LinkwitzReilly::process(juce::dsp::AudioBlock<float> samples){
    
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

double LinkwitzReilly::processSample(double x, int c, int o){
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

void LinkwitzReilly::updateCoefficients(){
    
    gma = tan((M_PI*freq)*Ts);
    BW = freq/q;
    
    switch (filterOrder) {
        case 0:
            break;
        case 1: // 1st order does not exist
            break;
        case 2: // 2nd order only has LPF, HPF
            D = 1. / (gma*gma + 2*gma + 1.);
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    b[0] = gma*gma * D;
                    b[1] = 2.* gma*gma * D;
                    b[2] = b[0];
                    a[1] = (2.*(gma*gma - 1.)) * D;
                    a[2] = (gma*gma - 2*gma + 1.) * D;
                    break;
                case 2: // HPF
                    b[0] = D;
                    b[1] = -2 * D;
                    b[2] = D;
                    a[1] = (2*(gma*gma - 1)) * D;
                    a[2] = (gma*gma - 2*gma + 1) * D;
                    break;
                default:
                    break;
            }
            break;
        case 3: // 3rd order does not exist
            break;
        case 4: // 4th order
            switch (filterType) {
                case 0:
                    break;
                case 1: // LPF
                    D = 1. / (gma*gma*gma*gma + 2*M_SQRT2*gma*gma*gma + 4*gma*gma + 2*M_SQRT2*gma + 1);
                    
                    b[0] = gma*gma*gma*gma*D;
                    b[1] = 4*b[0];
                    b[2] = 6*b[0];
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 4 * (gma*gma*gma*gma + M_SQRT2*gma*gma*gma - M_SQRT2*gma - 1) * D;
                    a[2] = 2 * (3*gma*gma*gma*gma - 4*gma*gma + 3) * D;
                    a[3] = 4 * (gma*gma*gma*gma - M_SQRT2*gma*gma*gma + M_SQRT2*gma - 1) * D;
                    a[4] = (gma*gma*gma*gma - 2*M_SQRT2*gma*gma*gma + 4*gma*gma - 2*M_SQRT2*gma + 1) * D;
                    break;
                case 2: // HPF
                    D = 1. / (gma*gma*gma*gma + 2*M_SQRT2*gma*gma*gma + 4*gma*gma + 2*M_SQRT2*gma + 1);
                    
                    b[0] = D;
                    b[1] = -4 * D;
                    b[2] = 6 * D;
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 4 * (gma*gma*gma*gma + M_SQRT2*gma*gma*gma - M_SQRT2*gma - 1) * D;
                    a[2] = 2 * (3*gma*gma*gma*gma - 4*gma*gma + 3) * D;
                    a[3] = 4 * (gma*gma*gma*gma - M_SQRT2*gma*gma*gma + M_SQRT2*gma - 1) * D;
                    a[4] = (gma*gma*gma*gma - 2*M_SQRT2*gma*gma*gma + 4*gma*gma - 2*M_SQRT2*gma + 1) * D;
                    break;
                case 3: // BPF
                    D = 1. / (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) + 2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma);
                    
                    b[0] = (BW*BW*gma*gma) * D;
                    b[1] = 0;
                    b[2] = -2 * b[0];
                    b[3] = 0;
                    b[4] = b[0];
                    a[1] = 4 * (freq*freq*(gma*gma*gma*gma - 1) + BW*freq*gma*(gma*gma - 1)) * D;
                    a[2] = 2 * (3*freq*freq*(gma*gma*gma*gma + 1) - gma*gma*(2*freq*freq + BW*BW)) * D;
                    a[3] = 4 * (freq*freq*(gma*gma*gma*gma - 1) + BW*freq*gma*(1 - gma*gma)) * D;
                    a[4] = (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) - 2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma) * D;
                    break;
                case 4: // BSF
                    D = 1. / (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) + 2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma);
                    
                    b[0] = (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1)) * D;
                    b[1] = (4*freq*freq*(gma*gma*gma*gma - 1)) * D;
                    b[2] = (2*freq*freq*(3*gma*gma*gma*gma - 2*gma*gma + 3)) * D;
                    b[3] = b[1];
                    b[4] = b[0];
                    a[1] = 4 * (freq*freq*(gma*gma*gma*gma - 1) + BW*freq*gma*(gma*gma - 1)) * D;
                    a[2] = 2 * (3*freq*freq*(gma*gma*gma*gma + 1) - gma*gma*(2*freq*freq + BW*BW)) * D;
                    a[3] = 4 * (freq*freq*(gma*gma*gma*gma - 1) + BW*freq*gma*(1 - gma*gma)) * D;
                    a[4] = (freq*freq*(gma*gma*gma*gma + 2*gma*gma + 1) - 2*BW*freq*gma*(gma*gma + 1) + BW*BW*gma*gma) * D;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
