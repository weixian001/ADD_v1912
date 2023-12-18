// Compute primitive perturbation variables (linMesh)
UPrime = (rhoUMean + rhoUPrime)/(rhoMean + rhoPrime) - UMean;
//Info << "Max / min UPrime = " << max(UPrime).value() << " / " << min(UPrime).value() << endl;

pPrime = gM1*	( rhoEMean + rhoEPrime 
		  - 0.5*(rhoMean + rhoPrime)*magSqr(UMean + UPrime)
		) - pMean;

//Info << "Max / min pPrime = " << max(pPrime).value() << " / " << min(pPrime).value() << endl;

TPrime = TMean*(pPrime/pMean - rhoPrime/rhoMean);

//Info << "Max / min TPrime = " << max(TPrime).value() << " / " << min(TPrime).value() << endl;
if (rkTime.s() == rkTime.nStages()-1)
{
	// Filter perturbation variables
	//strFvc::filter(TPrime);
	strFvc::filter(rhoPrime);
	
	strFvc::filter(UPrime);

	strFvc::filter(pPrime);

	// Set HOLE values to zero
	//drMesh.setHoleValues(TPrime);
	drMesh.setHoleValues(rhoPrime);

	drMesh.setHoleValues(  UPrime);

	drMesh.setHoleValues(  pPrime);
}
//Info << "------------ AFTER FILTERING ----------" << endl;
//Info << "Max / min UPrime   = " << max(UPrime).value() << " / " << min(UPrime).value() << endl;
//Info << "Max / min pPrime   = " << max(pPrime).value() << " / " << min(pPrime).value() << endl;
//Info << "Max / min rhoPrime = " << max(rhoPrime).value() << " / " << min(rhoPrime).value() << endl;
//rhoPrime = rhoMean*(pPrime/pMean - TPrime/TMean);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Map primitive perturbations fields from mesh to linMesh
drMesh.map_nl_to_l(rhoPrime, rhoMean, rho);

drMesh.map_nl_to_l(  UPrime,   UMean,   U); 

drMesh.map_nl_to_l(  pPrime,   pMean,   p_rgh);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Correct boundary conditions for perturbation variables (linMesh)
rhoPrime.correctBoundaryConditions();

UPrime.correctBoundaryConditions();

pPrime.correctBoundaryConditions();

// Reconstruct conservative variables rhoUPrime and rhoEPrime

rhoUPrime = (UMean + UPrime)*(rhoMean + rhoPrime) - rhoUMean;

rhoEPrime = (pMean + pPrime)/gM1 
	     + 0.5*(rhoMean + rhoPrime)*magSqr(UMean + UPrime)
	     - rhoEMean;

//rhoUPrime.correctBoundaryConditions();
//rhoEPrime.correctBoundaryConditions();

//Info << "------------ AFTER CORRECT BC ----------" << endl;
//Info << "Max / min UPrime   = " << max(UPrime).value() << " / " << min(UPrime).value() << endl;
//Info << "Max / min pPrime   = " << max(pPrime).value() << " / " << min(pPrime).value() << endl;
//Info << "Max / min rhoPrime = " << max(rhoPrime).value() << " / " << min(rhoPrime).value() << endl;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
