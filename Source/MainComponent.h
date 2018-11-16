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
                      public Timer,
                      public KeyListener
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

  virtual bool keyPressed(const KeyPress &key, Component *originatingComponent) override;
  virtual bool keyStateChanged(bool isKeyDown, Component *originatingComponent) override;

private:
  //==============================================================================
  double fs;
  double bufferSize;
  Sensel sensel;

  float minOut;
  float maxOut;
  OwnedArray<ViolinString> violinStrings;

  int numStrings;
  int octave;
  vector<const char> keys = {'A', 'W', 'S', 'E', 'D', 'F', 'T', 'G', 'Y', 'H', 'U', 'J', 'K'};

  vector<ViolinString *> activeStrings;
  int polyphony;
  int currentPoly = 0;

  float force = 0.0;
  float xpos = 0.0f;
  float ypos = 0.0f;
  double finger2X = 0.0;
  double finger2Force = 0.0;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
