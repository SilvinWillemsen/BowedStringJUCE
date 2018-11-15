/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : numStrings(1)
{
    // Make sure you set the size of the component after
    // you add any child components.

    setSize(800, 600);

    // specify the number of input and output channels that we want to open
    setAudioChannels(0, 2);

    startTimerHz(60);
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
        violinStrings.add(new ViolinString(196.0 * (i + 2) * 0.95, fs));
    }
}

void MainComponent::timerCallback()
{
    // check sensel
    sensel.check();

    float force = 0.0;
    float xpos = 0.0f;
    float ypos = 0.0f;
    
    
    if (sensel.mFingers[0].state.load())
    {
        xpos = sensel.mFingers[0].x.load() / 240.0f;
        ypos = sensel.mFingers[0].y.load() / 139.0f;
        force = (sensel.mFingers[0].force.load() / 8192.0f) * 10;
        double maxVb = 0.2;
        double Vb = fabs(0.5 - ypos) * 2 * maxVb; // / (static_cast<double> (getHeight() * 0.5)) * maxVb;
        double Fb = xpos * 100;
        for (auto string : violinStrings)
        {
            string->setBow(true);
            string->setVb(Vb);
            string->setFb(Fb);
        }
        
        //cout << "Finger[" << 0 << "] force: " << force * 1000 + 50 << "\n";
        //cout << "Finger[" << 0 << "] x: " << sensel.mFingers[0].x.load() << "\n";
        //cout << "Finger[" << 0 << "] y: " << sensel.mFingers[0].y.load() << "\n";
    } else
    {
        for (auto string : violinStrings)
            string->setBow(false);
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
                    output = output + violinStrings[j]->bow() * 1000;
                }
                output = output / numStrings;

                if (std::abs(output) > 1)
                {
                    std::cout << "wait" << std::endl;
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
