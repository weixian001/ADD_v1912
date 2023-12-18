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
    Foam::LDDRK46

Description
	Low-dissipation-dispersion 4th order 6-stage Runge Kutta scheme from
	Berland et, al (2006), Computers & Fluids.

SourceFiles
    LDDRK46.C

\*---------------------------------------------------------------------------*/

#ifndef LDDRK46_H
#define LDDRK46_H

#include "lowStorageRungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class LDDRK46 Declaration
\*---------------------------------------------------------------------------*/

class LDDRK46
:
    public lowStorageRungeKuttaScheme
{
    // Private Member Functions

        //- No copy construct
        LDDRK46(const LDDRK46&) = delete;

        //- No copy assignment
        void operator=(const LDDRK46&) = delete;


public:

    //- Runtime type information
    TypeName("LDDRK46");
    
    // Constructors

        //- Construct from time
        LDDRK46(Time& time);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
