/*
  ==============================================================================

    ViolinString.cpp
    Created: 14 Nov 2018 3:10:58pm
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ViolinString.h"

//==============================================================================
ViolinString::ViolinString (double freq, double fs) : fs (fs), freq (freq)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    gamma = freq * 2;                       // Wave speed
    k = 1 / fs;                             // Time-step
    
    s0 = 1;                               // Frequency-independent damping
    s1 = 0.005;                             // Frequency-dependent damping

    B = 0.0001;                             // Inharmonicity coefficient
    kappa = sqrt (B) * (gamma / double_Pi); // Stiffness Factor
    
    // Grid spacing
    h = sqrt ((gamma * gamma * k * k + 4.0 * s1 * k
              + sqrt (pow (gamma * gamma * k * k + 4.0 * s1 * k, 2.0)
                     + 16.0 * kappa * kappa * k * k)) * 0.5);
    
    N = floor (1.0 / h);                    // Number of gridpoints
    h = 1.0 / N;                            // Recalculate gridspacing

    // Initialise vectors
    u.resize (N);
    uPrev.resize (N);
    uNext.resize (N);
    
    // Courant numbers
    lambdaSq = pow (gamma * k / h, 2);
    muSq = pow (k * kappa / (h * h), 2);

    kOh = (kappa * kappa) / (h * h);
    gOh = (gamma * gamma) / (h * h);
    // Bow Model
    a = 100;                                // Free parameter
    BM = sqrt(2 * a) * exp(0.5);

    
    _Vb.store(0.2);                               // Bowing speed
    _Fb.store(50);                                // Bowing force / total mass of bow
    pickup = floor(3 * N / 4);                    // Pickup position
    
    // Initialise variables for Newton Raphson
    tol = 1e-4;
    qPrev = 0;
    
    _bp.store(floor (N / 4.0));
    
}

void ViolinString::setFrequency (double freq)
{
    gamma = freq * 2;                       // Wave speed
   // we dont want to set the kappa according to the gamma 
//    kappa = sqrt (B) * (gamma / double_Pi); // Stiffness Factor
    
    // Grid spacing
    h = sqrt ((gamma * gamma * k * k + 4.0 * s1 * k
               + sqrt (pow (gamma * gamma * k * k + 4.0 * s1 * k, 2.0)
                       + 16.0 * kappa * kappa * k * k)) * 0.5);

    N = floor (1.0 / h);                    // Number of gridpoints
    h = 1.0 / N;                            // Recalculate gridspacing
    N = 100;
    
    // Courant numbers
    lambdaSq = pow (gamma * k / h, 2);
    muSq = pow (k * kappa / (h * h), 2);
    
    kOh = (kappa * kappa) / (h * h);
    gOh = (gamma * gamma) / (h * h);
}

ViolinString::~ViolinString()
{
}

void ViolinString::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

}

void ViolinString::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

double ViolinString::bow()
{
    double Fb = _Fb.load();
    int bp = _bp.load();
    bool isBowing = _isBowing.load();
    newtonRaphson();
    double excitation = k * k * (1 / h) * Fb * BM * q * exp (-a * q * q);
    for (int l = 2; l < N - 2; ++l)
    {
        uNext[l] = (2 * u[l] - uPrev[l] + lambdaSq * (u[l+1] - 2 * u[l] + u[l-1])
                    - muSq * (u[l+2] - 4 * u[l+1] + 6 * u[l] - 4 * u[l-1] + u[l-2])
                    + s0 * k * uPrev[l]
                    + (2 * s1 * k) / (h * h) * ((u[l+1] - 2 * u[l] + u[l-1])
                                                - (uPrev[l+1] - 2 * uPrev[l] + uPrev[l-1]))) / (1 + s0 * k);
        if (l == bp && isBowing == true)
        {
            uNext[l] = uNext[l] - excitation;
        }
    }
//    if (fingerOn)
//    {
//        int fingerPos = floor(fp * N);
//        if (ff > 1)
//        {
//            std::cout << "wait" << std::endl;
//        }
//        uNext[fingerPos] = uNext[fingerPos] * ff;
//    }
    
    uPrev = u;
    u = uNext;
    return uNext[pickup];
}

void ViolinString::newtonRaphson()
{
    double Vb = _Vb.load();
    double Fb = _Fb.load();
    int bp = _bp.load();
    
    b = 2.0 / k * Vb - (2.0 / (k * k)) * (u[bp] - uPrev[bp])
    - gOh * (u[bp + 1] - 2 * u[bp] + u[bp - 1])
    + kOh * (u[bp + 2] - 4 * u[bp + 1] + 6 * u[bp] - 4 * u[bp - 1] + u[bp - 2])
    + 2 * s0 * Vb
    - (2 * s1 / (k * h * h)) * ((u[bp + 1] - 2 * u[bp] + u[bp - 1])
                                - (uPrev[bp + 1] - 2 * uPrev[bp] + uPrev[bp - 1]));
    eps = 1;
    int i = 0;
    while (eps>tol)
    {
        q=qPrev - (Fb * BM * qPrev * exp (-a * qPrev * qPrev) + 2 * qPrev / k + 2 * s0 * qPrev + b)
            / (Fb * BM * (1 - 2 * a * qPrev * qPrev) * exp (-a * qPrev * qPrev) + 2 / k + 2 * s0);
        eps = std::abs (q - qPrev);
        qPrev = q;
        ++i;
        if (i > 10000)
        {
            std::cout << "Nope" << std::endl;
        }
    }
}
