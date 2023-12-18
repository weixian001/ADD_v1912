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
    Foam::spatialScheme

Description
    Symmetric interpolation scheme for structured uniform meshes (strFvMesh).
    
    A symmetric (2n+1)-point finite-difference (FD) gradient scheme is given by,
    \frac{\partial f}{\partial x} 
    = \frac{1}{\Delta x}\sum_{m=1}^{n}{a_m\left(f_{i+m} - f_{i-m}\right)}.
    
    In finite-volume (FV), the gradient is obtained from Gauss theorem based on
    face interpolated values which in 1D is,
    \frac{\partial f}{\partial x}
    = \frac{1}{\Delta x}\left(f_{i+1/2} - f_{i-1/2} \right).
    
    The FV gradient computation based on the interpolated value must return the
    FD gradient exactly. The interpolated values are computed in a recursive 
    manner by adding the immediate cell values only. These partial sums are then
    summed up with appropriate weights to arrive at the final interpolated
    values at the faces. The FD coefficients are converted into the partial sum
    weights alpha.


SourceFiles
    spatialSchemeI.H
    spatialScheme.C

\*---------------------------------------------------------------------------*/

#ifndef spatialScheme_H
#define spatialScheme_H

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                          Class spatialScheme Declaration
\*---------------------------------------------------------------------------*/

class spatialScheme
{
    // Private Data
    
    // Private Member Functions

        //- No copy construct
        spatialScheme(const spatialScheme&) = delete;

        //- No copy assignment
        void operator=(const spatialScheme&) = delete;
        
protected:

	// Protected Data
	
		//- Coefficients
		scalarField alpha_;
		
	// Protected Member Functions
	
		//- Convert finite-difference gradient coefficients to finite volume 
		//  interpolation coefficients.
		//  For a (2n+1)-pt symmetric scheme, a_{-n} = -a_{n} and a_0 = 0.
		//  alpha is initialized with the n non-zero coefficients a_1 to a_n.
        void convertAlpha();
		
public:

    //- Runtime type information
    TypeName("spatialScheme");
    
    // Declare run-time constructor selection table(s)
		
		declareRunTimeSelectionTable
		(
		    autoPtr,
		    spatialScheme,
		    word,
            (),
            ()
		);


    // Constructors

        //- Construct from null
        spatialScheme();


    // Selectors

        //- Select null constructed
        static autoPtr<spatialScheme> New
        (
        	const fvMesh& mesh
        );


    //- Destructor
    virtual ~spatialScheme() {};

    // Member Functions

        // Access
        label size() const
        {
        	return alpha_.size();
        }
        
        const scalarField& alpha() const
        {
        	return alpha_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
