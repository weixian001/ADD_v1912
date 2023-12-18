//
// createDualRegionFvMesh.H
// ~~~~~~~~~~~~~~~~~~~~~~~~

	Info<< "Create dual region fvMesh for time = "
		<< runTime.timeName() << nl << endl;
		
	dualRegionFvMesh drMesh(runTime);
	
	// Const reference to linear mesh
	const fvMesh& linMesh = drMesh.linMesh();
	
	// Non-const reference to nonLinear mesh 
	// (for creation of pimpleControl)
	fvMesh& mesh = drMesh.nonLinMesh();
	
	// Create Runge-Kutta time scheme from linMesh
	autoPtr<lowStorageRungeKuttaScheme> prkTime
	(
		lowStorageRungeKuttaScheme::New(linMesh)
	);

	lowStorageRungeKuttaScheme& rkTime = prkTime();
