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

Class
    Foam::strFvMesh

Description
	Class to handle uniform structured meshes.
	
SourceFiles
    strFvMesh.C

\*---------------------------------------------------------------------------*/

#ifndef strFvMesh_H
#define strFvMesh_H

#include "fvMesh.H"
#include "cellList.H"
#include "surfaceFields.H"
#include "wallFvPatch.H"
#include "spatialScheme.H"
#include "filterScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class strFvMesh Declaration
\*---------------------------------------------------------------------------*/

class strFvMesh
:
    public fvMesh
{
    // Private data

        //- Cell size
        scalar delta_;
        
        //- Solved for dimensions
        vector dims_;
        
        //- Global cell indexing
        globalIndex gCells_;
        
        //- Global cell centroids
        pointField gC_;
        
        //- Unit face normal vector (used in wallReflect)
        tmp<surfaceVectorField> tnf_;
        
        //- Labels indicating face normal directions for internal and
        //  boundary faces (0 - x, 1 - y, 2 - z)
        labelList internalFaceDir_;
        
        labelListList boundaryFaceDir_;
        
        //- Spatial scheme
        autoPtr<spatialScheme> spSch_;
        
        //- Filter scheme
        autoPtr<filterScheme> flSch_;


    // Private Member Functions

        //- No copy construct
        strFvMesh(const strFvMesh&) = delete;

        //- No copy assignment
        void operator=(const strFvMesh&) = delete;
        
        //- Reflect value about normal direction and take average
        template<class Type>
        Type wallReflect(const Type&, const vector&) const;
        
        //- Get component (used in faceNormalSum)
        template<class Type>
        typename innerProduct<vector, Type>::type getCmpt
        (
        	const Type&,
        	const label
        ) const;
        
        //- Add to component (used in surfaceCmptSum)
        template<class Type>
        void addToCmpt
        (
        	Type&,
        	const label,
        	const typename innerProduct<vector, Type>::type&
        ) const;

	//- Get current cell size
        //const vector delta(const label gcelli) const;

        //- Check if point lies inside cell given the global cell index
        bool pointInsideCell
        (
        	const point&,
        	const label&
        ) const;
        
        //- Find global index of cell closest to given point
        bool findGlobalCell
        (
        	const point&,
        	label&,
        	point&
        ) const;
        
        //- Check corners and correct if outside mesh
        bool checkCorners(scalarListList&) const;

public:

    // Constructors

        //- Construct from dictionary
        strFvMesh
        (
        	const IOobject&
        );
        
    // Member Functions

        // Access
        const scalar& delta() const;

		const vector& dims() const;
        
        const globalIndex& gCells() const;
        
        const pointField& gC() const;
        
        const surfaceVectorField& nf() const;
        
        const labelList& internalFaceDir() const;
        
        const labelList& patchFaceDir(const label patchi) const;
        
        const spatialScheme& scheme() const;
        
        const filterScheme& filter() const;
        
        // Linear interpolation
        void calcLinearInterpAddr
        (
        	const point&,
        	const point&,
        	labelList&,
        	scalarList&
        ) const;
        
        // Higher order interpolation
        void calcLagrangeInterAddr
        (
        	const point&,
        	const label,
        	labelList&,
        	scalarList&
        ) const;
        
        // Initialize surface sum by adding values from owner and neigbour
        // cells for each face. Coupled (e.g. cyclic) patches are handled 
        // properly while reflective boundary conditions are applied at wall
        // patches. Zero-gradient applied to all other patches.
        template <class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
		initializeSurfaceSum
		(
			const GeometricField<Type, fvPatchField, volMesh>&
		) const;
        
        // Sum surface field into a volume field by adding values from the
        // owner and neighbour cells of each face. The results from faces of
        // different normal directions (x, y and z) are stored in different
        // components of a higher order tensor. For a scalar field in a general
        // 3D problem, the sums are stored in a vector field with the x, y and
        // z sums stored in components 0, 1 and 2 of the vector field.
        template<class Type>
		tmp
		<
			GeometricField
			<
				typename Foam::outerProduct<vector, Type>::type,
				fvPatchField,
				volMesh
			>
		>
		surfaceCmptSum
		(
			const GeometricField<Type, fvsPatchField, surfaceMesh>&
		) const;
		
		// Sum volume field into a surface field by adding the components 
        // along the face normal direction only. Interpolation involves cells
        // along the face normal direction only.
        template<class Type>
		tmp
		<
			GeometricField
			<
				typename innerProduct<vector, Type>::type,
				fvsPatchField,
				surfaceMesh
			> 
		>
		faceNormalSum
		(
			tmp<GeometricField<Type, fvPatchField, volMesh> >
		) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "strFvMeshI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
