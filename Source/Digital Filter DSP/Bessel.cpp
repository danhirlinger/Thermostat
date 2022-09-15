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

void Bessel::prepare(juce::dsp::ProcessSpec spec){
    
    Fs = spec.sampleRate;
    
//    bw1.prepare(Fs);
    
    
}

void Bessel::process(juce::dsp::AudioBlock<float> samples){
    
    updateParameters();
//
//    if (filterOrder == 1)
//        bw1.processBlock(samples);
//
//    if (filterOrder == 2)
//        bw2.processBlock(samples);
}


void Bessel::updateParameters(){
    
    
}

