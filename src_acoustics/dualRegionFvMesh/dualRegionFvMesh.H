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
    Foam::dualRegionFvMesh

Description
	An fvMesh class to handle interpolation between two coupled regions.
	The full mesh is split into two regions: linear and non-linear. 
	Interpolation from the non-linear region to the linear region is 

SourceFiles
    dualRegionFvMeshI.H
    dualRegionFvMesh.C
    dualRegionFvMeshIO.C

\*---------------------------------------------------------------------------*/

#ifndef dualRegionFvMesh_H
#define dualRegionFvMesh_H

#include "regionProperties.H"
#include "meshToMesh.H"
#include "cellVolumeWeightMethod.H"
#include "syncTools.H"
#include "strFvMesh.H"
#include "mixedFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class dualRegionFvMesh Declaration
\*---------------------------------------------------------------------------*/

class dualRegionFvMesh
{
public:

    enum linCellType
    {
        CALCULATED  = 0, // normal operation
        MAPPED      = 1, // mapped from nonLinMesh
        HOLE        = 2  // uncomputed and unmapped
    };
    
private:
    
    // Private data

        //- Linear mesh region
        //  (specialized to handle uniform structured meshes)
        autoPtr<strFvMesh> linMeshPtr_;
        
        //- Non-linear mesh region
        autoPtr<fvMesh> nonLinMeshPtr_;
        
        //- Objects for handling mapping from linear mesh cells
        //  to non-linear interface patches (ifp)
        scalarField valueFraction_;
        
        labelList interfacePatchIDs_;
        
        labelListList l_to_ifp_addr_;
        
        scalarListList l_to_ifp_wght_;
        
        autoPtr<mapDistribute> l_to_ifp_mapPtr_;
        
        //- Objects for handling mapping from non-linear mesh cells
        //  to linear mesh cells
        labelListList nl_to_l_addr_;
        
        scalarListList nl_to_l_wght_;
        
        autoPtr<mapDistribute> nl_to_l_mapPtr_;
        
        labelList holeCells_, mappedCells_;
        
        tmp<volScalarField> tlinCellTypes_;
        
        // Condition class for performing walkFront
        template<class Op1, class Op2, class combineOp>
        class Condition
        {
		   public:
		    	Op1 op1_;
		    	label val1_;
		    	
		    	Op2 op2_;
		    	label val2_;
		    	
		    	combineOp cop_;
        	
        		Condition
        		(
        			const Op1& op1,
        			linCellType val1,
        			const Op2& op2,
        			linCellType val2,
        			combineOp cop
        		)
        		:
		    		op1_(op1),
		    		val1_(val1),
		    		op2_(op2),
		    		val2_(val2),
		    		cop_(cop)
		    	{}
		    	
		    	bool valid(label cVal1, label cVal2) const
		    	{
		    		return cop_(op1_(cVal1, val1_), op2_(cVal2, val2_));
		    	}
		    	
		    	bool orValid(label cVal1, label cVal2) const
		    	{
		    		return valid(cVal1, cVal2) || valid(cVal2, cVal1);
		    	}
        };


    // Private Member Functions

        //- No copy construct
        dualRegionFvMesh(const dualRegionFvMesh&) = delete;

        //- No copy assignment
        void operator=(const dualRegionFvMesh&) = delete;
        
        //- Walk front
        template<class Type, class TestCondition>
        bool walkFront
        (
        	const fvMesh& mesh,
        	List<Type>& vals,
        	const Type newVal,
        	const TestCondition& test
        );
        
        // Convert from compact addressing to global adressing given
    	// list of global cell indices
    	void renumber
    	(
    		labelListList& compactAddr,
    		const labelList&
    	);
    	
    	// Set mapDistribute pointer based on given inputs.
    	// Renumber indices in local subMap and local constructMap of mapPtr to
    	// include only the required local elements instead of all local
    	// elements. The constructMaps of other processors must be offset due
    	// to the truncation. The compact addressing in addr must be updated to
    	// reflect the changes.
    	void setMapPtr
    	(
    		autoPtr<mapDistribute>& mapPtr,
    		labelListList& globalAddr,
    		const globalIndex& gi
    	);
        
public:

    // Constructors

        //- Construct from time
        dualRegionFvMesh(const Time& time);
        
    // Member Functions
    
    	// Access
    	const strFvMesh& linStrMesh() const;
    	
    	const fvMesh& linMesh() const;
    	fvMesh& linMesh();
    	
    	const fvMesh& nonLinMesh() const;
    	fvMesh& nonLinMesh();
    	
    	
    	const scalarField& valueFraction() const;
    	
    	const labelList& interfacePatchIDs() const;
    	
    	const labelListList& l_to_ifp_addr() const;
        
        const scalarListList& l_to_ifp_wght() const;
        
        const mapDistribute& l_to_ifp_map() const;
        
    	
    	const labelListList& nl_to_l_addr() const;
        
        const scalarListList& nl_to_l_wght() const;
        
        const mapDistribute& nl_to_l_map() const;
        
        const labelList& holeCells() const;
        
        const labelList& mappedCells() const;
        
        const volScalarField& linCellTypes() const;
       
        // Set hole values (default hole value is 0)
        template<class Type>
        void setHoleValues
        (
        	GeometricField<Type, fvPatchField, volMesh>& fldPrime,
        	const Type& holeVal = Zero
        ) const;
        
        // Map non-linear field onto linear mesh and subtract given
        // mean field to get perturbation, fldPrime = map(fld) - fldMean
        template<class Type>
        void map_nl_to_l
        (
        	GeometricField<Type, fvPatchField, volMesh>&       fldPrime,
        	const GeometricField<Type, fvPatchField, volMesh>& fldMean,
        	const GeometricField<Type, fvPatchField, volMesh>& fld
        ) const;
        
        // Set value fraction for interface patches
        template<class Type>
        void initializeInterfaces
        (
        	GeometricField<Type, fvPatchField, volMesh>&  fld
        ) const;
        
        // Map linear fields onto non-linear mesh by performing Lagrange
        // interpolation
        template<class Type>
        void map_l_to_ifp
        (
        	GeometricField<Type, fvPatchField, volMesh>&  fld,
        	const GeometricField<Type, fvPatchField, volMesh>& fldMean,
        	const GeometricField<Type, fvPatchField, volMesh>& fldPrime
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "dualRegionFvMeshI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
