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
    Foam::TVDRK3

Description
	Total-variation-diminishing 3rd order 3-stage Runge Kutta scheme from
	Gottlieb, S., & Shu, C. W. (1998). Total variation diminishing Runge-Kutta 
	schemes. Mathematics of computation of the American Mathematical Society, 
	67(221), 73-85.

SourceFiles
    TVDRK3.C

\*---------------------------------------------------------------------------*/

#ifndef TVDRK3_H
#define TVDRK3_H

#include "lowStorageRungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class TVDRK3 Declaration
\*---------------------------------------------------------------------------*/

class TVDRK3
:
    public lowStorageRungeKuttaScheme
{
    // Private Member Functions

        //- No copy construct
        TVDRK3(const TVDRK3&) = delete;

        //- No copy assignment
        void operator=(const TVDRK3&) = delete;


public:

    //- Runtime type information
    TypeName("TVDRK3");
    
    // Constructors

        //- Construct from time
        TVDRK3(Time& time);
        
    // Advance in time
    virtual void advance(volScalarField& u) const;
    virtual void advance(volVectorField& u) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
