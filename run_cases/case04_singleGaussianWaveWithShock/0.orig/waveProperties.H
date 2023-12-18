// Pressure wave properties
const vector kp(1.0, 0.0, 0.0);                  // orientation
const vector xp(-50.0, 0.0, 0.0);                // location
const scalar ep(1e-2);                           // strength
const scalar ap(Foam::log(2.0)/Foam::sqr(12.0)); // width

// Mean properties
// M = 1.0 (upstream)
const scalar pMean(1.0), TMean(1.0);
// M = 1.5
//const scalar pMean(2.45833333333), TMean(1.32021604938);
// M = 3.0
//const scalar pMean(10.3333333333), TMean(2.67901234567);
// M = 5.0
//const scalar pMean(29.0), TMean(5.8);

