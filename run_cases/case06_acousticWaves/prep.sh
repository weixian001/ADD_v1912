#!/bin/bash

blockMesh > /dev/null
splitMeshRegions -cellZonesOnly -overwrite > /dev/null
topoSet -region linearZone > /dev/null

rm -r 0
cp -r 0.orig 0
