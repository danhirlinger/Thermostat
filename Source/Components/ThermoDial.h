/*
  ==============================================================================

    ThermoDial.h
    Created: 12 Aug 2022 2:25:47pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ApplianceLnF.h"

//==============================================================================
/*
*/
class ThermoDial : public juce::Slider
{
public:
    ThermoDial()
    {
        setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        setLookAndFeel(&lnf);
        
        setVelocityBasedMode (true);
        setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                             juce::MathConstants<float>::pi * 2.75f,
                             true);
        
        setWantsKeyboardFocus (true);
        
    }

    ~ThermoDial() override
    {
        setLookAndFeel (nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        juce::Slider::paint (g);
        
//        if (hasKeyboardFocus (true))
//            drawFocusMark (g, findColour(juce::Slider::textBoxOutlineColourId));
    }
    
    void mouseEnter (const juce::MouseEvent& event) override
    {
        grabKeyboardFocus();
    }
    
    void mouseExit (const juce::MouseEvent& event) override
    {
        giveAwayKeyboardFocus();
    }
    
    void mouseDown (const juce::MouseEvent& event) override
    {
        juce::Slider::mouseDown (event);
        
        setMouseCursor (juce::MouseCursor::NoCursor);
    }
    
    void mouseDrag (const juce::MouseEvent& event) override
    {
        juce::Slider::mouseDrag (event);
        
        if (event.mods.isShiftDown())
            setVelocityModeParameters (0.1, 1, 0.1, false);
        else
            setVelocityModeParameters (1.0, 1, 0.1, false);
    }
    
    void mouseUp (const juce::MouseEvent& event) override
    {
        juce::Slider::mouseUp (event);
        
        auto mms = juce::Desktop::getInstance().getMainMouseSource();
        mms.setScreenPosition (event.source.getLastMouseDownPosition());
        
        mouseUpTime = event.eventTime;
        
        setMouseCursor (juce::MouseCursor::NormalCursor);
    }
    
//    bool keyPressed (const juce::KeyPress& key) override
//    {
//        if ('0' <= key.getKeyCode() && key.getKeyCode() <= '9')
//        {
//            DialTextBox::valueShownWithEditor = juce::String::charToString (key.getTextCharacter());
//            showTextBox();
//            
//            return true;
//        }
//        
//        return false;
//    }
    
    void focusLost (FocusChangeType cause) override
    {
        // Avoid a bug that causes this component to lose focus when the mouse is released.
        auto currentMillisec = juce::Time::getCurrentTime().toMilliseconds();
        auto diffMillisec    = currentMillisec - mouseUpTime.toMilliseconds();
        
        if (diffMillisec < 50)
            grabKeyboardFocus();
    }
    
private:
    
    void drawFocusMark (juce::Graphics& g, juce::Colour colour)
    {
        g.setColour (colour);

        auto bounds = getLocalBounds().toFloat().reduced (3.0f);
        auto length = juce::jmin (bounds.getHeight(), bounds.getWidth()) * 0.07f;
        auto thick  = length * 0.38f;
        auto radian = 0.0f;

        auto topL    = bounds.getTopLeft();
        auto topR    = bounds.getTopRight();
        auto bottomR = bounds.getBottomRight();
        auto bottomL = bounds.getBottomLeft();

        std::array<juce::Point<float>, 4> corners { topL, topR, bottomR, bottomL };

        // Draw in clockwise order, starting from top left.
        for (auto corner : corners)
        {
            juce::Path path;

            path.startNewSubPath (corner.x + length, corner.y);
            path.lineTo          (corner.x,          corner.y);
            path.lineTo          (corner.x,          corner.y + length);

            g.strokePath (path,
                          juce::PathStrokeType (thick),
                          juce::AffineTransform::rotation (radian, corner.x, corner.y));

            radian += juce::MathConstants<float>::halfPi;
        };
    }

    juce::Time mouseUpTime;
    
    ThermoLnF lnf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThermoDial)
};
