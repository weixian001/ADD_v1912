/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2011 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                            | Copyright (C) 2019 U S Vevek, NTU, Singapore
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Foam::tmp<Foam::GeometricField<Type, Foam::fvsPatchField, Foam::surfaceMesh> >
Foam::strFvMesh::initializeSurfaceSum
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
	const fvMesh& mesh = vf.mesh();
	
	const labelUList& own = mesh.owner();
    
    const labelUList& nei = mesh.neighbour();
	
	// Create surface field
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> tsf
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "+("+vf.name()+")+",
                vf.instance(),
                vf.db()
            ),
            mesh,
            dimensioned<Type>(vf.dimensions()) // set to zero
        )
    );
    
    GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsf.ref();
    
    forAll(sf, facei)
    {
    	sf[facei] = vf[own[facei]] + vf[nei[facei]];
    }
    
    forAll(sf.boundaryField(), patchi)
    {
    	const fvPatch& fvp = mesh.boundary()[patchi];
    	
    	Field<Type>& psf = sf.boundaryFieldRef()[patchi];
    	
    	const Field<Type> pvf
    	(
    		vf.boundaryField()[patchi].patchInternalField()
    	);
    	
    	if (fvp.coupled()) // perform linear interpolation
    	{
    		const Field<Type> pvfNei
    		(
    			vf.boundaryField()[patchi].patchNeighbourField()
    		);
    			
    		forAll(psf, facei)
			{
				psf[facei] = pvf[facei] + pvfNei[facei];
			}
    	}
    	else if (isA<wallFvPatch>(fvp)) // apply reflection
    	{
    		const vectorField& pnf = this->nf().boundaryField()[patchi];
    		
    		forAll(psf, facei)
			{
				psf[facei] = 2.0*wallReflect(pvf[facei], pnf[facei]);
			}
    	}
    	else // apply zeroGradient (for now)
    	{
    		psf = 2.0*pvf;
    	}
    }
    
    return tsf;
}

template<class Type>
Foam::tmp
<
	Foam::GeometricField
	<
		typename Foam::outerProduct<Foam::vector, Type>::type,
		Foam::fvPatchField,
		Foam::volMesh
	>
>
Foam::strFvMesh::surfaceCmptSum
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sf
) const
{
	const fvMesh& mesh = sf.mesh();
	
	const labelUList& own = mesh.owner();
    
    const labelUList& nei = mesh.neighbour();
    
    typedef typename outerProduct<vector, Type>::type oType;
	
	// Create surface field
    tmp<GeometricField<oType, fvPatchField, volMesh>> tvf
    (
        new GeometricField<oType, fvPatchField, volMesh>
        (
            IOobject
            (
                "+["+sf.name()+"]+",
                sf.instance(),
                sf.db()
            ),
            mesh,
            dimensioned<oType>(sf.dimensions()) // set to zero
        )
    );
    
    GeometricField<oType, fvPatchField, volMesh>& vf = tvf.ref();
    
    forAll(sf, facei)
    {
    	const label dir = this->internalFaceDir()[facei];
    	
    	const label owni = own[facei], neii = nei[facei];
    	
    	const Type& sfi = sf[facei];
    	
    	addToCmpt(vf[owni], dir, sfi);
    	
    	addToCmpt(vf[neii], dir, sfi);
    }
    
    forAll(sf.boundaryField(), patchi)
    {
    	const fvPatch& fvp = mesh.boundary()[patchi];
    	
    	const Field<Type>& psf = sf.boundaryField()[patchi];
    	
    	const labelList& pFaceDirs = this->patchFaceDir(patchi);
    	
    	const labelList& pCells = fvp.faceCells();
    	
	forAll(psf, facei)
    	{
    		const label dir = pFaceDirs[facei];
    		
    		const label pcelli = pCells[facei];
    		
    		addToCmpt(vf[pcelli], dir, psf[facei]);
    	}
    }
    
    return tvf;
}

