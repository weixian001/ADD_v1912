#!/bin/bash

rm -rf postProcessing/
rm -rf dynamicCode/

dualZoneEulerFoam $1 > log
tail log
