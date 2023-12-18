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

#include "DRP9.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
	defineTypeNameAndDebug(DRP9, 0);
	addToRunTimeSelectionTable(spatialScheme, DRP9, word);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::DRP9::DRP9()
:
	spatialScheme()
{
	// Initialize alpha with finite-difference coefficients
	alpha_.setSize(4);
	alpha_[0] =  0.8301178834769906875382633360472085;
	alpha_[1] = -0.23175338776901819008451262109655756;
	alpha_[2] =  0.05287205020483696423592156502901203;
	alpha_[3] = -6.306814638366300019250697235282424E-3;
	
	spatialScheme::convertAlpha();
}

// ************************************************************************* //


// ************************************************************************* //
