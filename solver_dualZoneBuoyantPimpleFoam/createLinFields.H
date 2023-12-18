Info<< "Reading thermophysical properties\n" << endl;

autoPtr<rhoThermo> pThermo_
(
    rhoThermo::New(linMesh)
);
rhoThermo& pthermo = pThermo_();

// Viscosity
bool inviscid(true);

if (max(pthermo.mu()().primitiveField()) > 0.0)
{
    inviscid = false;
}

// gM1 - used to compute pressure from total energy
const volScalarField gM1("gM1", pthermo.gamma() - 1.0);

// Interpolate gamma
const surfaceScalarField gammaf
(
	"gammaf",
	fvc::interpolate(pthermo.gamma())
);

// Interpolate thermo property z = gamma/(gamma-1)
const surfaceScalarField zetaf
(
	"zetaf",
	gammaf/(gammaf - 1.0)
);

// * * * * * * * * * * * Mean State Variables (primitive) * * * * * * * * * //

// p
volScalarField& pMean = pthermo.p();

// rho
volScalarField rhoMean
(
	IOobject
	(
		"rho",
		runTime.timeName(),
		linMesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	pthermo.rho()
);

// U
volVectorField UMean
(
	IOobject
	(
		"U",
		runTime.timeName(),
		linMesh,
		IOobject::MUST_READ,
		IOobject::AUTO_WRITE
	),
	linMesh
);

// * * * * * * * * * * Perturbed State Variables (primitive) * * * * * * * * //

// rhoPrime
volScalarField rhoPrime
(
	IOobject
	(
		"rhoPrime",
		runTime.timeName(),
		linMesh,
		IOobject::MUST_READ,
		IOobject::AUTO_WRITE
	),
	linMesh
);

// UPrime
volVectorField UPrime
(
	IOobject
	(
		"UPrime",
		runTime.timeName(),
		linMesh,
		IOobject::MUST_READ,
		IOobject::AUTO_WRITE
	),
	linMesh
);

// pPrime
volScalarField pPrime
(
	IOobject
	(
		"pPrime",
		runTime.timeName(),
		linMesh,
		IOobject::MUST_READ,
		IOobject::AUTO_WRITE
	),
	linMesh
);

// * * * * * * * * * * * Reinitializing Mean Fields * * * * * * * * * * * * //
/*
pMean += pPrime;
rhoMean += rhoPrime;
UMean += UPrime;

runTime.writeNow();

return 0;
*/
// * * * * * * * * * * * Mean State Variables (conservative) * * * * * * * * //

// rhoU
volVectorField rhoUMean
(
	IOobject
	(
		"rhoU",
		runTime.timeName(),
		linMesh
	),
	rhoMean*UMean
);

// rhoE
volScalarField rhoEMean
(
	IOobject
	(
		"rhoE",
		runTime.timeName(),
		linMesh
	),
	pMean/gM1 + 0.5*rhoMean*magSqr(UMean)
);

// * * * * * * * * * Perturbed State Variables (conservative) * * * * * * * * //

// rhoUPrime
volVectorField rhoUPrime
(
	IOobject
	(
		"rhoUPrime",
		runTime.timeName(),
		linMesh
	),
	(rhoMean + rhoPrime)*(UMean + UPrime) - rhoUMean
);

// rhoEPrime
volScalarField rhoEPrime
(
	IOobject
	(
		"rhoEPrime",
		runTime.timeName(),
		linMesh
	),
	pPrime/gM1
	+ 0.5*
	(
		(rhoMean + rhoPrime)*magSqr(UMean + UPrime)
		- rhoMean*magSqr(UMean)
	)
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Temporary variables used for Runge-Kutta scheme
volScalarField rhoPrimeTmp
(
	IOobject
	(
		"rhoPrimeTmp",
		runTime.timeName(),
		linMesh
	),
	rhoPrime
);

volVectorField rhoUPrimeTmp
(
	IOobject
	(
		"rhoUPrimeTmp",
		runTime.timeName(),
		linMesh
	),
	rhoUPrime
);

volScalarField rhoEPrimeTmp
(
	IOobject
	(
		"rhoEPrimeTmp",
		runTime.timeName(),
		linMesh
	),
	rhoEPrime
);

// * * * * * * * * * * * * Sponge Layer Properties * * * * * * * * * * * * * //

spongeZones sponges(linMesh);

sponges.sigma().write();

sponges.omegaf().write();

const bool diffusionIsActive(max(sponges.omegaf()).value() > SMALL);

const bool spongeIsActive(max(sponges.sigma()).value() > SMALL);

// * * * * * * * * * * * * * Interpolated Values * * * * * * * * * * * * * * //

// Interpolate mean state variables on linMesh
const surfaceScalarField rhoMeanf
(
	"rhof",
	strFvc::interpolate(rhoMean)
);

const surfaceScalarField pMeanf
(
	"pf",
	strFvc::interpolate(pMean)
);

const surfaceVectorField UMeanf
(
	"Uf",
	strFvc::interpolate(UMean)
);

// Mean state volumeric flux on linMesh
const surfaceScalarField phiMean("phi", UMeanf & linMesh.Sf());

// Mean state specific kinetic energy on linMesh
const surfaceScalarField keMeanf("kef", 0.5*magSqr(UMeanf));

surfaceVectorField sigmaMeanSf
(
	IOobject
    (
        "sigmaMeanSf",
        runTime.timeName(),
        linMesh
    ),
    linMesh.Sf() 
    & fvc::interpolate(pthermo.mu()*dev(twoSymm(fvc::grad(UMeanf))))
);

volScalarField TMean
(
	IOobject
    (
        "T",
        runTime.timeName(),
        linMesh
    ),
    pMean/((pthermo.Cp() - pthermo.Cv())*rhoMean)
);

volScalarField TPrime
(
	IOobject
    (
        "TPrime",
        runTime.timeName(),
        linMesh
    ),
    TMean*(pPrime/pMean - rhoPrime/rhoMean)
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
