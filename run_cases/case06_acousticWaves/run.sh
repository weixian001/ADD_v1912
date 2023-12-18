#!/bin/bash

rm -rf dynamicCode/
dualZoneBuoyantPimpleFoam > log
tail log
