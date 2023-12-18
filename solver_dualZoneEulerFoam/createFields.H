Info<< "Reading thermophysical properties\n" << endl;

autoPtr<psiThermo> pThermo_l
(
    psiThermo::New(linMesh)
);
psiThermo& thermo_l = pThermo_l();

autoPtr<psiThermo> pThermo
(
    psiThermo::New(mesh)
);
psiThermo& thermo = pThermo();

// gM1 - used to compute pressure from total energy
const volScalarField gM1_l("gM1", thermo_l.gamma() - 1.0);

const volScalarField gM1("gM1", thermo.gamma() - 1.0);

// R - used to compute temperature
const volScalarField R("R", thermo.Cp() - thermo.Cv());

// Interpolate gamma
const surfaceScalarField gammaf_l
(
	"gammaf",
	fvc::interpolate(thermo_l.gamma())
);

const surfaceScalarField gammaf
(
	"gammaf",
	fvc::interpolate(thermo.gamma())
);

// Interpolate thermo property z = gamma/(gamma-1)
const surfaceScalarField zetaf_l
(
	"zetaf",
	gammaf_l/(gammaf_l - 1.0)
);

const surfaceScalarField zetaf
(
	"zetaf",
	gammaf/(gammaf - 1.0)
);

// Cell-based length scale used for shock sensor computation
const surfaceScalarField delta(1.0/mesh.surfaceInterpolation::deltaCoeffs());

const volScalarField h
(
	fvc::cellReduce
	(
		delta,
		maxEqOp<scalar>(),
		scalar(0.0)
	)
);

// * * * * * * * * * * * Mean State Variables (primitive) * * * * * * * * * //

// p
volScalarField& pMean = thermo_l.p();

volScalarField& p = thermo.p();

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
	thermo_l.rho()
);

volScalarField rho
(
	IOobject
	(
		"rho",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	thermo.rho(),
	p.boundaryField().types()
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

volVectorField U
(
	IOobject
	(
		"U",
		runTime.timeName(),
		mesh,
		IOobject::MUST_READ,
		IOobject::AUTO_WRITE
	),
	mesh
);

// T
volScalarField& TMean = thermo_l.T();

volScalarField& T = thermo.T();


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

// TPrime
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

volVectorField rhoU
(
	IOobject
	(
		"rhoU",
		runTime.timeName(),
		mesh
	),
	rho*U
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
	pMean/gM1_l + 0.5*rhoMean*magSqr(UMean)
);

volScalarField rhoE
(
	IOobject
	(
		"rhoE",
		runTime.timeName(),
		mesh
	),
	p/gM1 + 0.5*rho*magSqr(U)
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
	pPrime/gM1_l
	+ 0.5*
	(
		(rhoMean + rhoPrime)*magSqr(UMean + UPrime)
		- rhoMean*magSqr(UMean)
	)
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Temporary variables used for Runge-Kutta scheme
volScalarField rhoTmp
(
	IOobject
	(
		"rhoTmp",
		runTime.timeName(),
		mesh
	),
	rho
);

volVectorField rhoUTmp
(
	IOobject
	(
		"rhoUTmp",
		runTime.timeName(),
		mesh
	),
	rhoU
);

volScalarField rhoETmp
(
	IOobject
	(
		"rhoETmp",
		runTime.timeName(),
		mesh
	),
	rhoE
);

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

// Linear zone
spongeZones sponges(linMesh);

sponges.sigma().write();

sponges.omegaf().write();

const bool diffusionIsActive(max(sponges.omegaf()).value() > SMALL);

const bool spongeIsActive(max(sponges.sigma()).value() > SMALL);

//  * * * * * * * * * * Fluxes (for non-linear region) * * * * * * * * * * * //

surfaceScalarField phi
(
    IOobject
    (
        "phi",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar(rho.dimensions()*dimVelocity*dimArea)
);

surfaceVectorField phiUp
(
    IOobject
    (
        "phiUp",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedVector(rhoU.dimensions()*dimVelocity*dimArea)
);

surfaceScalarField phiEp
(
    IOobject
    (
        "phiEp",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar(rhoE.dimensions()*dimVelocity*dimArea)
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Initialize refGradient and valueFraction for interface patches
drMesh.initializeInterfaces(U);

drMesh.initializeInterfaces(p);

drMesh.initializeInterfaces(T);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

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

// Face unit normal vectors on mesh
const surfaceVectorField Nf("Nf", mesh.Sf()/mesh.magSf());

surfaceScalarField plus
(
    IOobject
    (
        "plus",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar("plus", dimless, 1.0)
);

surfaceScalarField minus
(
    IOobject
    (
        "minus",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar("minus", dimless, -1.0)
);

// * * * * * * * * * * * * * Interpolated Values * * * * * * * * * * * * * * //

// rho
surfaceScalarField rho_pos
(
    IOobject
    (
        "rho_pos",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar("rho_pos", rho.dimensions(), 0.0)
);

surfaceScalarField rho_neg
(
    IOobject
    (
        "rho_neg",
        runTime.timeName(),
        mesh
    ),
    rho_pos
);

// U
surfaceVectorField U_pos
(
    IOobject
    (
        "U_pos",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedVector("U_pos", U.dimensions(), vector::zero)
);

surfaceVectorField U_neg
(
    IOobject
    (
        "U_neg",
        runTime.timeName(),
        mesh
    ),
    U_pos
);

// p
surfaceScalarField p_pos
(
    IOobject
    (
        "p_pos",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar("p_pos", p.dimensions(), 0.0)
);

surfaceScalarField p_neg
(
    IOobject
    (
        "p_neg",
        runTime.timeName(),
        mesh
    ),
    p_pos
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Shock sensor
volScalarField sensor
(
    IOobject
    (
        "sensor",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar(0.0)
);

// Blending parameter
surfaceScalarField theta
(
    IOobject
    (
        "theta",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar(0.0)
);

// Viscosity
bool inviscid(true);

if (max(thermo.mu()().primitiveField()) > 0.0)
{
    inviscid = false;
}

bool inviscid_l(true);

if (max(thermo_l.mu()().primitiveField()) > 0.0)
{
    inviscid_l = false;
}

surfaceVectorField sigmaMeanSf
(
	IOobject
    (
        "sigmaMeanSf",
        runTime.timeName(),
        linMesh
    ),
    linMesh.Sf() 
    & fvc::interpolate(thermo_l.mu()*dev(twoSymm(fvc::grad(UMeanf))))
);

// Turbulence model
/*Info<< "Creating turbulence model\n" << endl;

autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New
    (
        rho,
        U,
        phi,
        thermo
    )
);*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
