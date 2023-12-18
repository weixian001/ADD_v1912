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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type, class TestCondition>
bool Foam::dualRegionFvMesh::walkFront
(
	const fvMesh& mesh,
	List<Type>& cellVals,
	const Type newCellVal,
	const TestCondition& test
)
{
	List<Type> nbrCellVals;

	syncTools::swapBoundaryCellList(mesh, cellVals, nbrCellVals);
	
	bitSet isCellChanged(mesh.nCells());
	
	forAll(mesh.neighbour(), facei)
	{
		const label own = mesh.owner()[facei];
		
		const label nei = mesh.neighbour()[facei];
		
		if (test.valid(cellVals[own], cellVals[nei]))
		{
			isCellChanged.set(own);
		}
		else if (test.valid(cellVals[nei], cellVals[own]))
		{
			isCellChanged.set(nei);
		}
	}
	
	for
	(
		label facei = mesh.nInternalFaces();
		facei < mesh.nFaces();
		++facei
	)
	{
		const label own = mesh.faceOwner()[facei];
		
		const label bfacei = facei - mesh.nInternalFaces();
		
		if (test.valid(cellVals[own], nbrCellVals[bfacei]))
		{
			isCellChanged.set(own); // this proc only
		}
	}
	
	label nCellsChanged = 0;
	
	forAll(isCellChanged, celli)
	{
		if (isCellChanged.test(celli))
		{
			cellVals[celli] = newCellVal; // walk front
			
			++nCellsChanged;
		}
	}
	
	reduce(nCellsChanged, sumOp<label>());
	
	return (nCellsChanged == 0);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::dualRegionFvMesh::setHoleValues
(
	GeometricField<Type, fvPatchField, volMesh>& fldPrime,
	const Type& holeVal
) const 
{
	// Set values of holes to 0
	for(const label celli : holeCells_)
	{
		fldPrime[celli] = holeVal;
	}
}

template<class Type>
void Foam::dualRegionFvMesh::map_nl_to_l
(
	GeometricField<Type, fvPatchField, volMesh>&       fldPrime,
	const GeometricField<Type, fvPatchField, volMesh>& fldMean,
	const GeometricField<Type, fvPatchField, volMesh>& fld
) const
{
	// Source
	Field<Type> srcFld(fld.primitiveField());
	
	// Target
	if (nl_to_l_mapPtr_.valid())
	{
		nl_to_l_map().distribute(srcFld); // parallel comms
	}
	
	forAll(mappedCells_, i)
	{
		const label celli = mappedCells_[i];
		
		const labelList& addr = nl_to_l_addr_[celli];
		
		const scalarList& wght = nl_to_l_wght_[celli];
		
		Type val(Zero);
		
		forAll(addr, j)
		{
			val += wght[j]*srcFld[addr[j]];
		}
		
		fldPrime[celli] = val - fldMean[celli];
	}
}

template<class Type>
void Foam::dualRegionFvMesh::initializeInterfaces
(
	GeometricField<Type, fvPatchField, volMesh>& fld
) const
{
	if (interfacePatchIDs_.size())
	{
		label start(0);
		
		for(const label patchi : interfacePatchIDs_)
		{
			mixedFvPatchField<Type>& pfld
				= dynamicCast<mixedFvPatchField<Type>>
					(
						fld.boundaryFieldRef()[patchi]
					);
			
			// Initialize value fraction and reference gradient
			pfld.valueFraction()
				= SubField<scalar>(valueFraction_, pfld.size(), start);
				
			pfld.refGrad() = Field<Type>(pfld.size(), Zero);
				
			start += pfld.size();
		}
	}
}

template<class Type>
void Foam::dualRegionFvMesh::map_l_to_ifp
(
	GeometricField<Type, fvPatchField, volMesh>&  fld,
	const GeometricField<Type, fvPatchField, volMesh>& fldMean,
	const GeometricField<Type, fvPatchField, volMesh>& fldPrime
) const
{
	// Source fields
	Field<Type> meanSrc(fldMean.primitiveField());
	
	Field<Type> primeSrc(fldPrime.primitiveField());
	
	// Parallel comms
	if (l_to_ifp_mapPtr_.valid())
	{
		l_to_ifp_map().distribute(meanSrc);
		
		l_to_ifp_map().distribute(primeSrc);
	}
	
	// Target field (mean + prime)
	Field<Type> tgtFld(meanSrc + primeSrc);
	
	label pti(0);
	
	for(const label patchi : interfacePatchIDs_)
	{
		mixedFvPatchField<Type>& pfld
			= dynamicCast<mixedFvPatchField<Type>>
				(
					fld.boundaryFieldRef()[patchi]
				);
		
		// Reset reference value
		Field<Type>& refVal = pfld.refValue();
		
		forAll(pfld, facei)
		{
			const labelList& addr  = l_to_ifp_addr_[pti];
		
			const scalarList& wght = l_to_ifp_wght_[pti];
			
			Type val(Zero);
			
			forAll(addr, i)
			{
				val += wght[i]*tgtFld[addr[i]];
			}
			
			refVal[facei] = val;
			
			++pti;
		}
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //
