/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2011 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                            | Copyright (C) 2019 AUTHOR,AFFILIATION
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class Type>
void Foam::spongeZones::absorb
(
	GeometricField<Type, fvPatchField, volMesh>& vFlux,
	const GeometricField<Type, fvPatchField, volMesh>& fPrime
) const
{
	const labelList& cellIDs = cells_;

	const scalar invDt(1.0/vFlux.mesh().time().deltaTValue());
	
	forAll(cellIDs, i)
	{
		label celli = cellIDs[i];

		vFlux[celli] -= sigma_[celli]*invDt*fPrime[celli];
	}
}

template <class Type>
void Foam::spongeZones::diffuse
(
	GeometricField<Type, fvsPatchField, surfaceMesh>& flux,
	const GeometricField<Type, fvPatchField, volMesh>& fPrime
) const
{
	const labelList& faceIDs = ifaces_;
	
	const fvMesh& mesh = flux.mesh();
	
	const scalar invDt(1.0/mesh.time().deltaTValue());
	
	forAll(faceIDs, i)
	{
		label facei = faceIDs[i];
		
		const label own(mesh.owner()[facei]), nei(mesh.neighbour()[facei]);
		
		flux[facei]
			-= omegaf_[facei]*invDt*(fPrime[nei] - fPrime[own]);
	}
	
	forAll(mesh.boundary(), patchi)
	{
		if (!mesh.boundary()[patchi].coupled()) continue;
		
		const labelList& pfaceIDs = bfaces_[patchi];
		
		const scalarField& pomega = omegaf_.boundaryField()[patchi];
		
		Field<Type>& pflux = flux.boundaryFieldRef()[patchi];
		
		const Field<Type> ownfPrime
		(
			fPrime.boundaryField()[patchi].patchInternalField()
		);
		
		const Field<Type> neifPrime
		(
			fPrime.boundaryField()[patchi].patchNeighbourField()
		);
		
		forAll(pfaceIDs, i)
		{
			const label facei = pfaceIDs[i];
			
			pflux[facei]
				-= pomega[facei]*invDt*(neifPrime[facei] - ownfPrime[facei]);
		}
	}
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //
