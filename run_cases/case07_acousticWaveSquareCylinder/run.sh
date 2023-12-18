#!/bin/bash

dualZoneBuoyantPimpleFoam $1 >log
tail log
