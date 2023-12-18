// Interpolate perturbation variables (linMesh)
surfaceScalarField rhoPrimef
(
	"rhoPrimef",
	strFvc::interpolate(rhoPrime)
);
//Info << "Max / min rhoPrimef" << max(rhoPrimef).value() << " / " << min(rhoPrimef).value() << endl; 

surfaceVectorField UPrimef
(
	"UPrimef",
	strFvc::interpolate(UPrime)
);
//Info << "Max / min UPrimef" << max(UPrimef).value() << " / " << min(UPrimef).value() << endl;

surfaceScalarField pPrimef
(
	"pPrimef",
	strFvc::interpolate(pPrime)
);
//Info << "Max / min pPrimef" << max(pPrimef).value() << " / " << min(pPrimef).value() << endl;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Perturbation volumetric flux
surfaceScalarField phiPrime("phiPrime", UPrimef & linMesh.Sf());

// Face fluxes
surfaceScalarField Phi
(
	"Phi",
	phiMean*rhoPrimef + phiPrime*rhoMeanf
);

//Info << "Max / min phiMean  " << max(phiMean) << " / " << min(phiMean) << endl;
//Info << "Max / min rhoPrimef" << max(rhoPrimef) << " / " << min(rhoPrimef) << endl;
//Info << "Max / min phiPrime " << max(phiPrime) << " / " << min(phiPrime) << endl;
//Info << "Max / min rhoMeanf " << max(rhoMeanf) << " / " << min(rhoMeanf) << endl;
//Info << "Max / min Phi      " << max(Phi).value() << " / " << min(Phi).value() << endl;

surfaceVectorField PhiUp
(
	"PhiUp",
	Phi*UMeanf + phiMean*rhoMeanf*UPrimef + pPrimef*linMesh.Sf()
);

surfaceScalarField PhiEp
(
	"PhiEp",
	zetaf*(phiMean*pPrimef + phiPrime*pMeanf) 
	+ Phi*keMeanf + phiMean*rhoMeanf*(UMeanf & UPrimef)
);

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //
// Activate diffusion in sponge zones
if (diffusionIsActive)
{
	sponges.diffuse(  Phi,  rhoPrime);
	
	sponges.diffuse(PhiUp, rhoUPrime);
	
	sponges.diffuse(PhiEp, rhoEPrime);
}
// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //
//Info << "Begining of conservative Flux" << endl;
volScalarField rhoPrimeFlux
(
	"rhoPrimeFlux",
	- fvc::div(Phi)
);
//Info << "Done ----- rhoPrimeFlux" << endl;

volVectorField rhoUPrimeFlux
(
	"rhoUPrimeFlux",
	- fvc::div(PhiUp)
);
//Info << "Done ----- rhoUPrimeFlux" << endl;

volScalarField rhoEPrimeFlux
(
	"rhoEPrimeFlux",
	- fvc::div(PhiEp)
);
//Info << "Done ---- rhoEPrimeFlux" << endl;

// Viscous fluxes (2nd order)
if (!inviscid)
{
	const volSymmTensorField sigmaPrime
	(
		"sigmaPrime", 
		pthermo.mu()*dev(twoSymm(fvc::grad(UPrimef)))
	);

	rhoUPrimeFlux += fvc::div(sigmaPrime);

	const surfaceScalarField sigmaDotUPrime
	(
		"sigmaDotUPrime",
		((linMesh.Sf() & fvc::interpolate(sigmaPrime)) & UMeanf) 
		+ (sigmaMeanSf & UPrimef)
	);
	
	rhoEPrimeFlux 
	+= fvc::div(sigmaDotUPrime)
	+  fvc::laplacian(pthermo.kappa(), TPrime);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //
// Activate absorption in sponge zones
if (spongeIsActive)
{
	sponges.absorb( rhoPrimeFlux,  rhoPrime);

	sponges.absorb(rhoUPrimeFlux, rhoUPrime);

	sponges.absorb(rhoEPrimeFlux, rhoEPrime);
}
// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
