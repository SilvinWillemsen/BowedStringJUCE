/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once
#include "sys/time.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ViolinString.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public KeyListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    // Mouse functions
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    
    virtual bool keyPressed (const KeyPress &key, Component *originatingComponent) override;
    virtual bool keyStateChanged (bool isKeyDown, Component *originatingComponent) override;

private:
    //==============================================================================
    double fs;
    double bufferSize;
    float minOut;
    float maxOut;
    OwnedArray<ViolinString> violinStrings;
    int numStrings;
    int octave;
    std::vector<const char> keys = {'A', 'W', 'S', 'E', 'D', 'F', 'T', 'G', 'Y', 'H', 'U', 'J', 'K'};
    
    std::vector<ViolinString*> activeStrings;
    int polyphony;
    int currentPoly = 0;
    
    double eYPrev = 0;
    long int timePrev = 0;;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
