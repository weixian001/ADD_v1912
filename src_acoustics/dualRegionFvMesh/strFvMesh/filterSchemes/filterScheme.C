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

#include "filterScheme.H"

namespace Foam
{
	defineTypeNameAndDebug(filterScheme, 0);
	defineRunTimeSelectionTable(filterScheme, fromIstream);
}

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::filterScheme::convertBeta()
{
	const label n = beta_.size();
	
	// Form upper-triangular matrix
	scalarSquareMatrix U(n, 0.0);
	
	for(register label r = 0; r < n; r++)
	{
		for(register label c = r; c < n; c++)
		{
			//U[r][c] = (1.0/pow(4,c))*factorial(2*c)/(factorial(r+c)*factorial(c-r));
			U[r][c] = factorial(2*c)/(factorial(r+c)*factorial(c-r));
		}
	}
	// Solve for beta
	LUsolve(U, beta_);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::filterScheme::filterScheme(Istream& is)
:
	F_(SMALL),
	beta_()
{
	if (!is.eof())
	{
		F_ = readScalar(is);
		
		if (F_ < -SMALL || F_ > 1.0)
		{
			FatalErrorInFunction
		    	<< "Filter strength must be betweeen 0-1. "
		        << "Given value " << F_ << "." << nl << endl
		        << abort(FatalError);
		}
	}
}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::filterScheme>
Foam::filterScheme::New
(
    const fvMesh& mesh
)
{
    if (!mesh.schemesDict().found("filterScheme"))
    {
        FatalErrorInFunction
        	<< "filterScheme not specified in fvSchemes" << nl << nl
            << "Valid filter schemes are :" << endl
            << fromIstreamConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }
    
    ITstream is(mesh.schemesDict().lookup("filterScheme"));
    
    const word schemeName(is);
    	
    auto cstrIter = fromIstreamConstructorTablePtr_->cfind(schemeName);

    if (!cstrIter.found())
    {
        FatalErrorInFunction
        	<< "Unknown filterScheme " << schemeName << nl << nl
            << "Valid filter schemes are :" << endl
            << fromIstreamConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }
    
    Info<< "Filter scheme: " << schemeName << nl << endl;
    
    return cstrIter()(is);
}

// ************************************************************************* //
