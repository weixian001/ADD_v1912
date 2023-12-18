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
    Foam::SFo11p

Description
	11-point optimized selective filter [Bogey & Bailly (2004), JCP 194:194-214]

SourceFiles
    SFo11p.C

\*---------------------------------------------------------------------------*/

#ifndef SFo11p_H
#define SFo11p_H

#include "filterScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                             Class SFo11p Declaration
\*---------------------------------------------------------------------------*/

class SFo11p
:
    public filterScheme
{
    // Private Member Functions

        //- No copy construct
        SFo11p(const SFo11p&) = delete;

        //- No copy assignment
        void operator=(const SFo11p&) = delete;


public:

    //- Runtime type information
    TypeName("SFo11p");
    
    // Constructors

        //- Construct from Istream
        SFo11p(Istream&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
