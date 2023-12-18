// Last stage of RK scheme
if (rkTime.s() == rkTime.nStages()-1)
{
	// Filter perturbation variables
	strFvc::filter(rhoPrime);
	
	strFvc::filter(rhoUPrime);

	strFvc::filter(rhoEPrime);
	
	// Set HOLE values to zero
	drMesh.setHoleValues(rhoPrime);

	drMesh.setHoleValues(  UPrime);

	drMesh.setHoleValues(  pPrime);
}

// Compute primitive perturbation variables (linMesh)
UPrime = (rhoUMean + rhoUPrime)/(rhoMean + rhoPrime) - UMean;

pPrime = gM1*
		(
			  rhoEMean + rhoEPrime 
			- 0.5*(rhoMean + rhoPrime)*magSqr(UMean + UPrime)
		) - pMean;
		
TPrime = TMean*(pPrime/pMean - rhoPrime/rhoMean);
		
// Map primitive perturbations fields from mesh to linMesh
drMesh.map_nl_to_l(rhoPrime, rhoMean, rho);

drMesh.map_nl_to_l(  UPrime,   UMean,   U); 

drMesh.map_nl_to_l(  pPrime,   pMean,   p_rgh);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Correct boundary conditions for perturbation variables (linMesh)
rhoPrime.correctBoundaryConditions();

UPrime.correctBoundaryConditions();

pPrime.correctBoundaryConditions();
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
