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

#include "DRP11.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
	defineTypeNameAndDebug(DRP11, 0);
	addToRunTimeSelectionTable(spatialScheme, DRP11, word);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::DRP11::DRP11()
:
	spatialScheme()
{
	// Initialize alpha with finite-difference coefficients
	alpha_.setSize(5);
	alpha_[0] =  0.8691451973307874495001324546317289;
	alpha_[1] = -0.28182159562075193452800172953580692;
	alpha_[2] =  0.08707110821545964532454798738037454;
	alpha_[3] = -0.019510858728038347594594712914321635;
	alpha_[4] =  2.265620835298174792121178791209576E-3;
	
	spatialScheme::convertAlpha();
}

// ************************************************************************* //


// ************************************************************************* //
