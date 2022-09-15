/*
  ==============================================================================

    ThermoComboBox.h
    Created: 23 Aug 2022 10:46:41am
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once

class ThermoComboBox : public juce::ComboBox
{
public:
    ThermoComboBox()
    {
        setEditableText(false);
        setJustificationType(Justification::centredLeft);
        setLookAndFeel(&lnf);
    };

    ~ThermoComboBox(){
        setLookAndFeel(nullptr);
    };

    void paint (juce::Graphics& g) override
    {
        juce::ComboBox::paint (g);
    }

    void mouseEnter (const juce::MouseEvent& event) override
    {
        grabKeyboardFocus();
    }

    
private:
    ThermoLnF lnf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThermoComboBox)
};
