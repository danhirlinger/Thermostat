/*
  ==============================================================================

    EQStack.cpp
    Created: 26 Aug 2022 9:35:08am
    Author:  Dan Hirlinger

  ==============================================================================
*/

#include "EQStack.h"

EQStack::EQStack(){
    this->Fs = 48000;
    this->bufferSize = 96000;
    
    
};


EQStack::~EQStack(){
};

void EQStack::process(juce::dsp::AudioBlock<float> samples, const int blockSize, const int channel){
    
};

void EQStack::prepare(double sampleRate, int bufferSize){
    this->Fs = sampleRate;
    this->bufferSize = bufferSize;
};


void EQStack::setFreq(){
    // eqDesign.setFreq(freq);
}

void EQStack::setQ(){
    
}

void EQStack::setAmpdB(){
    
}


void EQStack::updateParams(){
    setFreq();
    setQ();
    setAmpdB();
}

