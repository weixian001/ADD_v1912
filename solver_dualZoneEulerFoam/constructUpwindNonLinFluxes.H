// Directed interpolation of primitive variables (mesh)
if (useLimited)
{
	lInterpolate(rho, rho_pos, rho_neg);

	lInterpolate(p, p_pos, p_neg);

	lInterpolate(U, U_pos, U_neg);
}
else
{
	rho_pos = dInterpolate(rho,  plus, Nf);

	rho_neg = dInterpolate(rho, minus, Nf);

	p_pos   = dInterpolate(  p,  plus, Nf);

	p_neg   = dInterpolate(  p, minus, Nf);

	U_pos   = dInterpolate(  U,  plus, Nf);

	U_neg   = dInterpolate(  U, minus, Nf);
}

// Central intrpolation primitive variables (mesh)
surfaceScalarField rhof("rhof", interpolate2(rho,   Nf));

surfaceScalarField   pf(  "pf", interpolate2(  p,   Nf));

surfaceVectorField   Uf(  "Uf", interpolate2(  U,   Nf));

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Shock sensor
sensor = DucrosSensor(rho, p, U, h, thermo.gamma());

// Intermediate state velocity
surfaceVectorField Ustar
(
	IOobject
	(
	    "Ustar",
	    runTime.timeName(),
	    mesh
	),
	mesh,
	dimensionedVector(dimVelocity)
);

forAll(mesh.neighbour(), facei)
{
	scalar phic, phiEpc; vector phiUpc;
	
	fluxCentral
	(
		phic,
		phiUpc,
		phiEpc,
		Nf   [facei],
		zetaf[facei],
		rhof [facei],
		Uf   [facei],
		pf   [facei]
	);
	
	scalar phiu, phiEpu; vector phiUpu, Ustaru;
	
	fluxUpwind
	(
		phiu,
		phiUpu,
		phiEpu,
		Ustaru,
		Nf     [facei],
		gammaf [facei],
		rho_pos[facei],
		rho_neg[facei],
		U_pos  [facei],
		U_neg  [facei],
		p_pos  [facei],
		p_neg  [facei]
	);
	
	//const scalar& thetaf = theta.primitiveField()[facei];
	const scalar thetaf
	(
		max
		(
			sensor[mesh.owner()    [facei]],
			sensor[mesh.neighbour()[facei]]
		)
	);
	
	const scalar oneMinusThetaf(1.0 - thetaf);
	
	phi  .primitiveFieldRef()[facei]
		= thetaf*phiu   + oneMinusThetaf*phic;
	
	phiUp.primitiveFieldRef()[facei]
		= thetaf*phiUpu + oneMinusThetaf*phiUpc;
	
	phiEp.primitiveFieldRef()[facei]
		= thetaf*phiEpu + oneMinusThetaf*phiEpc;
		
	Ustar.primitiveFieldRef()[facei]
		= thetaf*Ustaru + oneMinusThetaf*Uf[facei];
}

forAll(mesh.boundary(), patchi)
{
	if (mesh.boundary()[patchi].coupled())
	{
		const scalarField ownSensor
		(
			sensor.boundaryField()[patchi].patchInternalField()
		);
		
		const scalarField neiSensor
		(
			sensor.boundaryField()[patchi].patchNeighbourField()
		);
		
		forAll(phi.boundaryField()[patchi], facei)
		{
			scalar phic, phiEpc; vector phiUpc;
		
			fluxCentral
			(
				phic,
				phiUpc,
				phiEpc,
				Nf   .boundaryField()[patchi][facei],
				zetaf.boundaryField()[patchi][facei],
				rhof .boundaryField()[patchi][facei],
				Uf   .boundaryField()[patchi][facei],
				pf   .boundaryField()[patchi][facei]
			);
			
			scalar phiu, phiEpu; vector phiUpu, Ustaru;
			
			fluxUpwind
			(
				phiu,
				phiUpu,
				phiEpu,
				Ustaru,
				Nf     .boundaryField()[patchi][facei],
				gammaf .boundaryField()[patchi][facei],
				rho_pos.boundaryField()[patchi][facei],
				rho_neg.boundaryField()[patchi][facei],
				U_pos  .boundaryField()[patchi][facei],
				U_neg  .boundaryField()[patchi][facei],
				p_pos  .boundaryField()[patchi][facei],
				p_neg  .boundaryField()[patchi][facei]
			);
			
			//const scalar& thetaf = theta.boundaryField()[patchi][facei];
			const scalar thetaf
			(
				max
				(
					ownSensor[facei],
					neiSensor[facei]
				)
			);
			
			const scalar oneMinusThetaf(1.0 - thetaf);
			
			phi  .boundaryFieldRef()[patchi][facei]
				= thetaf*phiu   + oneMinusThetaf*phic;
			
			phiUp.boundaryFieldRef()[patchi][facei]
				= thetaf*phiUpu + oneMinusThetaf*phiUpc;
			
			phiEp.boundaryFieldRef()[patchi][facei]
				= thetaf*phiEpu + oneMinusThetaf*phiEpc;
				
			Ustar.boundaryFieldRef()[patchi][facei]
				= thetaf*Ustaru + oneMinusThetaf*Uf.boundaryField()[patchi][facei];
		}
	}
	else
	{
		forAll(phi.boundaryField()[patchi], facei)
		{
			scalar phic, phiEpc; vector phiUpc;
		
			fluxCentral
			(
				phic,
				phiUpc,
				phiEpc,
				Nf   .boundaryField()[patchi][facei],
				zetaf.boundaryField()[patchi][facei],
				rhof .boundaryField()[patchi][facei],
				Uf   .boundaryField()[patchi][facei],
				pf   .boundaryField()[patchi][facei]
			);
			
			phi  .boundaryFieldRef()[patchi][facei] = phic;

			phiUp.boundaryFieldRef()[patchi][facei] = phiUpc;

			phiEp.boundaryFieldRef()[patchi][facei] = phiEpc;
			
			Ustar.boundaryFieldRef()[patchi][facei]
				= Uf.boundaryField()[patchi][facei];
		}
	}
}

// Multiply by face area
phi   *= mesh.magSf(); phi.dimensions()   /= mesh.magSf().dimensions();

phiUp *= mesh.magSf(); phiUp.dimensions() /= mesh.magSf().dimensions();

phiEp *= mesh.magSf(); phiEp.dimensions() /= mesh.magSf().dimensions();

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

volScalarField rhoFlux
(
	"rhoFlux",
	- fvc::div(phi)
);

volVectorField rhoUFlux
(
	"rhoUFlux",
	- fvc::div(phiUp)
);

volScalarField rhoEFlux
(
	"rhoEFlux",
	- fvc::div(phiEp)
);

// Viscous fluxes (2nd order)
if (!inviscid)
{
	const volSymmTensorField sigma
	(
		"sigma", 
		thermo.mu()*dev(twoSymm(fvc::grad(U)))
	);

	rhoUFlux += fvc::div(sigma);

	const surfaceScalarField sigmaDotU
	(
		"sigmaDotU",
		(mesh.Sf() & fvc::interpolate(sigma)) & Ustar
	);

	rhoEFlux 
	+= fvc::div(sigmaDotU)
	+  fvc::laplacian(thermo.kappa(), thermo.T());
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
