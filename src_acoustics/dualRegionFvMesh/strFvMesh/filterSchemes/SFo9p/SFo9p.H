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
    Foam::SFo9p

Description
	9-point optimized selective filter [Bogey & Bailly (2004), JCP 194:194-214].

SourceFiles
    SFo9p.C

\*---------------------------------------------------------------------------*/

#ifndef SFo9p_H
#define SFo9p_H

#include "filterScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                             Class SFo9p Declaration
\*---------------------------------------------------------------------------*/

class SFo9p
:
    public filterScheme
{
    // Private Member Functions

        //- No copy construct
        SFo9p(const SFo9p&) = delete;

        //- No copy assignment
        void operator=(const SFo9p&) = delete;


public:

    //- Runtime type information
    TypeName("SFo9p");
    
    // Constructors

        //- Construct from Istream
        SFo9p(Istream&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
