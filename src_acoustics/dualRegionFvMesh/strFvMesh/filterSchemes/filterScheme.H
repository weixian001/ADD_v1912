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

Class
    Foam::filterScheme

Description
    Symmetric filter scheme for structured uniform meshes (strFvMesh).
    
    A symmetric (2n+1)-point finite-difference (FD) filter scheme is given by,
    f\hat 
    = d_0 f_{i} + \sum_{m=1}^{n}{d_m\left(f_{i+m} + f_{i-m}\right)}.
    
    The filtered value is computed as a weighted average of a series of partial
    sums which are computed by recursively adding the immediate cell values
    only. The partial sum weights beta are derived from the original FD filter
    weights d_m.
    

SourceFiles
    filterSchemeI.H
    filterScheme.C

\*---------------------------------------------------------------------------*/

#ifndef filterScheme_H
#define filterScheme_H

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                          Class filterScheme Declaration
\*---------------------------------------------------------------------------*/

class filterScheme
{
    // Private Data
    
    	//- Filter strength
	    scalar F_;
    
    // Private Member Functions

        //- No copy construct
        filterScheme(const filterScheme&) = delete;

        //- No copy assignment
        void operator=(const filterScheme&) = delete;

protected:

	// Protected Data
	
		//- Coefficients
		scalarField beta_;
		
		
	// Protected Member Functions
	
		//- Convert finite-difference coefficients to beta coefficients.
		//  For a (2n+1)-pt symmetric filter, d_{-m} = d_{m}.
		//  beta is initialized with the n+1 non-zero coefficients d_0 to d_n.
        void convertBeta();
		
public:

    //- Runtime type information
    TypeName("filterScheme");
    
    // Declare run-time constructor selection table(s)
		
		declareRunTimeSelectionTable
		(
		    autoPtr,
		    filterScheme,
		    fromIstream,
            (Istream& is),
            (is)
		);


    // Constructors

        //- Construct from Istream
        filterScheme(Istream& is);


    // Selectors

        //- Select null constructed
        static autoPtr<filterScheme> New
        (
        	const fvMesh& mesh
        );


    //- Destructor
    virtual ~filterScheme() {};

    // Member Functions

        // Access
        label size() const
        {
        	return beta_.size() - 1; // minus one for central cell
        }
        
        const scalar& F() const
        {
        	return F_;
        }
        
        const scalarField& beta() const
        {
        	return beta_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
