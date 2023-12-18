// Pressure wave properties
const vector kp(1.0, 0.0, 0.0);                  // orientation
const vector xp(-50.0, 0.0, 0.0);                 // location
const scalar ep(1e-2);                          // strength
const scalar ap(Foam::log(2.0)/Foam::sqr(12.0)); // width

// Mean properties
const scalar pMean(1.0), TMean(1.0);

// Ma=0.75
//const scalar pMean(0.60850057), TMean(3.3312095);
