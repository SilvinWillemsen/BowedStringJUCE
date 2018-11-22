/*
  ==============================================================================

    ViolinString.h
    Created: 14 Nov 2018 3:10:58pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace std;
//==============================================================================
/*
*/
class ViolinString : public Component
{
public:
  ViolinString(double freq, double fs);
  ~ViolinString();

  void paint(Graphics &) override;
  void resized() override;

  double bow();

  void newtonRaphson();
  double getOutput();

  void setFrequency(double freq);
  void setFingerPoint(double val) { fp = val; };
  void setFingerForce(double val) { ff = val; };
  //void setFingerOn (bool val) { fingerOn = val; };

  void setBow(bool val) { _isBowing.store(val); };
  void setVb(double val) { _Vb.store(val); }
  void setFb(double val) { _Fb.store(val); }
  void setBowPos(double val) { _bp.store(floor(val * N)); }

private:
  double fs, freq, gamma, k, s0, s1, B, kappa, h, N, lambdaSq, muSq, kOh, gOh, a, BM, pickup, tol, q, qPrev, b, eps, fp, ff;
  atomic<double> _Vb, _Fb, _bp;
  atomic<bool> _isBowing{false};

  vector<double> u, uPrev, uNext;
  bool active = false;

  unsigned long count;
  unsigned long t = 0;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ViolinString)
};
