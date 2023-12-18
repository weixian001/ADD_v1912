// Pressure wave properties
const vector kp(1.0, 0.0, 0.0);                  // orientation
const vector xp(-150, 0.0, 0.0);                 // location
const scalar ep(0.0);                            // strength
const scalar ap(Foam::log(2.0)/Foam::sqr(10.0)); // width

// Mean properties
const scalar pMean(1e5), TMean(300);
