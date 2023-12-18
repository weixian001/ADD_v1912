/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2004-2010 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                            | Copyright (C) 2011 OpenFOAM Foundation
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

Application
    Test-meshToMesh

Description
    Test functionality of dualRegionFvMesh class in mapping fields from
    the linear mesh region to the non-linear mesh region.

\*---------------------------------------------------------------------------*/

#include "acousticsFvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

	#include "setRootCase.H"
	#include "createTime.H"
	#include "createDualRegionFvMesh.H"
	
	// Read fields
	volScalarField phiLin
    (
    	IOobject
        (
            "phi",
            runTime.timeName(),
            linMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        linMesh
    );
    
    volScalarField phiNonLin
    (
    	IOobject
        (
            "phi",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );
    
    // Set internal values to proc numbers
    phiLin = dimensionedScalar(Pstream::myProcNo());
    
    phiNonLin = dimensionedScalar(Pstream::nProcs() + Pstream::myProcNo());
    
    // Create zero field on linMesh to suppy as mean field during mapping
    volScalarField zero
    (
    	IOobject
        (
            "zero",
            runTime.timeName(),
            linMesh
        ),
        linMesh,
        dimensionedScalar(0.0)
    );
    
    // Initialize value fraction & reference gradient for interface patches
    // Reference value is initialized only in map_l_to_ifp function later.
    drMesh.initializeInterfaces(phiNonLin);
    
	runTime.write();
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	
	runTime++;
	
	Info<< "\nTime = " << runTime.timeName() << endl;
	
	// Map phi from nonLinMesh to linMesh
	drMesh.map_nl_to_l
	(
		phiLin,
		zero,
		phiNonLin
	);
	
    runTime.write();
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	
	runTime++;
	
	Info<< "\nTime = " << runTime.timeName() << endl;
	
    // Map phi from linMesh to interface patches of nonLinMesh
    drMesh.map_l_to_ifp
    (
    	phiNonLin,
    	phiLin,
    	zero
    );
    
    // Call correct boundary conditions to update interface patch values 
    phiNonLin.correctBoundaryConditions();
    
    // Transfer patch values to patch cells for visualization
    for(const label patchi : drMesh.interfacePatchIDs())
    {
    	const scalarField& phip = phiNonLin.boundaryField()[patchi];
    	
    	const labelList& faceCells = mesh.boundary()[patchi].faceCells();
    	
    	forAll(phip, facei)
    	{
    		phiNonLin[faceCells[facei]] = phip[facei];
    	}
    }
    
	runTime.write();
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  //
	
    Info<< "\nEnd" << endl;
}


// ************************************************************************* //
