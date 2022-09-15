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

void Butterworth::prepare(juce::dsp::ProcessSpec spec){
    
    Fs = spec.sampleRate;
    
    bw1.prepare(Fs);
    bw2.prepare(Fs);
    bw3.prepare(Fs);
    
}

void Butterworth::process(juce::dsp::AudioBlock<float> samples){
    
    updateParameters();
    
    if (filterType != 0){
        switch (filterOrder) {
            case 0:
                break;
            case 1:
                bw1.processBlock(samples);
                break;
            case 2:
                bw2.processBlock(samples);
                break;
            case 3:
                bw3.processBlock(samples);
                break;
            default:
                break;
        }
    } 
}

void Butterworth::updateParameters(){
    // first dependent on filter ORDER (which class to call)
    // then dependent on filter TYPE (LPF, HPF)
    
    switch (filterOrder) {
        case 1:
            bw1.setFs(Fs);
            bw1.setFreq(freq);
            bw1.setAmpdB(ampdB);
            bw1.setFilterType(filterType);
            break;
        case 2:
            bw2.setFs(Fs);
            bw2.setFreq(freq);
            bw2.setAmpdB(ampdB);
            // bw2.setQ(q);
            bw2.setFilterType(filterType);
            break;
        case 3:
            bw3.setFs(Fs);
            bw3.setFreq(freq);
            bw3.setAmpdB(ampdB);
            bw3.setFilterType(filterType);
        default:
            break;
    }
}

