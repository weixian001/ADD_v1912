/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
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
    dualZoneEulerFoam

Description
    Solve the linearized Euler equations (LEE) on a structured uniform
    mesh.

\*---------------------------------------------------------------------------*/

#include "acousticsFvCFD.H"
#include "fvcCellReduce.H"
#include "psiThermo.H"
#include "turbulentFluidThermoModel.H"
#include "fvOptions.H"

#include "interpolateFuncs.H"
#include "fluxFuncs.H"

#define fluxUpwind fluxHLLC

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
	argList::addBoolOption
    (
        "useLimited",
        "use limited gradient to compute upwind fluxes"
    );
    
	#include "setRootCase.H"
	#include "createTime.H"
    #include "createDualRegionFvMesh.H"
    #include "createFields.H"
    #include "createFvOptions.H"
    
    const bool useLimited(args.found("useLimited"));
    
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    
    #include "correctBCs.H"
	
	Info<< "\nStarting time loop\n" << endl;
	
	while(runTime.run())
    {
    	while(rkTime.run())
    	{
			#include "constructLinFluxes.H"
			
			#include "constructUpwindNonLinFluxes.H"
			
			// Add (optional) heat source
			fvOptions.correct(rhoEFlux);
			
			rkTime.advance( rho);
    		
			rkTime.advance(rhoU);
			
			rkTime.advance(rhoE);
			
			rkTime.advance( rhoPrime);
    		
			rkTime.advance(rhoUPrime);
			
			rkTime.advance(rhoEPrime);
			
    		#include "correctBCs.H"
    		
    		rkTime++;
    	}
    	
    	runTime++;
    	
    	Info<< "RunTime = " << runTime.timeName() << endl;
    	
    	runTime.write();
    	
    	Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }
    
    Info << "End\n" << endl;
    
    return 0;
}


// ************************************************************************* //
