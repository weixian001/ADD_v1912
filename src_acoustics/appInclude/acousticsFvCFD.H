#include "fvCFD.H"
#include "dualRegionFvMesh.H"
#include "spatialScheme.H"
#include "filterScheme.H"
#include "lowStorageRungeKuttaScheme.H"
#include "spongeZones.H"

namespace strFvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
	const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
	// Mesh properties
	const strFvMesh& mesh = dynamic_cast<const strFvMesh&>(vf.mesh());
	
	// Get spatial scheme from mesh
	const spatialScheme& sp = mesh.scheme();
	
	GeometricField<Type, fvsPatchField, surfaceMesh> ssf
	(
		mesh.initializeSurfaceSum(vf)
	);
	
	// Create surface field
	tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
	(
	    new GeometricField<Type, fvsPatchField, surfaceMesh>
	    (
	    	IOobject
	    	(
	    		"strFvInterpolate(" + vf.name() + ")",
	    		vf.instance(),
	    		vf.db()
	    	),
	    	sp.alpha()[0]*ssf
	    )
	);
	
	GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsf.ref();
	
	// Interpolate recursively
	for(register label i = 1; i < sp.alpha().size(); i++)
	{
		ssf = mesh.faceNormalSum(mesh.surfaceCmptSum(ssf));
		
		sf += sp.alpha()[i]*ssf;
	}
	
	return tsf;
}

template<class Type>
void filter
(
	GeometricField<Type, fvPatchField, volMesh>& vf
)
{
	// Mesh properties
	const strFvMesh& mesh = dynamic_cast<const strFvMesh&>(vf.mesh());
	
	// Get filter scheme from mesh
	const filterScheme& fl = mesh.filter();
	
	if (fl.F() > SMALL)
	{
		GeometricField<Type, fvsPatchField, surfaceMesh> ssf
		(
			mesh.initializeSurfaceSum(vf)
		);

		//Info << " initializeSurfaceSum(vf) " << endl;
		//Info << "Max / Min ssf = " << max(ssf).value() << " / " << min(ssf).value() << endl;
		
		typedef typename outerProduct<vector, Type>::type oType;
		
		tmp<GeometricField<oType, fvPatchField, volMesh> > tdf
		(
			mesh.surfaceCmptSum(ssf)	// surface to vol
		);

		//Info << " surfaceCmptSum(ssf) " << endl;
                //Info << "Max / Min tdf = " << gMax(mesh.surfaceCmptSum(ssf)) << " / " << gMin(mesh.surfaceCmptSum(ssf)) << endl;
	
                //Info << " faceNormalSum(surfaceCmptSum(ssf)) " << endl;
                //Info << "Max / Min tdf = " << gMax(mesh.faceNormalSum(mesh.surfaceCmptSum(ssf)))
		//	<< " / " << gMin(mesh.faceNormalSum(mesh.surfaceCmptSum(ssf))) << endl;

		GeometricField<oType, fvPatchField, volMesh> df
		(
			fl.beta()[1]*tdf.cref()
		);
		
		// Filter recursively
		for(register label i = 2; i < fl.beta().size(); i++)
		{
			ssf = mesh.faceNormalSum(tdf);	// vol to surface
			
		//Info << " Filter recursively i = " << i << endl;
//Info << "Max / Min ssf = " << max(ssf).value() << " / " << min(ssf).value() << endl;
			
			tdf = mesh.surfaceCmptSum(ssf);
//Info << "Max / Min ssf = " << max(mesh.surfaceCmptSum(ssf)).value() << " / " 
//	<< min(mesh.surfaceCmptSum(ssf)).value() << endl;
		
//Info << "--------beta()["<<i<<"] = " << fl.beta()[i] << endl;	
			df += fl.beta()[i]*tdf.cref();
//Info << "Max / Min df = " << max(df).value() << " / " << min(df).value() << endl;
		}

		//Info << " Filter recursively " << endl;
                //Info << "Max / Min df = " << gMax(df) << " / " << gMin(df) << endl;
		
		const vector avg
		(
			0.5*(mesh.solutionD() + cmptMag(mesh.solutionD()))
		);
		vf -= fl.F()*((df & avg) + (mesh.nSolutionD()*fl.beta()[0])*vf);
		//Info << " vf final " << endl;
                //Info << "Max / Min vf = " << gMax(vf) << " / " << gMin(vf) << endl;
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
	
} // End namespace strFvc
