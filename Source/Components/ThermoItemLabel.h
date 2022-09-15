/*
  ==============================================================================

    ThermoItemLabel.h
    Created: 24 Aug 2022 8:44:56am
    Author:  Dan Hirlinger

  ==============================================================================
 
 Label to display the name / type of component.
*/

class ThermoItemLabel : public juce::Label
{
public:
    ThermoItemLabel()
    {
        setJustificationType(Justification::centred);
        setEditable(false);
        setLookAndFeel(&lnf);
    };

    ~ThermoItemLabel(){
        setLookAndFeel(nullptr);
    };

    void paint (juce::Graphics& g) override
    {
        juce::Label::paint (g);
    }

    void mouseEnter (const juce::MouseEvent& event) override
    {
        grabKeyboardFocus();
    }
    
    
private:
    ThermoLnF lnf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThermoItemLabel)
};


#pragma once
