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

#include "dualRegionFvMesh.H"

// * * * * * * * * * * * * * Private Member Functions * * * * * * * * * * * * //

void Foam::dualRegionFvMesh::renumber
(
	labelListList& compactAddr,
	const labelList& globalIndices
)
{
	for(labelList& addr: compactAddr)
	{
		for(label& celli : addr)
		{
			celli = globalIndices[celli];
		}
	}
}

void Foam::dualRegionFvMesh::setMapPtr
(
	autoPtr<mapDistribute>& mapPtr,
	labelListList& globalAddr,
	const globalIndex& gi
)
{
	// Set mapDistribute pointer
	List<Map<label>> compactMap;
	
	mapPtr.set
	(
		new mapDistribute
		(
			gi,
			globalAddr,
			compactMap
		)
	);
	
	// Global addressing has been converted to compact addressing. Change name
	// to reflect this.
	labelListList& compactAddr = globalAddr;
	
	// mySubMap initially contains all local element indices
	labelList& mySubMap = mapPtr().subMap()[Pstream::myProcNo()];
	
	const label mySubSize(mySubMap.size()); // local size
	
	// List of local elements required locally (subset of mySubMap)
	labelList newSubMap(mySubSize, -1);
	
	// Keep track of local elements required locally
	bitSet isElemUsed(mySubSize);
	
	label n(0);
	
	for(const labelList& addr : compactAddr)
	{
		for(const label& elem : addr)
		{
			// Local element required locally; element not marked previously
			if (elem < mySubSize && !isElemUsed.test(elem))
			{
				isElemUsed.set(elem);
				
				newSubMap[n++] = elem;
			}
		}
	}
	
	// Update mySubMap with resized newSubMap
	newSubMap.setSize(n);
	
	mySubMap.transfer(newSubMap);
	
	// Update construct size (difference = local size - new size)
	labelList& myCstrMap = mapPtr().constructMap()[Pstream::myProcNo()];
	
	const label offset(myCstrMap.size() - n);
	
	mapPtr().constructSize() -= offset;
	
	// Truncate myCstrMap to n elements and populate map by running indices
	// as all (and only) elements in updated mySubMap will be required. Create
	// map of where local cell from mySubMap ends up in myCstrMap.
	myCstrMap.setSize(n);
	
	Map<label> elemMap;
	
	forAll(myCstrMap, i)
	{
		myCstrMap[i] = i;
		
		elemMap.insert(mySubMap[i], i);
	}
	
	// Correct all other constructMaps by offsetting indices
	forAll(mapPtr().constructMap(), proci)
	{
		labelList& cstrMap = mapPtr().constructMap()[proci];
		
		if (Pstream::myProcNo() != proci && cstrMap.size())
		{
			for(label& elem : cstrMap)
			{
				elem -= offset;
			}
		}
	}
	
	// Correct indices in addr either by looking up elemMap if element is local
	// or by offsetting indices if element is remote.
	for(labelList& addr : compactAddr)
	{
		for(label& elem : addr)
		{
			// Local element
			if (elem < mySubSize)
			{
				elem = elemMap[elem];
			}
			// Remote element
			else
			{
				elem -= offset;
			}
		}
	}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::dualRegionFvMesh::dualRegionFvMesh(const Time& time)
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
	//              CREATE MESHES & READ INTERPOLATION PROPERTIES            //
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
	
	// Read constant/regionProperties dictionary
	regionProperties rp(time);
	
	const wordList linNames(rp["linear"]);
	
	const wordList nonLinNames(rp["nonLinear"]);
	
	if (linNames.size() > 1)
	{
		FatalErrorInFunction
			<< "More than one linear mesh region specified in "
			<< "regionProperties."
			<< exit(FatalError);
	}
	
	if (nonLinNames.size() > 1)
	{
		FatalErrorInFunction
			<< "More than one nonLinear mesh region specified in "
			<< "regionProperties."
			<< exit(FatalError);
	}
	
	// Create linear mesh
	linMeshPtr_.set
	(
		new strFvMesh
		(
			IOobject
			(
				linNames[0],
				time.timeName(),
				time,
				IOobject::MUST_READ
			)
		)
	);
	
	// Create non-linear mesh
	nonLinMeshPtr_.set
	(
		new fvMesh
		(
			IOobject
			(
				nonLinNames[0],
				time.timeName(),
				time,
				IOobject::MUST_READ
			)
		)
	);
	
	// Store cellTypes with default value CALCULATED
    tlinCellTypes_.reset
    (
    	new volScalarField
    	(
			IOobject
			(
				"cellTypes",
				time.timeName(),
				linMesh(),
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			linMesh(),
			dimensionedScalar(CALCULATED)
    	)
    );
	
	// Read interface properties from fvSchemes dictionary of linMesh 
	const dictionary& dict
	(
		linMesh().schemesDict().subDict("interfaceInterpolation")
	);
	
	// If no patches specified in dictionary, it is a decoupled simulation and
	// no mapping is required.
	if (!dict.found("patches"))
	{
		interfacePatchIDs_.setSize(0);
		
		WarningInFunction
			<< "No interfaces specified. Proceeding with uncoupled simulation."
			<< endl;
		
		return;
	}
	
	// Read user-specified interface patches of nonLinMesh & store their IDs
	const wordList interfacePatchNames(dict.lookup("patches"));
	
	interfacePatchIDs_.setSize(interfacePatchNames.size());
	
	forAll(interfacePatchNames, patchi)
	{
		const word& patchName = interfacePatchNames[patchi];
		
		interfacePatchIDs_[patchi]
			= nonLinMesh().boundary().findPatchID(patchName);
	}
	
	// Read number of offset layers (default = 0)
	const label nOffset
	(
		dict.lookupOrDefault<label>("nOffset", 0)
	);
	
	if (nOffset < 0)
	{
		FatalErrorInFunction
			<< "Invalid input " << nOffset << " for nOffset "
			<< "in interfaceInterpolation dictionary in "
			<< linNames[0] << "/fvSchemes." << nl
			<< "nOffset must be greater than or equal to 0."
			<< exit(FatalError);
	}
	
	// Read number of layers from dictionary if found and check if it meets
	// the requirement for spatial and filter scheme stencil widths.
	label nLayers(0);
	
	if (dict.found("nLayers"))
	{
		nLayers = readLabel(dict["nLayers"]);
		
		const label depth(nLayers - nOffset);
	
		if (depth < linMeshPtr_().scheme().size())
		{
			FatalErrorInFunction
				<< "Number of mapped cells " << depth << " is insufficient for "
				<< "spatial scheme with size " << linMeshPtr_().scheme().size()
				<< exit(FatalError);
		}
		
		if 
		(
			   depth < linMeshPtr_().filter().size()
			&& linMeshPtr_().filter().F() > SMALL
		)
		{
			FatalErrorInFunction
				<< "Number of mapped cells " << depth << " is insufficient for "
				<< "filter scheme with size " << linMeshPtr_().filter().size()
				<< exit(FatalError);
		}
	}
	// If not provided, determine based on stencil widths of spatial and filter
	// schemes and correct by number of offset layers.
	else
	{
		const label depth
		( 
			max
			(
				linMeshPtr_().scheme().size(),
				(
					linMeshPtr_().filter().F() > SMALL ? 
					linMeshPtr_().filter().size() : 0
				)
			)
		);
		
		nLayers = depth + nOffset;
	}
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
    //                   MAP FROM LINEAR TO INTERFACE PATCHES                //
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
    
    // Read interpolation order (default = 4th order)
	const label order(dict.lookupOrDefault<label>("order", 4));
	
    // Determine number of interpolation points on interfaces patches
    label nIntPts(0);
    
    for(const label patchi : interfacePatchIDs_)
    {
    	nIntPts += nonLinMesh().boundary()[patchi].size();
    }
    
    l_to_ifp_addr_.setSize(nIntPts);
    
    l_to_ifp_wght_.setSize(nIntPts);
    
    valueFraction_.setSize(nIntPts, 1.0);
    
    label pti(0);
    
    if (order == 0) // linear interpolation with one cell in each region
    {
		for(const label patchi : interfacePatchIDs_)
		{
			const pointField& cf = nonLinMesh().boundary()[patchi].Cf();
			
			const pointField cn(nonLinMesh().boundary()[patchi].Cn());
			
			forAll(cf, facei)
			{
				linStrMesh().calcLinearInterpAddr
				(
					cf[facei],
					cn[facei],
					l_to_ifp_addr_[pti],
					l_to_ifp_wght_[pti]
				);
				
				// Transfer weight to value fraction and reset weight to 1.
				valueFraction_[pti] = l_to_ifp_wght_[pti][0];
				
				l_to_ifp_wght_[pti][0] = 1.0;
				
				++pti;
			}
		}
    }
    else // generalized Lagrange interpolation
    {
    	// (interface patch face value)
    	// = relax*(nonlinear region cell value) 
    	// + (1-relax)(linear region interpolated value)
    	
    	scalar relax(0.0);
    	
    	// Read relaxation coefficient from dict for 2nd order and higher
    	if (order > 1)
    	{
    		relax = dict.lookupOrDefault<scalar>("relaxation", 0.0);
    	}
    	
    	// Value fraction depends on relaxation coefficient
    	valueFraction_ = 1.0 - relax;
    	
    	// Factor to shift interpolation point from current position along
    	// vector from cn -> cf to achieve desired valueFraction.
    	const scalar factor(relax/(1.0 - relax));
    	
    	for(const label patchi : interfacePatchIDs_)
		{
			const pointField& cf = nonLinMesh().boundary()[patchi].Cf();
			
			const pointField cn(nonLinMesh().boundary()[patchi].Cn());
			
			forAll(cf, facei)
			{
				const point ptToInterp
				(
					cf[facei] + factor*(cf[facei] - cn[facei])
				);
				
				linStrMesh().calcLagrangeInterAddr
				(
					ptToInterp,
					order,
					l_to_ifp_addr_[pti],
					l_to_ifp_wght_[pti]
				);
				
				++pti;
			}
		}
    }
    
    setMapPtr
    (
    	l_to_ifp_mapPtr_,
    	l_to_ifp_addr_,
    	linStrMesh().gCells()
    );
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
    //                    MAP FROM NON-LINEAR TO LINEAR                      //
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
	
	// Mesh-to-mesh mapper using cellVolumeWeight method by default
	const meshToMesh::interpolationMethod mapMethod
	(
		meshToMesh::interpolationMethodNames_.get
		(
			dict.lookupOrDefault<word>("method", "cellVolumeWeight")
		)
	);
	
    meshToMesh mapper
    (
        linMesh(),              // src
        nonLinMesh(),           // tgt
        mapMethod,              // mesh-to-mesh interpolation method
        HashTable<word>(0),     // patchMap,
        wordList(0),            // cuttingPatches
        meshToMesh::procMapMethod::pmAABB,
        false                   // do not normalise weights
    );
    
    // For each cell in linMesh, nl_to_l_addr contains list of nonLinMesh cell 
    // indices (in compact form) involved in mapping. nl_to_l_wght consists of 
    // the corresponding (un-normalized) weights.
    nl_to_l_addr_ = mapper.srcToTgtCellAddr();
    
    nl_to_l_wght_ = mapper.srcToTgtCellWght();
    
    // Cell type (default CALCULATED)
	labelList cellTypes(linMesh().nCells(), CALCULATED);
	
	// Set linMesh cells which overlap nonLinMesh to HOLE
	forAll(nl_to_l_addr_, celli) // celli = index in linMesh
	{
		const labelList& addr = nl_to_l_addr_[celli];
		
		if (addr.size())
		{
			cellTypes[celli] = HOLE;
		}
	}
	
	// Check for linMesh cells which overlap interface patch cells. This is
	// easier than checking for cells which overlap the patch faces.
	globalIndex giLin(linMesh().nCells());
	
	labelList linCellIDs(labelRange::identity(linMesh().nCells()).labels());
	
	giLin.inplaceToGlobal(linCellIDs); // convert to global (in proc order)
	
	if (mapper.srcMap().valid())
	{
		// !Parallel Communication! linCellIDs contains indices of all linMesh
		// cells which overlap nonLinMesh in compact form.
		mapper.srcMap()().distribute(linCellIDs); 
	}
	
	boolList isMappedToInterfaceCell(giLin.size(), false);
	
	boolList isMappedToPatchCell(giLin.size(), false);
	
	// For each cell in nonLinMesh, l_to_nl_addr contains list of linMesh cell 
	// indices (in compact form) involved in mapping.
	const labelListList& l_to_nl_addr = mapper.tgtToSrcCellAddr();
	
	for(const label patchi : interfacePatchIDs_)
	{
		const fvPatch& fvp = nonLinMesh().boundary()[patchi];
		
		// pcelli = local nonLinMesh cell index
		for(const label pcelli : fvp.faceCells())
		{
			const labelList& addr = l_to_nl_addr[pcelli];
			
			// celli = compact linMesh cell index (in srcMap)
			for(const label celli : addr) 
			{
				// gcelli = global linMesh cell index
				label gcelli = linCellIDs[celli]; 
				
				isMappedToInterfaceCell[gcelli] = true;
			}
		}
	}
	
	Pstream::listCombineGather(isMappedToInterfaceCell, orEqOp<bool>());

	Pstream::listCombineScatter(isMappedToInterfaceCell);
	
	// Set cell type of cells next to interfaces patches to MAPPED temporarily 
	// to create a layer of MAPPED cells separating CALCULATED and HOLE cells.
	forAll(nl_to_l_addr_, celli) // celli = local linMesh cell index
	{
		// gcelli = global linMesh cell index
		const label gcelli = giLin.toGlobal(celli); 
		
		if (isMappedToInterfaceCell[gcelli])
		{
			cellTypes[celli] = MAPPED;
		}
	}
	
	// Determine CALCULATED cells which lie next to HOLE cells and set them to 
	// HOLE as well.
	while (true)
	{
		Condition
		<
			equalOp<label>,
			equalOp<label>,
			andOp<bool>
		> fillHoleCondition
		(
			equalOp<label>(),
			CALCULATED,
			equalOp<label>(),
			HOLE,
			andOp<bool>()
		);
		
		bool stopWalk = walkFront
		(
			linMesh(),
			cellTypes,
			label(HOLE),
			fillHoleCondition
		);
		
		if (stopWalk)
		{
			break;
		}
	}
	
	// Convert ALL MAPPED cells to CALCULATED if there is insufficient overlap
	// or HOLE if there is sufficinet overlap.
	forAll(nl_to_l_addr_, celli) // celli = local linMesh cell index
	{
		if (cellTypes[celli] == MAPPED)
		{
			const scalar overlapVol(sum(nl_to_l_wght_[celli]));
			
			const scalar& tgtCellVol = linMesh().V()[celli];
			
			// Insufficient overlap
			if ((1.0 - overlapVol/tgtCellVol) > 1e-6)
			{
				cellTypes[celli] = CALCULATED;
			}
			else
			{
				cellTypes[celli] = HOLE;
			}
		}
	}
	
	// Walk front once across faces with CALCULATED | HOLE cell arrangement.
	// The HOLE cells are converted to MAPPED to create the first layer of
	// CALCULATED | MAPPED boundary.
	// Walk front for nLayers-1 more times. Each time the front is extended 
	// across faces with MAPPED | HOLE cell arrangement. The HOLE cell is 
	// converted to MAPPED. The process is terminated early if no cells are 
	// changed in a particular iteration.
	linCellType TYPE(CALCULATED);
	
	for(register label n = 0; n < nLayers; ++n)
	{
		Condition
		<
			equalOp<label>,
			equalOp<label>,
			andOp<bool>
		> mappedLayerCondition
		(
			equalOp<label>(),
			HOLE,
			equalOp<label>(),
			TYPE,
			andOp<bool>()
		);
		
		bool stopWalk = walkFront
		(
			linMesh(),
			cellTypes,
			label(MAPPED),
			mappedLayerCondition
		);
		
		// Change to MAPPED after first layer
		TYPE = MAPPED;
		
		if (stopWalk)
		{
			break;
		}
	}
	
	// Convert MAPPED cells to CALCULATED for nOffset layers starting from the
	// CALCULATED | MAPPED boundary.
	for(register label n = 0; n < nOffset; ++n)
	{
		Condition
		<
			equalOp<label>,
			equalOp<label>,
			andOp<bool>
		> layerOffsetCondition
		(
			equalOp<label>(),
			MAPPED,
			equalOp<label>(),
			CALCULATED,
			andOp<bool>()
		);
		
		bool stopWalk = walkFront
		(
			linMesh(),
			cellTypes,
			label(CALCULATED),
			layerOffsetCondition
		);
		
		if (stopWalk)
		{
			break;
		}
	}
	
	// Remove addressing for CALCULATED and HOLE cells and normalize weights
	// for MAPPED cells.
	forAll(nl_to_l_addr_, celli) // celli = local linMesh cell index
	{
		if (cellTypes[celli] == MAPPED)
		{
			scalarList& wght = nl_to_l_wght_[celli];
			
			const scalar overlapVol(sum(wght));
			
			// normalise weights
			for(scalar& w : wght)
			{
				w /= overlapVol;
			}
		}
		else
		{
			nl_to_l_addr_[celli].clear();
			
			nl_to_l_wght_[celli].clear();
		}
	}
	
    // Renumber nl_to_l_addr_ using global indexing for non-linear mesh
	globalIndex giNonLin(nonLinMesh().nCells());
	
	labelList nonLinCellIDs
	(
		labelRange::identity(nonLinMesh().nCells()).labels()
	);
	
	giNonLin.inplaceToGlobal(nonLinCellIDs); // convert to global
	
	if (mapper.tgtMap().valid())
	{
		// !Parallel Communication! nonLinCellIDs contains global indices of 
		// all nonLinMesh cells which overlap linMesh ordered in compact form.
		mapper.tgtMap()().distribute(nonLinCellIDs);
	}
	
	// Convert compact indexing to global indexing and create mapDistribute
	// object pointer based on current nl_to_l_addr_ mapping.
	renumber(nl_to_l_addr_, nonLinCellIDs);
    
    setMapPtr
    (
    	nl_to_l_mapPtr_,
    	nl_to_l_addr_,
    	giNonLin
    );
    
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
    //                          HANDLING CELL TYPES                          //
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
	
	// Create list of hole and mapped cells for ease of manipulation later
    {
    	DynamicList<label> holeIDs(linMesh().nCells());
    	
    	DynamicList<label> mappedIDs(linMesh().nCells());
    	
    	forAll(cellTypes, celli)
    	{
    		if (cellTypes[celli] == HOLE)
    		{
    			holeIDs.append(celli);
    		}
    		else if (cellTypes[celli] == MAPPED)
    		{
    			mappedIDs.append(celli);
    		}
    	}
    	
    	holeCells_.transfer(holeIDs);
    	
    	mappedCells_.transfer(mappedIDs);
    }
    
    scalarField& ilinCellTypes = tlinCellTypes_.ref().primitiveFieldRef();
    
    forAll(cellTypes, celli)
    {
    	ilinCellTypes[celli] = scalar(cellTypes[celli]);
    }
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::strFvMesh& Foam::dualRegionFvMesh::linStrMesh() const 
{
	return linMeshPtr_();
}

const Foam::fvMesh& Foam::dualRegionFvMesh::linMesh() const 
{
	return linMeshPtr_();
}

Foam::fvMesh& Foam::dualRegionFvMesh::linMesh() 
{
	return linMeshPtr_();
}

const Foam::fvMesh& Foam::dualRegionFvMesh::nonLinMesh() const 
{
	return nonLinMeshPtr_();
}

Foam::fvMesh& Foam::dualRegionFvMesh::nonLinMesh() 
{
	return nonLinMeshPtr_();
}

const Foam::scalarField& Foam::dualRegionFvMesh::valueFraction() const
{
	return valueFraction_;
}

const Foam::labelList& Foam::dualRegionFvMesh::interfacePatchIDs() const
{
	return interfacePatchIDs_;
}

const Foam::labelListList& Foam::dualRegionFvMesh::l_to_ifp_addr() const 
{
	return l_to_ifp_addr_;
}

const Foam::scalarListList& Foam::dualRegionFvMesh::l_to_ifp_wght() const 
{
	return l_to_ifp_wght_;
}

const Foam::mapDistribute& Foam::dualRegionFvMesh::l_to_ifp_map() const 
{
	return l_to_ifp_mapPtr_();
}

const Foam::labelListList& Foam::dualRegionFvMesh::nl_to_l_addr() const 
{
	return nl_to_l_addr_;
}

const Foam::scalarListList& Foam::dualRegionFvMesh::nl_to_l_wght() const 
{
	return nl_to_l_wght_;
}

const Foam::mapDistribute& Foam::dualRegionFvMesh::nl_to_l_map() const 
{
	return nl_to_l_mapPtr_();
}

const Foam::labelList& Foam::dualRegionFvMesh::holeCells() const 
{
	return holeCells_;
}

const Foam::labelList& Foam::dualRegionFvMesh::mappedCells() const 
{
	return mappedCells_;
}

const Foam::volScalarField& Foam::dualRegionFvMesh::linCellTypes() const 
{
	return tlinCellTypes_.cref();
}

// ************************************************************************* //
