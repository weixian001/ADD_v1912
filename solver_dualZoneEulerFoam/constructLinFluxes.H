// Interpolate perturbation variables (linMesh)
surfaceScalarField rhoPrimef
(
	"rhoPrimef",
	strFvc::interpolate(rhoPrime)
);

surfaceVectorField UPrimef
(
	"UPrimef",
	strFvc::interpolate(UPrime)
); 

surfaceScalarField pPrimef
(
	"pPrimef",
	strFvc::interpolate(pPrime)
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Perturbation volumetric flux
surfaceScalarField phiPrime("phiPrime", UPrimef & linMesh.Sf());

// Face fluxes
surfaceScalarField Phi_l
(
	"Phi",
	phiMean*rhoPrimef + phiPrime*rhoMeanf
);

surfaceVectorField PhiUp_l
(
	"PhiUp",
	Phi_l*UMeanf + phiMean*rhoMeanf*UPrimef + pPrimef*linMesh.Sf()
);

surfaceScalarField PhiEp_l
(
	"PhiEp",
	zetaf_l*(phiMean*pPrimef + phiPrime*pMeanf) 
	+ Phi_l*keMeanf + phiMean*rhoMeanf*(UMeanf & UPrimef)
);

// Remove contribution of flux from faces next to interface boundaries
/*if (consistent)
{
	drMesh.correctFlux(Phi_l);

	drMesh.correctFlux(PhiUp_l);

	drMesh.correctFlux(PhiEp_l);
}*/

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //
// Activate diffusion in sponge zones
if (diffusionIsActive)
{
	sponges.diffuse(  Phi_l,  rhoPrime);
	
	sponges.diffuse(PhiUp_l, rhoUPrime);
	
	sponges.diffuse(PhiEp_l, rhoEPrime);
}
// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

volScalarField rhoPrimeFlux
(
	"rhoPrimeFlux",
	- fvc::div(Phi_l)
);

volVectorField rhoUPrimeFlux
(
	"rhoUPrimeFlux",
	- fvc::div(PhiUp_l)
);

volScalarField rhoEPrimeFlux
(
	"rhoEPrimeFlux",
	- fvc::div(PhiEp_l)
);

// Viscous fluxes (2nd order)
if (!inviscid_l)
{
	const volSymmTensorField sigmaPrime
	(
		"sigmaPrime", 
		thermo_l.mu()*dev(twoSymm(fvc::grad(UPrimef)))
	);

	rhoUPrimeFlux += fvc::div(sigmaPrime);

	const surfaceScalarField sigmaDotUPrime
	(
		"sigmaDotUPrime",
		((linMesh.Sf() & fvc::interpolate(sigmaPrime)) & UMeanf) 
		+ (sigmaMeanSf & UPrimef)
	);
	
	const volScalarField TPrime
	(
		"TPrime",
		TMean*(pPrime/pMean - rhoPrime/rhoMean)
	);

	rhoEPrimeFlux 
	+= fvc::div(sigmaDotUPrime)
	+  fvc::laplacian(thermo_l.kappa(), TPrime);
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
