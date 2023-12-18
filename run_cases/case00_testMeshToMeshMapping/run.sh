#!/bin/bash

np=0
for i in processor*/
do
    np=`echo $np+1 | bc`
done
mpirun -np $np Test-meshToMesh -parallel > log.sol
reconstructPar -allRegions -withZero > log.rec
