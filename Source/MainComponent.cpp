/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent() : minOut(-1.0), maxOut(1.0), polyphony(5)
{
    // Make sure you set the size of the component after
    // you add any child components.

    setSize(800, 600);

    // specify the number of input and output channels that we want to open
    setAudioChannels(0, 2);
    startTimer(1000.0/150.0);
    setWantsKeyboardFocus(true);
    addKeyListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    fs = sampleRate;
    bufferSize = samplesPerBlockExpected;

    for (int i = 0; i < numStrings; ++i)
    {
        //ViolinString* newString = new ViolinString (196.0 * (i + 2) * 0.95, fs);
        violinStrings.add(new ViolinString(196.0 * (i + 1) * 0.95, fs));
        violinStrings[i]->activate();
    }

    activeStrings.resize(polyphony, violinStrings[0]);

    for (int i = 0; i < amountOfSensels; i++)
        sensels.add(new Sensel(i)); // chooses the device in the sensel device list
}

void MainComponent::hiResTimerCallback()
{
    /*// check sensel
    sensel.check();
    unsigned int size = sensel.contactAmount;

    double maxVb = 0.2;
    double Vb = 0.0;
    double Fb = force * 100;

    double finger2X = 0.0;
    double finger2Force = 0.0;
    bool state1 = false;
    bool state2 = false;
    
    for (int f = 0; f < size; f++)
    {
        ///</if (sensel.mFingers[f].state.load())
        //    cout << "Finger[" << f << "] ID: " << sensel.mFingers[f].fingerID.load() << "\n";

        
        int index1 = sensel.fingers[f].fingerID;
        if (index1 == 0) // first press is indexed 0
        {
            state1 = sensel.fingers[f].state;
            xpos = sensel.fingers[f].x;
            ypos = sensel.fingers[f].y;
            
            force = sensel.fingers[f].force * 10;

            Vb = sensel.fingers[f].delta_y * maxVb;
            Fb = force * 100;

            //cout << "Finger[" << 0 << "] ID: " << sensel.mFingers[0].fingerID.load() << "\n";
            //cout << "Finger[" << 0 << "] delta x: " << sensel.mFingers[0].delta_x.load() << "\n";
            //secout << "Finger[" << 0 << "] delta y: " << sensel.mFingers[0].delta_y.load() << "\n";
        }

        int index2 = sensel.fingers[f].fingerID;
        
        
        
        if (index2 == 1)
        {
            state2 = sensel.fingers[f].state;
            finger2X = sensel.fingers[f].x;
            finger2Force = sensel.fingers[f].force;

            //violinStrings[0]->setFingerOn(true);
        }

        //cout << "Finger[" << 0 << "] ID: " << sensel.mFingers[0].fingerID.load() << "\n";
    }*/
    double maxVb = 0.2;
    double Vb = 0.0;
    double Fb = force * 100;

    double finger2X = 0.0;
    double finger2Force = 0.0;
    bool state1 = false;
    bool state2 = false;

    for (auto sensel : sensels)
    {
        sensel->check();
        
        unsigned int fingerCount = sensel->contactAmount;
        
        for (int f = 0; f < fingerCount; f++)
            if (sensel->fingers[f].state)
                cout << "Sensel[" << sensel->senselIndex << "] Finger ID: " << sensel->fingers[f].fingerID << "\n";

    }
    for (auto string : violinStrings)
    {
        string->setBow(state1);
        string->setVb(Vb);
        string->setFb(Fb);
        string->setBowPos(xpos);
        string->setFingerPoint(finger2X);
        string->setFingerForce(finger2Force);
        string->setFingerOn(state2);
    }
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
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
                }
                else if (output < minOut)
                {
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
void MainComponent::paint(Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    for (int i = 0; i < numStrings; ++i)
    {
        g.setColour(Colour::fromRGB(50 + i * 200.0 / static_cast<double>(numStrings), 0, 0));
        g.fillRect(round(i * getWidth() / static_cast<double>(numStrings)), 0, round(getWidth() / static_cast<double>(numStrings)), getHeight());
        g.setColour(Colours::grey);
        g.drawRect(round(i * getWidth() / static_cast<double>(numStrings)), 0, round(getWidth() / static_cast<double>(numStrings)), getHeight(), 2);
    }
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::mouseDown(const MouseEvent &e)
{
    for (int j = 0; j < numStrings; ++j)
    {
        violinStrings[j]->setBow(true);
    }
    float scaledEX = e.x / static_cast<double>(getWidth());
    int idx = floor(scaledEX * static_cast<double>(numStrings));
    violinStrings[idx]->setBow(true);
    violinStrings[idx]->activate();
}

void MainComponent::mouseDrag(const MouseEvent &e)
{
    double maxVb = 0.2;
    double Vb = (e.y - getHeight() * 0.5) / (static_cast<double>(getHeight() * 0.5)) * maxVb;
    double Fb = e.x / (static_cast<double>(getWidth())) * 100;
    for (int j = 0; j < numStrings; ++j)
    {
        violinStrings[j]->setVb(Vb);
        violinStrings[j]->setFb(Fb);
    }
}

void MainComponent::mouseUp(const MouseEvent &e)
{
    for (int j = 0; j < numStrings; ++j)
    {
        violinStrings[j]->setBow(false);
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
                activeStrings[currentPoly % polyphony] = violinStrings[idx];
                ++currentPoly;
            }
        }
        else
        {
            violinStrings[i + octave]->setBow(false);
        }
    }
    return false;
}
