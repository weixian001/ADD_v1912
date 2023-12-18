#!/bin/bash

blockMesh > /dev/null
splitMeshRegions -cellZonesOnly -overwrite > /dev/null
topoSet -region linearZone > /dev/null

rm -rf 0
cp -r 0.orig 0

foamDictionary -entry startTime -set 0 system/controlDict >/dev/null
foamDictionary -entry endTime -set 1000 system/controlDict >/dev/null
foamDictionary -entry writeInterval -set 1000 system/controlDict >/dev/null
foamDictionary -entry deltaT -set 2e-2 system/controlDict >/dev/null

foamDictionary -entry interfaceInterpolation.patches -remove system/linearZone/fvSchemes
