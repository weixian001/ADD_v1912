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

#include "LDDRK46.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
	defineTypeNameAndDebug(LDDRK46, 0);
	addToRunTimeSelectionTable(lowStorageRungeKuttaScheme, LDDRK46, word);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::LDDRK46::LDDRK46
(
	Time& time
)
:
	lowStorageRungeKuttaScheme(time)
{
	this->lowStorageRungeKuttaScheme::nStages_ = 6;
	
	scalarField& alpha = lowStorageRungeKuttaScheme::alpha_;
	alpha.setSize(6);
	alpha[0] =  0.0;
	alpha[1] = -0.737101392796;
	alpha[2] = -1.634740794341;
	alpha[3] = -0.744739003780;
	alpha[4] = -1.469897351522;
	alpha[5] = -2.813971388035;
	
	scalarField& beta = lowStorageRungeKuttaScheme::beta_;
	beta.setSize(6);
	beta[0] = 0.032918605146;
	beta[1] = 0.823256998200;
	beta[2] = 0.381530948900;
	beta[3] = 0.200092213184;
	beta[4] = 1.718581042715;
	beta[5] = 0.27;
	
	scalarField& c = lowStorageRungeKuttaScheme::c_;
	c.setSize(7);
	c[0] = 0.0;
	c[1] = 0.032918605146;
	c[2] = 0.249351723343;
	c[3] = 0.466911705055;
	c[4] = 0.582030414044;
	c[5] = 0.847252983783;
	c[6] = 1.0;
	
	lowStorageRungeKuttaScheme::checkCoeffsSize();
}

// ************************************************************************* //


// ************************************************************************* //
