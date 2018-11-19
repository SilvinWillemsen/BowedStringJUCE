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
class MainComponent   : public AudioAppComponent, public KeyListener,
                        private MidiInputCallback,
                        private MidiKeyboardStateListener
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

    void setMidiInput (int index);
    
    void logMessage (const String& m);
    
    static String getMidiMessageDescription (const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on "          + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off "         + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change "   + String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel "      + String (m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch "      + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + String (m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";
        
        if (m.isController())
        {
            String name (MidiMessage::getControllerName (m.getControllerNumber()));
            
            if (name.isEmpty())
                name = "[" + String (m.getControllerNumber()) + "]";
            
            return "Controller " + name + ": " + String (m.getControllerValue());
        }
        
        return String::toHexString (m.getRawData(), m.getRawDataSize());
    }

    
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;
    
    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    
    void postMessageToList (const MidiMessage& message, const String& source);
    
    void addMessageToList (const MidiMessage& message, const String& source);
   
    
    
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
    long int timePrev = 0;
    
    //MIDI
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    
    AudioDeviceManager deviceManager;           // [1]
    ComboBox midiInputList;                     // [2]
    Label midiInputListLabel;
    int lastInputIndex = 0;                     // [3]
    bool isAddingFromMidiInput = false;         // [4]
    
    TextEditor midiMessagesBox;
    double startTime;
    
    class IncomingMessageCallback   : public CallbackMessage
    {
    public:
        IncomingMessageCallback (MainComponent* o, const MidiMessage& m, const String& s)
        : owner (o), message (m), source (s)
        {}
        
        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList (message, source);
        }
        
        Component::SafePointer<MainComponent> owner;
        MidiMessage message;
        String source;
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
