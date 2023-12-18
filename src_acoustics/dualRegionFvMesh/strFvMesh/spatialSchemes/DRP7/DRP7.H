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
    Foam::DRP7

Description
	Group velocity optimized dispersion-relation-preserving (DRP) scheme from 
	Computational Aeroacoustics (2012), Tam C K W, Cambridge University Press.

SourceFiles
    DRP7.C

\*---------------------------------------------------------------------------*/

#ifndef DRP7_H
#define DRP7_H

#include "spatialScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                             Class DRP7 Declaration
\*---------------------------------------------------------------------------*/

class DRP7
:
    public spatialScheme
{
    // Private Member Functions

        //- No copy construct
        DRP7(const DRP7&) = delete;

        //- No copy assignment
        void operator=(const DRP7&) = delete;
        
public:

    //- Runtime type information
    TypeName("DRP7");
    
    // Constructors

        //- Construct from null
        DRP7();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
