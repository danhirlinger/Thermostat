/*
  ==============================================================================

    TempSlider.h
    Created: 5 Aug 2022 4:22:05pm
    Author:  Dan Hirlinger

  ==============================================================================
*/

#pragma once


class ThermoLnF : public LookAndFeel_V4
{
public:
    
    ThermoLnF(){};
    ~ThermoLnF() = default;
    
    
    
    // === OVERRIDE FUNCTIONS ====
    void drawRotarySlider (juce::Graphics& g,
                           int x, int y,
                           int width, int height,
                           float sliderPos,
                           const float rotaryStartAngle,
                           const float rotaryEndAngle,
                           juce::Slider &slider) override
    {
        auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width  * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour (knobCFill);
        g.fillEllipse (rx, ry, rw, rw);

        // outline
        g.setColour (knobCBorder);
        g.drawEllipse (rx, ry, rw, rw, 2.5f);
        //inside outline
        g.setColour (knobCInsideBorder);
        g.drawEllipse (rx + 1, ry + 1, rw - 2, rw - 2, 0.75f);
        
        // tic markers on outline
        g.setColour (knobCTics);
        juce::Path tic;
        juce::PathStrokeType sTic {2.f, PathStrokeType::curved};
        tic.clear();
        
        auto sliderRange = rotaryEndAngle - rotaryStartAngle;
        auto numTics = 10;
        auto ticDist = sliderRange / numTics;
        auto ticSize = ticDist / 3.f;
        for (float n = rotaryStartAngle ; n < rotaryEndAngle + .1 ; n = n + ticDist)
        {
            tic.addArc(rx, ry, rw, rw, n - ticSize/2, n + ticSize/2, true);
            g.strokePath(tic, sTic);
        };

        // knob pointer
        juce::Path p;
        float pointerLen = 0.f;
        auto pointerLength = radius * pointerLen;
        auto pointerThickness = 6.0f;
        p.addTriangle(0, -radius + 5, -pointerThickness, pointerLength, pointerThickness, pointerLength);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
        juce::PathStrokeType stroke {2.f, PathStrokeType::JointStyle::mitered};

        g.setColour (knobCPointer);
        g.strokePath(p, stroke);
    }
    
    
    void drawToggleButton (Graphics &g,
                           ToggleButton &button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override
    {

        auto width = button.getWidth();
        auto height = button.getHeight();
        
        auto cushion = 3;
        
        // fill
        g.setColour(buttonSlideCFill);
        g.fillRoundedRectangle(0, 0, width, height, height/2);

        // outline
        g.setColour(buttonSlideCOutline);
        g.drawRoundedRectangle(0, 0, width, height, height/2, 1.f);
        
        if (button.getToggleState() == true)
        {
            g.setColour(buttonSlideCKnobOn);
            g.fillEllipse(cushion, cushion, width/2 - cushion*2, height - cushion*2);
        }
        if (button.getToggleState() == false)
        {
            g.setColour(buttonSlideCKnobOff);
            g.fillEllipse(cushion+(width/2), cushion, width/2 - cushion*2, height - cushion*2);
        }
    }
    
    
    
    
    void drawComboBox (Graphics &g,
                  int width, int height,
                  bool isButtonDown,
                  int buttonX, int buttonY,
                  int buttonW, int buttonH,
                  ComboBox &comboBox) override
    {
        // outline
        g.setColour(juce::Colours::blue);
        g.drawRect(0,0,width,height);
        
        // drop down triangle
        Path p;
        float sF = 8;
        float offset = 5;
        Point<float> p1 { (float)(offset + buttonX + sF), (float)(buttonY + sF) }; // 0,0
        Point<float> p2 { (float)(offset + buttonX + buttonW - sF), (float)(buttonY + sF)};
        Point<float> p3 { (float)(offset + buttonX + (buttonW/2)), (float)(buttonY + buttonH - sF/2) };
        
        p.addTriangle(p1, p2, p3);
        g.setColour(juce::Colours::grey);
        g.fillPath(p);
        
//        g.setColour(juce::Colours::black);
//        g.setFont(getComboBoxFont(comboBox));
//        g.drawText(comboBox.getText(), 5, 0, width*.75, height, Justification::centredLeft);
        
    }
    Font getComboBoxFont(ComboBox &comboBox) override
    {
        Font font { "Digital-7", 11.f, Font::FontStyleFlags::plain};
        return font;
    }
    Font getPopupMenuFont() override
    {
        Font font { "Digital-7", 11.f, Font::FontStyleFlags::plain };
        return font;
    }
    
    
    
    
    
    void drawTextEditorOutline(Graphics &g,
                               int width, int height,
                               TextEditor &editor) override
    {

        g.setColour(valueCOutline);
        g.setOpacity(0.4);
        g.drawRect(0,0,width,height);
    }
    void fillTextEditorBackground(Graphics &g,
                                  int width,
                                  int height,
                                  TextEditor &textEditor) override
    {
        // function does nothing, makes background transparent
    }
    
    
    
    
    
    
    void drawLabel(Graphics &g,
                   Label &label) override
    {
        
        auto w = label.getWidth();
        auto h = label.getHeight();
        
        Font font { "Digital-7", 12.f, Font::FontStyleFlags::plain };
        
        g.setColour(juce::Colours::black);
        g.setFont(font);
        g.drawText(label.getText(), 0, 0, w, h, Justification::centred);
        
    }
    
    // ==================================
    
    
    // === COLOUR SETTING FUNCTIONS ===
    void setKnobColours(Colour newCKnobFill,
                        Colour newCKnobBorder,
                        Colour newCKnobInsideBorder,
                        Colour newCKnobPointer,
                        Colour newCKnobCTics)
    {
        knobCFill = newCKnobFill;
        knobCBorder = newCKnobBorder;
        knobCInsideBorder = newCKnobInsideBorder;
        knobCPointer = newCKnobPointer;
        knobCTics = newCKnobCTics;
    }
    
    void setButtonColours(Colour newButtonSlideCFill,
                          Colour newButtonSlideCOutline,
                          Colour newButtonSlideCKnobOn,
                          Colour newButtonSlideCKnobOff)
    {
        buttonSlideCFill = newButtonSlideCFill;
        buttonSlideCOutline = newButtonSlideCOutline;
        buttonSlideCKnobOn =  newButtonSlideCKnobOn;
        buttonSlideCKnobOff = newButtonSlideCKnobOff;
    }
    
    // ===============================================
private:
    
    // COLOURS
    Colour knobCFill = juce::Colours::black;
    Colour knobCBorder = juce::Colours::black;
    Colour knobCInsideBorder = juce::Colours::blue;
    Colour knobCPointer = juce::Colours::white;
    Colour knobCTics = juce::Colours::white;
    
    Colour buttonSlideCFill = juce::Colours::black;
    Colour buttonSlideCOutline = juce::Colours::grey;
    Colour buttonSlideCKnobOn =  juce::Colours::red;
    Colour buttonSlideCKnobOff = juce::Colours::darkred;
    
    Colour valueCOutline = juce::Colours::black;
    
    
    
    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThermoLnF);
};
