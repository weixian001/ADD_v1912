#!/bin/bash

foamDictionary -entry startTime -set 1000 system/controlDict >/dev/null
foamDictionary -entry endTime -set 1100 system/controlDict >/dev/null
foamDictionary -entry writeInterval -set 10 system/controlDict >/dev/null
foamDictionary -entry deltaT -set 1e-2 system/controlDict >/dev/null

foamDictionary -entry interfaceInterpolation.patches -set '(interfaceL interfaceR)' system/linearZone/fvSchemes > /dev/null

setFields -region linearZone > /dev/null

cp 0.post/linearZone/* 1000/linearZone/

scalarFlds=(p T)
patches=(interfaceL interfaceR)
for patch in ${patches[@]}
do
	# Correct interface conditions for p & T
	for fld in ${scalarFlds[@]}
	do
		dict=`echo 1000/nonLinearZone/$fld`
		foamDictionary -entry boundaryField.$patch.type          -set mixed       $dict >/dev/null
		foamDictionary -entry boundaryField.$patch.refValue      -set 'uniform 1' $dict >/dev/null
		foamDictionary -entry boundaryField.$patch.refGradient   -set 'uniform 1' $dict >/dev/null
		foamDictionary -entry boundaryField.$patch.valueFraction -set 'uniform 0' $dict >/dev/null
	done
	
	# Correct interface conditions for U
	dict=`echo 1000/nonLinearZone/U`
	foamDictionary -entry boundaryField.$patch.type          -set mixed             $dict >/dev/null
	foamDictionary -entry boundaryField.$patch.refValue      -set 'uniform (0 0 0)' $dict >/dev/null
	foamDictionary -entry boundaryField.$patch.refGradient   -set 'uniform (0 0 0)' $dict >/dev/null
	foamDictionary -entry boundaryField.$patch.valueFraction -set 'uniform 0'       $dict >/dev/null
	
done
