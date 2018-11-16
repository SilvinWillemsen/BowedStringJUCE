/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ViolinString.h"
#include "SenselWrapper.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent,
                      public Timer
{
  public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(Graphics &g) override;
    void resized() override;
    void mouseDown(const MouseEvent &e) override;
    void mouseUp(const MouseEvent &e) override;
    void mouseDrag(const MouseEvent &e) override;
    void timerCallback() override; 
  private:
    //==============================================================================
    double fs;
    double bufferSize;
    Sensel sensel;
    
    float force = 0.0;
    float xpos = 0.0f;
    float ypos = 0.0f;

    OwnedArray<ViolinString> violinStrings;
    int numStrings;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
