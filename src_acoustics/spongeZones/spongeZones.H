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
    Foam::spongeZones

Description

SourceFiles
    spongeZonesI.H
    spongeZones.C

\*---------------------------------------------------------------------------*/

#ifndef spongeZones_H
#define spongeZones_H

#include "cellZoneSet.H"
#include "bitSet.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class spongeZones Declaration
\*---------------------------------------------------------------------------*/

class spongeZones
{
    // Private data
    
    	//- Sponge zone cell indices
    	labelList cells_;
    	
    	//- Sponge zone (internal & boundary) face indices
    	labelList ifaces_;
    	
    	PtrList<labelList> bfaces_;
    	
    	//- Sponge properties
    	volScalarField sigma_;
    	
    	surfaceScalarField omegaf_;


    // Private Member Functions

        //- No copy construct
        spongeZones(const spongeZones&) = delete;

        //- No copy assignment
        void operator=(const spongeZones&) = delete;
        
        //- Add sponge zone from cellZone
        void addSpongeZone
        (
        	const fvMesh& mesh,
        	const dictionary& dict,
        	bitSet& cellInSponge,
        	volScalarField& omega
        );


public:

    // Constructors

        //- Construct from components
        spongeZones
        (
        	const fvMesh& mesh
        );

    // Member Functions
    
    	// Access
    	const volScalarField& sigma() const;
    	
    	const surfaceScalarField& omegaf() const;

        // Calculate
        template <class Type>
        void absorb
        (
        	GeometricField<Type, fvPatchField, volMesh>& vFlux,
			const GeometricField<Type, fvPatchField, volMesh>& vf
        ) const;
        
        template <class Type>
        void diffuse
        (
        	GeometricField<Type, fvsPatchField, surfaceMesh>& flux,
			const GeometricField<Type, fvPatchField, volMesh>& vf
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "spongeZonesI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
