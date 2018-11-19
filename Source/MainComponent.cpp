/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : minOut (-1.0), maxOut (1.0), numStrings (25), octave (0), polyphony (2),
    keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
    startTime (Time::getMillisecondCounterHiRes() * 0.001)
{
    // Make sure you set the size of the component after
    // you add any child components.

    setSize (800, 600);

    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    
    //MIDI
    addAndMakeVisible(keyboardComponent);
    keyboardState.addListener (this);
    
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    auto midiInputs = MidiInput::getDevices();
    midiInputList.addItemList (midiInputs, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };
    // find the first enabled device and use that by default
    for (auto midiInput : midiInputs)
    {
        if (deviceManager.isMidiInputEnabled (midiInput))
        {
            setMidiInput (midiInputs.indexOf (midiInput));
            break;
        }
    }
    // if no enabled devices were found just use the first one in the list
    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    fs = sampleRate;
    bufferSize = samplesPerBlockExpected;
    int test = 0;
    for (int i = test; i < numStrings + test; ++i)
    {
//        violinStrings.add(new ViolinString (196.0 * (i + 1) * (0.985 - (0.025 * pow(1.5, i))), fs));
        violinStrings.add(new ViolinString (118 * pow (2, i / 12.0) + 1, fs));
    }
    activeStrings.resize (polyphony, nullptr);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
//    bufferToFill.clearActiveBufferRegion(); //comment this out
    
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        float *const channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        
        if (channel == 0)
        {
            for (int i = 0; i < bufferToFill.buffer->getNumSamples(); i++)
            {
               
                float output = 0.0;
                for (int j = 0; j < numStrings; ++j)
                {
                    if (violinStrings[j]->isActive())
                    {
                        float stringSound = violinStrings[j]->bow() * 600;
                        output = output + stringSound;
                    }
                }
                
                if (output > maxOut)
                {
                    output = maxOut;
                } else if (output < minOut) {
                    output = minOut;
                }
                channelData[i] = output;
            }
        }
        else
        {
            memcpy(channelData,
                   bufferToFill.buffer->getReadPointer(0),
                   bufferToFill.numSamples * sizeof(float));
        }
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    for (int i = 0; i < numStrings; ++i)
    {
        g.setColour (Colour::fromRGB(50 + i * 200.0 / static_cast<double> (numStrings), 0, 0));
        g.fillRect(round(i * getWidth() / static_cast<double> (numStrings)), 0, round(getWidth() / static_cast<double> (numStrings)), getHeight());
        g.setColour(Colours::grey);
        g.drawRect(round(i * getWidth() / static_cast<double> (numStrings)), 0, round(getWidth() / static_cast<double> (numStrings)), getHeight(), 2);
    }
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
//    keyboardComponent.setBounds(0, 2.0 * getHeight() / 3.0, getWidth(), getHeight() / 3.0);
    auto area = getLocalBounds();
    
    midiInputList    .setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
    keyboardComponent.setBounds (area.removeFromBottom (80).reduced(8));
    midiMessagesBox  .setBounds (area.reduced (8));
}

void MainComponent::mouseDown(const MouseEvent &e)
{
    float scaledEX = e.x / static_cast<double> (getWidth());
    int idx = floor(scaledEX * static_cast<double> (numStrings));
    violinStrings[idx]->setBow (true);
    violinStrings[idx]->activate();
}

void MainComponent::mouseDrag(const MouseEvent &e)
{
    double maxVb = 0.2;
    double Vb = (e.y - getHeight() * 0.5) / (static_cast<double> (getHeight() * 0.5)) * maxVb;
    for (int j = 0; j < numStrings; ++j)
    {
        violinStrings[j]->setVb (Vb);
    }
    for (int j = 0; j < numStrings; ++j)
    {
        violinStrings[j]->setBowPos(e.x / static_cast<double> (getWidth()));
    }
}

void MainComponent::mouseUp(const MouseEvent &e)
{
    for (int j = 0; j < numStrings; ++j)
    {
        violinStrings[j]->setBow (false);
    }
}

bool MainComponent::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent)
{
    switch (key.getKeyCode())
    {
        case 'Z':
            octave = 0;
            break;
        case 'X':
            octave = 12;
            break;
    }
    return true;
}

