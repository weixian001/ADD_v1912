#include "wallPolyPatch.H"
#include "cyclicFvPatch.H"
#include "processorFvPatch.H"

// Functions used for implementing reflective boundaries at walls
void wallReflect
(
	scalarField& sf,
	const vectorField&
)
{}

void wallReflect
(
	vectorField& vf,
	const vectorField& nf
)
{
	vf -= (vf & nf)*nf;
}

//- Interpolate field vf according to direction dir
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> interpolate2
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const surfaceVectorField& Nf
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
    (
    	fvc::interpolate(vf) 
    );
    
    GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsf.ref();
    
    // Transfer values from vf to sf for uncoupled patches
    forAll(vf.mesh().boundary(), patchi)
	{
		const polyPatch& pp = vf.mesh().boundary()[patchi].patch();
		
		if (isA<wallPolyPatch>(pp))
		{
			const vectorField& nf = Nf.boundaryField()[patchi];
			
			wallReflect(sf.boundaryFieldRef()[patchi], nf);
		}
	}

    return tsf;
}

//- Interpolate field vf according to direction dir
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> dInterpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const surfaceScalarField& dir,
    const surfaceVectorField& Nf,
    const word& reconFieldName = word::null
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> tsf
    (
        fvc::interpolate
        (
            vf,
            dir,
            "reconstruct("
          + (reconFieldName != word::null ? reconFieldName : vf.name())
          + ')'
        )
    );

    GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsf.ref();

    sf.rename(vf.name() + '_' + dir.name());
    
    // Transfer values from vf to sf for uncoupled patches
    forAll(vf.mesh().boundary(), patchi)
	{
		const polyPatch& pp = vf.mesh().boundary()[patchi].patch();
		
		if (isA<wallPolyPatch>(pp))
		{
			const vectorField& nf = Nf.boundaryField()[patchi];
			
			wallReflect(sf.boundaryFieldRef()[patchi], nf);
		}
	}

    return tsf;
}

//- Interpolate field vf using limited gradient
template<class Type>
void lInterpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    GeometricField<Type, fvsPatchField, surfaceMesh>& sf_pos,
    GeometricField<Type, fvsPatchField, surfaceMesh>& sf_neg
)
{
	const fvMesh& mesh = vf.mesh();
	
	typedef typename outerProduct<vector, Type>::type gradType;
	
	typedef GeometricField<gradType, fvPatchField, volMesh> gradVfType;
	
	// Limited gradient of vf
	const gradVfType gradVf(fvc::grad(vf, "limited"));
	
    const labelUList& owner = mesh.owner();
    const labelUList& neighbour = mesh.neighbour();

    const volVectorField& C = mesh.C();
    const surfaceVectorField& Cf = mesh.Cf();

    forAll(owner, facei)
    {
        label own = owner[facei];
        label nei = neighbour[facei];
        
        const vector& cf = Cf[facei];
        
        sf_pos[facei] = vf[own] + ((cf - C[own]) & gradVf[own]);
        sf_neg[facei] = vf[nei] + ((cf - C[nei]) & gradVf[nei]);
    }
    
    const typename 
    GeometricField<Type, fvPatchField, volMesh>::Boundary& bf 
    	= vf.boundaryField();

    forAll(bf, patchi)
    {
        const fvPatchField<Type>& pf = bf[patchi];
        
        const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
        
        const vectorField& pCfOwn = Cf.boundaryField()[patchi];

        Field<Type>& bsf_pos = sf_pos.boundaryFieldRef()[patchi];
        
        Field<Type>& bsf_neg = sf_neg.boundaryFieldRef()[patchi];
        
        if (pf.coupled())
        {
            const polyPatch& pp = mesh.boundary()[patchi].patch();
            
            const Field<Type>& pvfNei 
		        = vf.boundaryField()[patchi].patchNeighbourField();
		        	
	        const Field<gradType>& pgradVfNei
	        	= gradVf.boundaryField()[patchi].patchNeighbourField();
	        	
	        vectorField pCfNei(pp.size()), pCNei(pp.size());
	        
            if (isA<cyclicPolyPatch>(pp))
            {
		        cyclicFvPatch cfvp(pp, mesh.boundary());
		        
		        pCfNei = cfvp.neighbPatch().Cf();
		        
		        pCNei  = cfvp.neighbPatch().Cn();
		        
		        Field<Type>& nbsf_neg 
		        	= sf_neg.boundaryFieldRef()[cfvp.neighbPatchID()];
		        
		        forAll(pOwner, pFacei)
			    {
			        label own = pOwner[pFacei];
			        
			        bsf_pos[pFacei] 
			        	= vf[own] + ((pCfOwn[pFacei] - C[own]) & gradVf[own]);
			        	
			        nbsf_neg[pFacei] = bsf_pos[pFacei];
			    }
			}
			else if (isA<processorPolyPatch>(pp))
			{
		        processorFvPatch pfvp(pp, mesh.boundary());
		        
		        const processorPolyPatch& ppp = pfvp.procPolyPatch();
		        
		        pCfNei = ppp.neighbFaceCentres();
		        
		        pCNei  = ppp.neighbFaceCellCentres();
		        
		        forAll(pOwner, pFacei)
			    {
			        label own = pOwner[pFacei];
			        
			        bsf_pos[pFacei] 
			        	= vf[own] + ((pCfOwn[pFacei] - C[own]) & gradVf[own]);
			        	
			        bsf_neg[pFacei] 
			        	= pvfNei[pFacei] + 
			        	(
			        		(pCfNei[pFacei] - pCNei[pFacei])
			        		& pgradVfNei[pFacei]
			        	);
			    }
			}
        }
        else
        {
            bsf_neg = pf; // set to boundary value
            
            forAll(pOwner, pFacei)
            {
                label own = pOwner[pFacei];
                
                bsf_pos[pFacei]
                	= vf[own] + ((pCfOwn[pFacei] - C[own]) & gradVf[own]);
            }
        }
    }
}
