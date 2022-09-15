/*
  ==============================================================================

    SpectrumAnalyser.h
    Created: 24 Aug 2022 2:57:58pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once

class AnalyserComponent   : public juce::Component,
                            private juce::Timer
{
public:
    AnalyserComponent() :
    forwardFFT (fftOrder),
    window (fftSize, juce::dsp::WindowingFunction<float>::hann)
    {
        setOpaque (true);
//        setAudioChannels (2, 0);  // we want a couple of input channels but no outputs
        startTimerHz (30);
//        setSize (700, 500);
    }

    ~AnalyserComponent() override
    {
    }

    //==============================================================================

    void process(juce::dsp::AudioBlock<float> samples)
    {
        for (int c = 0 ; c < samples.getNumChannels() ; c++){
            
            for (int n = 0 ; n < samples.getNumSamples() ; n++){
                pushNextSampleIntoFifo ( samples.getSample(c, n) );
            };
        }
    }
    

    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        PixelARGB argb { 204, 116, 199, 204};
        Colour c{argb};
        g.fillAll (c);

        g.setColour (juce::Colours::black);
        drawFrame (g);
    }

    void timerCallback() override
    {
        if (nextFFTBlockReady)
        {
            drawNextFrameOfSpectrum();
            nextFFTBlockReady = false;
            repaint();
        }
    }

    void pushNextSampleIntoFifo (float sample) noexcept
    {
        // if the fifo contains enough data, set a flag to say
        // that the next frame should now be rendered..
        if (fifoIndex == fftSize)
        {
            if (! nextFFTBlockReady)
            {
                juce::zeromem (fftData, sizeof (fftData));
                memcpy (fftData, fifo, sizeof (fifo));
                nextFFTBlockReady = true;
            }
            fifoIndex = 0;
        }
        fifo[fifoIndex++] = sample;
    }

    void drawNextFrameOfSpectrum() {
        // apply windowing function to data
        window.multiplyWithWindowingTable(fftData, fftSize);
        
        // then render FFT data
        forwardFFT.performFrequencyOnlyForwardTransform(fftData);
        
        auto mindB = -100.f;
        auto maxdB = 0.f;
        
        for (int i = 0; i < scopeSize ; ++i)
        {
            auto skewedProportionX = 1.f - std::exp (std::log (1.f - (float) i / (float) scopeSize) * 0.2f);
            auto fftDataIndex = juce::jlimit (0, fftSize/2, (int) (skewedProportionX * (float) fftSize * 0.5f));
            auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (fftData[fftDataIndex])
                                                   - juce::Decibels::gainToDecibels ((float) fftSize)),
                                     mindB, maxdB, 0.f, 1.f);
            scopeData[i] = level;
        }
    }
    void drawFrame (juce::Graphics& g)
    {
        for (int i = 1; i < scopeSize; ++i)
        {
            auto width = getLocalBounds().getWidth();
            auto height = getLocalBounds().getHeight();
            
            g.drawLine ({ (float) juce::jmap (i - 1, 0, scopeSize - 1, 0, width),
                                  juce::jmap (scopeData[i - 1], 0.f, 1.f, (float) height, 0.f),
                          (float) juce::jmap (i, 0, scopeSize - 1, 0, width),
                                  juce::jmap (scopeData[i], 0.f, 1.f, (float) height, 0.f) });
        }
    }
    
    enum {
        fftOrder = 11, // fftOrder ; # bins = 2^fftOrder
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

private:
    
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    
    float fifo [fftSize];
    float fftData [2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData [scopeSize];
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalyserComponent)
};
