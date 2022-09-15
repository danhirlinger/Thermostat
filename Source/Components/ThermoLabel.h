/*
  ==============================================================================

    ThermoLabel.h
    Created: 16 Aug 2022 6:07:58pm
    Author:  Dan Hirlinger

  ==============================================================================
 
 Label that links with a given component to display its value
 
*/
//
class ThermoLabel : public juce::TextEditor
{
public:
    ThermoLabel()
    {
//        setMultiLine(false);
        setPopupMenuEnabled(false);
        Font font { "Digital-7", 14.f, Font::FontStyleFlags::plain };
        setFont(font);
        juce::String textAllowed = ". 0 1 2 3 4 5 6 7 8 9";
        setInputRestrictions(5, textAllowed);
        setJustification (juce::Justification::centred);

        setLookAndFeel(&lnf);
    };

    ~ThermoLabel(){
        setLookAndFeel(nullptr);
    };

    void paint (juce::Graphics& g) override
    {
        juce::TextEditor::paint (g);
    }

    void mouseEnter (const juce::MouseEvent& event) override
    {
        grabKeyboardFocus();
    }

    void returnPressed() override
    {
        giveAwayKeyboardFocus();
    }
    
    
private:
    ThermoLnF lnf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThermoLabel)
};