template<class Type>
Foam::tmp
<
	Foam::GeometricField
	<
		typename Foam::innerProduct<Foam::vector, Type>::type,
		Foam::fvsPatchField,
		Foam::surfaceMesh
	> 
>
Foam::strFvMesh::faceNormalSum
(
    tmp<GeometricField<Type, fvPatchField, volMesh> > tvf
) const
{
	GeometricField<Type, fvPatchField, volMesh>& vf = tvf.ref();
	
	const fvMesh& mesh = vf.mesh();
	
	const labelUList& own = mesh.owner();
    
    const labelUList& nei = mesh.neighbour();
    
    typedef typename innerProduct<vector, Type>::type iType;
	
	// Create surface field
    tmp<GeometricField<iType, fvsPatchField, surfaceMesh>> tsf
    (
        new GeometricField<iType, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "+("+vf.name()+")+",
                vf.instance(),
                vf.db()
            ),
            mesh,
            dimensioned<iType>(vf.dimensions()) // set to zero
        )
    );
    
    GeometricField<iType, fvsPatchField, surfaceMesh>& sf = tsf.ref();
    
    forAll(sf, facei)
    {
    	const label dir = this->internalFaceDir()[facei];
    	
    	sf[facei]
    		= getCmpt(vf[own[facei]], dir) + getCmpt(vf[nei[facei]], dir);
    }
    
    forAll(sf.boundaryField(), patchi)
    {
    	const fvPatch& fvp = mesh.boundary()[patchi];
    	
    	Field<iType>& psf = sf.boundaryFieldRef()[patchi];
    	
    	const Field<Type> pvf
    	(
    		vf.boundaryField()[patchi].patchInternalField()
    	);
    	
    	const labelList& pFaceDirs = this->patchFaceDir(patchi);
    	
    	if (fvp.coupled()) // perform linear interpolation
    	{
    		const Field<Type> pvfNei
    		(
    			vf.boundaryField()[patchi].patchNeighbourField()
    		);
    		
    		forAll(psf, facei)
			{
				const label dir = pFaceDirs[facei];
				
				psf[facei]
					= getCmpt(pvf[facei], dir) + getCmpt(pvfNei[facei], dir);
			}
    	}
    	else if (isA<wallFvPatch>(fvp)) // apply reflection
    	{
    		const vectorField& pnf = this->nf().boundaryField()[patchi];
    		
    		forAll(psf, facei)
			{
				const label dir = pFaceDirs[facei];
				
				psf[facei]
					= 2.0*wallReflect(getCmpt(pvf[facei], dir), pnf[facei]);
			}
    	}
    	else // apply zeroGradient (for now)
    	{
    		forAll(psf, facei)
			{
				const label dir = pFaceDirs[facei];
				
				psf[facei] = 2.0*getCmpt(pvf[facei], dir);
			}
    	}
    }
    
    tvf.clear();
    
    return tsf;
}

/*
namespace Foam	// To work around the g++ bug
{

template<>
scalar strFvMesh::wallReflect<scalar>
(
        const scalar& s,
        const vector& n
) const
{
        return s;
}

template<>
vector strFvMesh::wallReflect<vector>
(
        const vector& v,
        const vector& n
) const
{
        return v - ((v & n)*n);
}

template<>
scalar strFvMesh::getCmpt<vector>
(
        const vector& v,
        const label d
) const
{
        return v[d];
}

template<>
vector strFvMesh::getCmpt<tensor>
(
        const tensor& t,
        const label d
) const
{
        return vector(t[d], t[d+3], t[d+6]);
}

template<>
void strFvMesh::addToCmpt<vector>
(
        vector& v,
        const label d,
        const scalar& s
) const
{
        v[d] += s;
}

template<>
void strFvMesh::addToCmpt<tensor>
(
        tensor& t,
        const label d,
        const vector& v
) const
{
        t[d]   += v[0];
        t[d+3] += v[1];
        t[d+6] += v[2];
}

}*/
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //
