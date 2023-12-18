/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2004-2011 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                            | Copyright (C) 2011-2017 OpenFOAM Foundation
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
    dualbuoyantPimpleFoam

Group
    grpHeatTransferSolvers

Description
    Transient solver for buoyant, turbulent flow of compressible fluids for
    ventilation and heat-transfer.

    Turbulence is modelled using a run-time selectable compressible RAS or
    LES model.

\*---------------------------------------------------------------------------*/

#include "acousticsFvCFD.H"
#include "rhoThermo.H"
#include "psiThermo.H"
#include "turbulentFluidThermoModel.H"
#include "radiationModel.H"
#include "fvOptions.H"
#include "pimpleControl.H"
#include "pressureControl.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Transient solver for buoyant, turbulent fluid flow"
        " of compressible fluids, including radiation."
    );

    #include "addCheckCaseOptions.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createDualRegionFvMesh.H"
    #include "createControl.H"
    #include "createFields.H"    
    #include "createLinFields.H"
    #include "initContinuityErrs.H"
    #include "createTimeControls.H"
    #include "compressibleCourantNo.H"
    #include "setInitialDeltaT.H"
    
    turbulence->validate();
    
    fv::options& linFvOptions(fv::options::New(linMesh));

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;
    
    #include "correctBCs.H"

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "compressibleCourantNo.H"
        #include "setDeltaT.H"
        
        Info<< "Time = " << runTime.timeName() << nl << endl;
        
        // * * * * * * * * * * * * * * * LINEAR * * * * * * * * * * * * * * * //
        
        // Store deltaT used in (nonlinear) mesh
		const scalar dtNonLin = runTime.deltaTValue();
		
		// Read nSubCycles from fvSchemes
    	const label nSubCycles
		(
			linMesh.schemesDict().lookupOrDefault<label>("nSubCycles", 1)
		);
		
		// Set deltaT to be used in linMesh (linear)
		runTime.setDeltaT(dtNonLin/nSubCycles, false);
		
		// Run linear solver for specified number of sub-cycles
		for(register label i = 0; i < nSubCycles; i++)
    		{ 
		    while(rkTime.run())
			{
				#include "constructLinFluxes.H"
				
//Info << "Done ------- constructLinFluxes" << endl;
				linFvOptions.correct(rhoEPrimeFlux);
				
				rkTime.advance( rhoPrime);
				rkTime.advance(rhoUPrime);
				rkTime.advance(rhoEPrime);
				
//Info << "Done ------- time advance" << endl;
				#include "correctBCs.H"
				
//Info << "Done ------- correctBcs" << endl;
				rkTime++;
//Info << "Done iteration of L solver" << endl;
			}
		}
		
		// Update interface patch reference values based on filtered values
		// before proceeding with nonlinear computation

		drMesh.map_l_to_ifp(    T, TMean, TPrime);

		drMesh.map_l_to_ifp(p_rgh, pMean, pPrime);

		drMesh.map_l_to_ifp(    U, UMean, UPrime);

//Info << "Done mapping from l to interface" << endl;		
		// Reset deltaT for (nonlinear) mesh
    	runTime.setDeltaT(dtNonLin, false);

		// * * * * * * * * * * * * * * NONLINEAR * * * * * * * * * * * * * * //
        ++runTime;

        #include "rhoEqn.H"
        
        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "UEqn.H"
            #include "EEqn.H"

            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.H"
            }

            if (pimple.turbCorr())
            {
                turbulence->correct();
            }
        }

        rho = thermo.rho();
        
        runTime.write();

        runTime.printExecutionTime(Info);

        // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
