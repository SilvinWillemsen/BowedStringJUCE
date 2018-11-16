/*
  ==============================================================================

    ViolinString.h
    Created: 14 Nov 2018 3:10:58pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ViolinString    : public Component
{
public:
    ViolinString(double freq, double fs);
    ~ViolinString();

    void paint (Graphics&) override;
    void resized() override;
    
    double bow();
    
    void newtonRaphson();
    double getOutput();
    
    void setFrequency(double freq);
    void setBow (bool val) { isBowing = val; };
    void setVb (double val) { Vb = val; }
    void setFb (double val) { Fb = val; }

private:

    double fs, freq, gamma, k, s0, s1, B, kappa, h, N, lambdaSq, muSq, kOh, gOh, a, BM, Vb, Fb, pickup, tol, q, qPrev, bp, b, eps;
    bool isBowing = false;
    std::vector<double> u, uPrev, uNext;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViolinString)
};
