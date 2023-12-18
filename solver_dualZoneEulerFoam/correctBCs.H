// Last stage of RK scheme
//if (rkTime.s() == rkTime.nStages()-1)
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

// Map conservative volume perturbations fields from mesh to linMesh
drMesh.map_nl_to_l( rhoPrime,  rhoMean,  rho);

drMesh.map_nl_to_l(rhoUPrime, rhoUMean, rhoU);

drMesh.map_nl_to_l(rhoEPrime, rhoEMean, rhoE);

// Compute primitive perturbation variables (linMesh)
UPrime = (rhoUMean + rhoUPrime)/(rhoMean + rhoPrime) - UMean;

pPrime = gM1_l
	*(rhoEMean + rhoEPrime - 0.5*(rhoMean + rhoPrime)*magSqr(UMean + UPrime))
	- pMean;
		
TPrime = TMean*(pPrime/pMean - rhoPrime/rhoMean);

// Add acoustic sources
//fvOptions.correct(pPrime);

// Correct boundary conditions for perturbation variables (linMesh)
UPrime.correctBoundaryConditions();

pPrime.correctBoundaryConditions();

rhoPrime.correctBoundaryConditions();
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Compute primitive variables (mesh)
U = rhoU/rho;

p = gM1*(rhoE - 0.5*rho*magSqr(U));

T = p/(R*rho); // ideal gas

// Correct interface patch reference values before updating boundary conditions
drMesh.map_l_to_ifp(U, UMean, UPrime);

drMesh.map_l_to_ifp(p, pMean, pPrime);

drMesh.map_l_to_ifp(T, TMean, TPrime);

// Correct boundary conditions for state variables (mesh)
U.correctBoundaryConditions();

p.correctBoundaryConditions();

T.correctBoundaryConditions();

// Explicit correction for density boundary field based on ideal gas law as
// pressure and temperature boundary fields have already been computed.
rho.boundaryFieldRef()
	= p.boundaryField()/(R.boundaryField()*T.boundaryField());
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
