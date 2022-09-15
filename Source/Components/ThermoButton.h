/*
  ==============================================================================

    ThermoButton.h
    Created: 15 Aug 2022 12:23:57pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ThermoButton  : public juce::ToggleButton
{
public:
    ThermoButton()
    {
        setToggleable(true);
        setClickingTogglesState(true);
        
        setLookAndFeel(&lnf);
        
    }

    ~ThermoButton() override
    {
        setLookAndFeel(nullptr);
    }

    void paint (juce::Graphics& g) override
    {
        juce::ToggleButton::paint (g);
    }

    void mouseEnter (const juce::MouseEvent& event) override
    {
        grabKeyboardFocus();
    }
    
    void mouseExit (const juce::MouseEvent& event) override
    {
        giveAwayKeyboardFocus();
    }
private:
    
    ThermoLnF lnf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThermoButton)
};
