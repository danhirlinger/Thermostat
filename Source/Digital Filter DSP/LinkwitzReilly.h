/*
  ==============================================================================

    LinkwitzReilly.h
    Created: 28 Sep 2022 2:25:31pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "EQ.h"

class LinkwitzReilly : public EQ
{
public:
    LinkwitzReilly();
    ~LinkwitzReilly() {};
    
    void process(juce::dsp::AudioBlock<float> samples) override;
    
    double processSample(double x, int c, int o) override;
    
private:
    double gma;
    double D;
    double BW;
        
    void updateCoefficients() override;
};
