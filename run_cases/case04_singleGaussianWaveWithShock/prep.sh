#!/bin/bash

blockMesh > /dev/null
splitMeshRegions -cellZonesOnly -overwrite > /dev/null
topoSet -region linearZone > /dev/null

rm -rf 0
cp -r 0.orig 0

cp system/setFieldsDict.master system/linearZone/setFieldsDict
setFields -region linearZone > /dev/null

cp system/setFieldsDict.master system/nonLinearZone/setFieldsDict
setFields -region nonLinearZone > /dev/null
