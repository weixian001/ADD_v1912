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

#include "spongeZones.H"
#include "boundBox.H"
#include "zeroGradientFvPatchField.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::spongeZones::addSpongeZone
(
	const fvMesh& mesh,
	const dictionary& dict,
	bitSet& cellInSponge,
	volScalarField& omega
)
{
	// Read cell zone
	cellZoneSet cz(mesh, word(dict.lookup("cellZone")));
	
	const labelList& cellIDs = cz.addressing();
	
	if (!cellIDs.size())
	{
		return;
	}
	
	// Add cells to sponge zone
	cellInSponge.set(cellIDs);
	
	// Read sponge layer properties from dictionary
	const vector direction(dict.lookup("direction"));
	
	const scalar sigmaMax(readScalar(dict.lookup("sigmaMax")));
	
	const scalar omegaMax(dict.lookupOrDefault<scalar>("omegaMax", 0.0));
	
	// Find bounds of sponge region (cell version)
	const vectorField& cc = mesh.cellCentres();
	
	const boundBox spBb(cc, cellIDs, false); // doReduce = false
	
	vector length(spBb.span());
	
	forAll(length, cmpt)
	{
		length[cmpt] = max(mag(length[cmpt]), SMALL);
	}
	
	// Find start location
	const vector start
	(
		cmptMultiply
		(
			direction,
			min
			(
				cmptMultiply(direction, spBb.min()),
				cmptMultiply(direction, spBb.max())
			)
		)
	);
	
	const vector cmptMagDir(cmptMag(direction));
	
	// Compute sponge layer properties
	forAll(cellIDs, i)
	{
		const label celli = cellIDs[i];
		
		vector dX
		= cmptDivide
		(
			cmptMultiply
			(
				cc[celli] - start,
				direction
			),
			length
		);
		
		vector dXsqr = cmptMultiply(dX, dX);
		
		scalar dXmag = dXsqr & cmptMagDir;
		
		omega[celli]  += omegaMax*dXmag;
		
		sigma_[celli] += sigmaMax*dXmag;
	}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::spongeZones::spongeZones
(
	const fvMesh& mesh
)
:
    cells_(),
    ifaces_(),
    bfaces_(mesh.boundary().size()),
    sigma_
    (
		IOobject
		(
			"sigma",
			mesh.time().constant(),
			mesh,
			IOobject::NO_READ
		),
		mesh,
		dimensionedScalar("zero", dimless, 0.0),
		zeroGradientFvPatchField<scalar>::typeName
    ),
    omegaf_
    (
		IOobject
		(
			"omegaf",
			mesh.time().constant(),
			mesh,
			IOobject::NO_READ
		),
		mesh,
		dimensionedScalar("zero", dimless, 0.0)
    )
{
	Info<< "\nReading spongeZonesDict" << nl << endl;
	
	IOdictionary spongeZonesDict
	(
		IOobject
		(
			"spongeZonesDict",
			mesh.time().system(),
			mesh,
			IOobject::MUST_READ,
			IOobject::NO_WRITE,
			false // do not register
		)
	);
	
	PtrList<dictionary> spongeDicts(spongeZonesDict.lookup("spongeZones"));
	
	tmp<volScalarField> tomega
	(
		new volScalarField
		(
			IOobject
			(
				"omega",
				mesh.time().constant(),
				mesh,
				IOobject::NO_READ,
				IOobject::NO_WRITE,
				false // do not register
			),
			mesh,
			dimensionedScalar("zero", dimless/dimTime, 0.0),
			zeroGradientFvPatchField<scalar>::typeName
		)
	);
	
	volScalarField& omega = tomega.ref();
	
	bitSet cellInSponge(mesh.nCells());
	
	forAll(spongeDicts, i)
	{
		const dictionary& dict = spongeDicts[i];
		
		addSpongeZone(mesh, dict, cellInSponge, omega);
	}
	
	// Create list of sponge cell indices
	label nSpongeCells(cellInSponge.count());
	
	cells_.setSize(nSpongeCells, 0);
	
	nSpongeCells = 0;
	
	forAll(cellInSponge, celli)
	{
		if (cellInSponge.test(celli))
		{
			cells_[nSpongeCells++] = celli;
		}
	}
	
	// Count number of faces for sizing face list
	bitSet faceInSponge(mesh.nFaces());
	
	const cellList& cellFaces = mesh.cells();
	
	forAll(cells_, i)
	{
		const cell& cFaces = cellFaces[cells_[i]];
		
		for(const label facei : cFaces)
		{
			faceInSponge.set(facei);
		}
	}
	
	label nIntFaces(0);
	
	for(register label facei = 0; facei < mesh.nInternalFaces(); facei++)
	{
		if (faceInSponge.test(facei))
		{
			++nIntFaces;
		}
	}
	
	labelList nPatchFaces(mesh.boundary().size(), 0);
	
	forAll(mesh.boundary(), patchi)
	{
		if (!omega.boundaryField()[patchi].size()) continue;
		
		const fvPatch& fvp = mesh.boundary()[patchi];
		
		forAll(fvp, i)
		{
			label facei = fvp.start() + i;
			
			if (faceInSponge.test(facei))
			{
				++nPatchFaces[patchi];
			}
		}
	}
	
	// Set sizes
	ifaces_.setSize(nIntFaces);
	
	forAll(nPatchFaces, patchi)
	{
		bfaces_.set
		(
			patchi,
			new labelList(nPatchFaces[patchi], 0)
		);
	}
	
	// Store face indices
	label n(0);
	
	for(register label facei = 0; facei < mesh.nInternalFaces(); facei++)
	{
		if (faceInSponge.test(facei))
		{
			ifaces_[n++] = facei;
		}
	}
	
	forAll(mesh.boundary(), patchi)
	{
		if (!omega.boundaryField()[patchi].size()) continue;
		
		const fvPatch& fvp = mesh.boundary()[patchi];
		
		n = 0;
		
		forAll(fvp, i)
		{
			label facei = fvp.start() + i;
			
			if (faceInSponge.test(facei))
			{
				bfaces_[patchi][n++] = i;
			}
		}
	}
	
	// Transfer omega values to omegaf
	forAll(mesh.neighbour(), facei)
	{
		const label own = mesh.owner()[facei];
		
		const label nei = mesh.neighbour()[facei];
		
		omegaf_[facei] = min(omega[own], omega[nei]);
	}
	
	forAll(mesh.boundary(), patchi)
	{
		if (mesh.boundary()[patchi].coupled())
		{
			omegaf_.boundaryFieldRef()[patchi]
				= min
				(
					omega.boundaryField()[patchi].patchInternalField(),
					omega.boundaryField()[patchi].patchNeighbourField()
				);
		}
	}
	
	tomega.clear();
	
	omegaf_ *= mesh.magSf()/mesh.surfaceInterpolation::deltaCoeffs();
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::volScalarField& Foam::spongeZones::sigma() const
{
	return sigma_;
}

const Foam::surfaceScalarField& Foam::spongeZones::omegaf() const
{
	return omegaf_;
}


// ************************************************************************* //
