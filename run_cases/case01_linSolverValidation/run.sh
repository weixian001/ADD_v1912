#!/bin/bash

dualZoneEulerFoam $1 >log.sol
tail log.sol
