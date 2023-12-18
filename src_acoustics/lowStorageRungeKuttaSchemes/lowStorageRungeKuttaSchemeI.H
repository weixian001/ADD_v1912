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

makeAdvanceType(scalar)
makeAdvanceType(vector)

/*template<class Type>
void Foam::lowStorageRungeKuttaScheme::advance
(
	Type& u,
	Type& uTmp,
	const Type& F
) const
{
    uTmp *= this->alpha_[this->s_];
    
    uTmp += this->time_.deltaTValue()*F;
    
    u += this->beta_[this->s_]*uTmp;
}

template<class Type>
void Foam::lowStorageRungeKuttaScheme::advance
(
	GeometricField<Type, fvPatchField, volMesh>& u
) const
{
	const fvMesh& mesh = u.mesh();
	
	typedef GeometricField<Type, fvPatchField, volMesh> vfType;
	
	vfType& uTmp = mesh.lookupObjectRef<vfType>(u.name() + "Tmp");
		
	vfType& F = mesh.lookupObjectRef<vfType>(u.name() + "Flux");
	
	advance
	(
		u.primitiveFieldRef(),
		uTmp.primitiveFieldRef(),
		F.primitiveField()
	);
}*/



// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //
