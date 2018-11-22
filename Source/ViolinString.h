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
    
    void setFrequency (double freq);
    void setFingerPoint (double val) { fp = val; };
    void setFingerForce (double val) { ff = val; };
    void setFingerOn (bool val) { fingerOn = val; };
    
    void setBow (bool val) { isBowing = val; };
    void setVb (double val) { Vb = val; }
    void setFb (double val) { Fb = val; }
    void setBowPos (double val) { bp = floor (val * N); }

private:

    double fs, freq, gamma, k, s0, s1, B, kappa, h, N, lambdaSq, muSq, kOh, gOh, a, BM, Vb, Fb, pickup, tol, q, qPrev, bp, b, eps, fp, ff;
    bool isBowing = false;
    bool fingerOn = false;
    std::vector<double> u, uPrev, uNext;
    bool active = false;
    
    unsigned long count;
    unsigned long t = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViolinString)
};