bool MainComponent::keyStateChanged(bool isKeyDown, Component *originatingComponent)
{
    for (int i = 0; i < 13; i++)
    {
        char k = keys[i];
        int idx = i + octave;
        if (KeyPress::isKeyCurrentlyDown(k))
        {
            violinStrings[idx]->setBow(true);
            if (!violinStrings[idx]->isActive())
            {
                violinStrings[idx]->activate();
                bool inVector = false;
                for (int j = 0; j < polyphony; ++j)
                {
                    // If the currently pressed string is in the polyphony vector, do nothing
                    if (activeStrings[j] == violinStrings[idx])
                    {
                        inVector = true;
                        break;
                    }
                }
                if (!inVector)
                {
                    if (activeStrings[currentPoly % polyphony] != nullptr)
                    {
                        activeStrings[currentPoly % polyphony]->deactivate();
                    }
                    activeStrings[currentPoly % polyphony] = violinStrings[idx];
                    ++currentPoly;
                }
            }
        }
        else
        {
            violinStrings[i + octave]->setBow(false);
        }
    }
    return false;
}

void MainComponent::logMessage(const String &m)
{
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret (m + newLine);
}

void MainComponent::setMidiInput(int index)
{
    auto list = MidiInput::getDevices();
    deviceManager.removeMidiInputCallback (list[lastInputIndex], this);
    auto newInput = list[index];
    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);
    deviceManager.addMidiInputCallback (newInput, this);
    midiInputList.setSelectedId (index + 1, dontSendNotification);
    lastInputIndex = index;
}

void MainComponent::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message)
{
    const ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent (message);
    postMessageToList (message, source->getName());
}

void MainComponent::handleNoteOn(MidiKeyboardState *, int midiChannel, int midiNoteNumber, float velocity)
{
    if (midiNoteNumber >= 48 && midiNoteNumber <= 73)
    {
        int idx = midiNoteNumber - 48;
        violinStrings[idx]->setBow(true);
        if (!violinStrings[idx]->isActive())
        {
            violinStrings[idx]->activate();
            bool inVector = false;
            for (int j = 0; j < polyphony; ++j)
            {
                // If the currently pressed string is in the polyphony vector, do nothing
                if (activeStrings[j] == violinStrings[idx])
                {
                    inVector = true;
                    break;
                }
            }
            if (!inVector)
            {
                if (activeStrings[currentPoly % polyphony] != nullptr)
                {
                    activeStrings[currentPoly % polyphony]->deactivate();
                }
                activeStrings[currentPoly % polyphony] = violinStrings[idx];
                ++currentPoly;
            }
        }
    }
    
    if (! isAddingFromMidiInput)
    {
        auto m = MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList (m, "On-Screen Keyboard");
    }
}

void MainComponent::handleNoteOff(MidiKeyboardState *, int midiChannel, int midiNoteNumber, float velocity)
{
    if (midiNoteNumber >= 48 && midiNoteNumber <= 73)
    {
        int idx = midiNoteNumber - 48;
        violinStrings[idx]->setBow(false);
    }
    if (! isAddingFromMidiInput)
    {
        auto m = MidiMessage::noteOff (midiChannel, midiNoteNumber);
        m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList (m, "On-Screen Keyboard");
    }
}

void MainComponent::postMessageToList(const MidiMessage &message, const String &source)
{
    (new IncomingMessageCallback (this, message, source))->post();
}

void MainComponent::addMessageToList(const MidiMessage &message, const String &source)
{
    auto time = message.getTimeStamp() - startTime;
    
    auto hours   = ((int) (time / 3600.0)) % 24;
    auto minutes = ((int) (time / 60.0)) % 60;
    auto seconds = ((int) time) % 60;
    auto millis  = ((int) (time * 1000.0)) % 1000;
    
    auto timecode = String::formatted ("%02d:%02d:%02d.%03d",
                                       hours,
                                       minutes,
                                       seconds,
                                       millis);
    
    auto description = getMidiMessageDescription (message);
    
    String midiMessageString (timecode + "  -  " + description + " (" + source + ")"); // [7]
    logMessage (midiMessageString);
}
