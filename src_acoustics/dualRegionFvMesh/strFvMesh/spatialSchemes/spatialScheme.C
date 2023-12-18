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

#include "spatialScheme.H"

namespace Foam
{
	defineTypeNameAndDebug(spatialScheme, 0);
	defineRunTimeSelectionTable(spatialScheme, word);
}

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::spatialScheme::convertAlpha()
{
	const label n = alpha_.size();
	
	// Form upper-triangular matrix
	scalarSquareMatrix U(n, 0.0);
	
	for(register label r = 0; r < n; r++)
	{
		for(register label c = r; c < n; c++)
		{
			U[r][c] = factorial(2*c+1)/(factorial(r+c+1)*factorial(c-r));
			
			alpha_[r] += alpha_[c+1];
		}
	}
	
	// Solve for alpha
	LUsolve(U, alpha_);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::spatialScheme::spatialScheme
()
:
	alpha_()
{}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::spatialScheme>
Foam::spatialScheme::New
(
    const fvMesh& mesh
)
{
    if (!mesh.schemesDict().found("spatialScheme"))
    {
        FatalErrorInFunction
        	<< "spatialScheme not specified in fvSchemes" << nl << nl
            << "Valid spatial schemes are :" << endl
            << wordConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }
    
    const word schemeName(mesh.schemesDict()["spatialScheme"]);
    	
    auto cstrIter = wordConstructorTablePtr_->cfind(schemeName);

    if (!cstrIter.found())
    {
        FatalErrorInFunction
        	<< "Unknown spatialScheme " << schemeName << nl << nl
            << "Valid spatial schemes are :" << endl
            << wordConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }
    
    Info<< "Spatial scheme: " << schemeName << nl << endl;
    
    return cstrIter()();
}

// ************************************************************************* //
