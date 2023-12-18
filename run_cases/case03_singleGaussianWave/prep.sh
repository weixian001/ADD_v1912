#!/bin/bash

blockMesh > /dev/null
splitMeshRegions -cellZonesOnly -overwrite > /dev/null

#rm -rf dynamicCode/
rm -r 0
cp -r 0.orig 0
