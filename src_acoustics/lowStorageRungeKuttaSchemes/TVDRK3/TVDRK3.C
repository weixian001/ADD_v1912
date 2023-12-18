/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2011 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                            | Copyright (C) 2019 U S Vevek, NTU, Singapore
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "TVDRK3.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
	defineTypeNameAndDebug(TVDRK3, 0);
	addToRunTimeSelectionTable(lowStorageRungeKuttaScheme, TVDRK3, word);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::TVDRK3::TVDRK3
(
	Time& time
)
:
	lowStorageRungeKuttaScheme(time)
{
	this->lowStorageRungeKuttaScheme::nStages_ = 3;
	
	scalarField& alpha = lowStorageRungeKuttaScheme::alpha_;
	alpha.setSize(3);
	alpha[0] = 0.0;
	alpha[1] = 0.75;
	alpha[2] = 1.0/3.0;
	
	scalarField& beta = lowStorageRungeKuttaScheme::beta_;
	beta = 1.0 - alpha;
	
	scalarField& c = lowStorageRungeKuttaScheme::c_;
	c.setSize(4);
	c[0] = 0.0;
	c[1] = 0.25;
	c[2] = 2.0/3.0;
	c[3] = 1.0;
	
	lowStorageRungeKuttaScheme::checkCoeffsSize();
}

// * * * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * //

#define makeAdvanceType(vfType)                                               \
void Foam::TVDRK3::advance                                                    \
(                                                                             \
	vfType& u                                                                 \
) const                                                                       \
{                                                                             \
	const fvMesh& mesh = u.mesh();                                            \
	                                                                          \
	vfType& uTmp = mesh.lookupObjectRef<vfType>(u.name() + "Tmp");            \
	                                                                          \
	const vfType& F = mesh.lookupObject<vfType>(u.name() + "Flux");           \
	                                                                          \
	if (s() == 0)                                                             \
	{                                                                         \
		uTmp.primitiveFieldRef() = u.primitiveField();                        \
		                                                                      \
		u.primitiveFieldRef() += time().deltaTValue()*F.primitiveField();     \
	}                                                                         \
	else                                                                      \
	{                                                                         \
		u.primitiveFieldRef() *= beta()[s()];                                 \
		                                                                      \
		u.primitiveFieldRef()                                                 \
			+= alpha()[s()]*uTmp.primitiveField()                             \
			+ beta()[s()]*time().deltaTValue()*F.primitiveField();            \
	}                                                                         \
}

makeAdvanceType(volScalarField);
makeAdvanceType(volVectorField);

#undef makeAdvanceType

// ************************************************************************* //


// ************************************************************************* //
