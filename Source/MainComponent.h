/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ViolinString.h"
#include "../SenselWrapper/SenselWrapper.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent,
                      public HighResolutionTimer
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
  void hiResTimerCallback() override;
    
private:
  //==============================================================================
  double fs;
  double bufferSize;

  float minOut;
  float maxOut;
  OwnedArray<ViolinString> violinStrings;

  OwnedArray<Sensel> sensels;
  static const unsigned int amountOfSensels = 2;

  static const unsigned int numStrings = 2;

  array<float, amountOfSensels> force = {0.0};
  array<float, amountOfSensels> xpos = {0.0};
  array<float, amountOfSensels> ypos = {0.0};
  array<float, amountOfSensels> Vb = {0.0};
  array<float, amountOfSensels> Fb = {0.0};
  array<bool, amountOfSensels> state = {0};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
