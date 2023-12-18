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

#include "strFvMesh.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

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
}
/*
const Foam::vector Foam::strFvMesh::delta(const label gcelli) const
{
        label lCelli = gCells_.toLocal(gcelli);
        const labelList& cellFacei = this->cells()[lCelli];
        vector delta_(vector::zero);
	label nx = 0, ny = 0, nz = 0;
        forAll(cellFacei, facei)
        {
	   label faceIndex = cellFacei[facei];
	   if (this->isInternalFace(faceIndex))
	   {
	     if (nx == 1 && ny == 1 && nz == 1) break;
	     // find delta at x-axis
	     if (nx == 0 && internalFaceDir_[faceIndex] == 0)
	     {
		delta_[0] = cmptMax(cmptMag(this->Cf()[faceIndex] - gC_[gcelli]))*2;
		nx++;
	     }
	     // find delta at y-axis
	     if (ny == 0 && internalFaceDir_[faceIndex] == 1)
	     {
		delta_[1] = cmptMax(cmptMag(this->Cf()[faceIndex] - gC_[gcelli]))*2;
		ny++;
	     }
	     // find delta at z-axis
	     if (nz == 0 && internalFaceDir_[faceIndex] == 2)
	     {
		delta_[2] = cmptMax(cmptMag(this->Cf()[faceIndex] - gC_[gcelli]))*2;
		nz++;
	     }
	   }
	}
	return delta_;
}
*/
bool Foam::strFvMesh::pointInsideCell
(
	const point& pt,
	const label& gcelli
)const
{
	// Check if pt lies beyond 1/2 cell width from centroid in any direction.
	// Works because all cells are uniform hexahedral with width delta_.
	return cmptMax(cmptMag(pt - gC_[gcelli])) < (0.5 + SMALL)*delta_;
/*	
	bool isInsideCell(true);
	vector dist = cmptMag(pt - gC_[gcelli]);
	vector max = delta(gcelli);

	// Check if pt lies beyond 1/2 cell width from centroid in any direction.
	for(register label i=0; i < 3; i++)
	{
	    if (!isInsideCell) break;
	    if (max[i] != 0 && dist[i] > (0.5 + SMALL)*max[i])
	    {
		isInsideCell = false;
	    }
	}

	return isInsideCell;
*/
}

bool Foam::strFvMesh::findGlobalCell
(
	const point& pt,
	label& gcelli,
	point& xc
) const
{
	scalar dist(GREAT);
	
	gcelli = -1;
	
	// Find nearest cell centroid to pt and find its global index
	forAll(gC_, celli)
	{
		scalar newDist(magSqr(gC_[celli] - pt));
		
		if (newDist < dist)
		{
			dist = newDist;
			
			gcelli = celli;
		}
	}
	
	// Store centroid
	xc = gC_[gcelli];
	
	// Nearest cell need not necessarily contain the point. So, it must
	// be checked explicitly.
	return pointInsideCell(pt, gcelli);
}

bool Foam::strFvMesh::checkCorners
(
	scalarListList& X
)const
{
	scalar distSqr(0.0);
	
	vector disp(Zero);
	
	// Check if corner points lie inside the mesh
	for(register label i = 0; i < X[0].size(); i += max(1, X[0].size() - 1))
	{
	for(register label j = 0; j < X[1].size(); j += max(1, X[1].size() - 1))
	{
	for(register label k = 0; k < X[2].size(); k += max(1, X[2].size() - 1))
	{
		const point cornerPt(X[0][i], X[1][j], X[2][k]);
		
		label gcelli; point xc;
		
		// Corner point lies outside nearest cell
		if (!findGlobalCell(cornerPt, gcelli, xc))
		{
			const vector dx(cornerPt - xc);
			
			const scalar magSqrDx(magSqr(dx));
			
			if (magSqrDx > distSqr)
			{
				distSqr = magSqrDx;
				
				disp = dx;
			}
		}
	}}}
	
	// All corners coincide with cell centroids of mesh
	if (distSqr < SMALL)
	{
		return true;
	}
	
	// Shift coordinates by disp vector components to move stencil
	// into the mesh
	forAll(X, d)
	{
		scalarList& Xd = X[d];
		
		forAll(Xd, i)
		{
			Xd[i] -= disp[d];
		}
	}
	
	return false;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::strFvMesh::strFvMesh
(
	const IOobject& io
)
:
    fvMesh(io)
{
	// Compute (uniform) cell size from measuring distance between centroid of 
	// owner and neighbour cells of first interal face.
	delta_ = mag(this->C()[this->owner()[0]] - this->C()[this->neighbour()[0]]);
	
	// Compute solved for dimensions
	dims_= 0.5*(this->solutionD() + cmptMag(this->solutionD()));
			
	// Reset global index based on number of cells in local mesh
	gCells_.reset(this->nCells());
	
	// Maintain a copy of cell centroid points from all procs
	gC_.setSize(gCells_.size(), Zero);
	
	forAll(this->C(), celli)
	{
		gC_[gCells_.toGlobal(celli)] = this->C()[celli];
	}
	
	Pstream::listCombineGather(gC_, plusEqOp<vector>());
	
	Pstream::listCombineScatter(gC_);
	
	// Store tmp reference to unit face normal vector
	tnf_ = this->Sf()/this->magSf();
	
	tnf_.ref().rename("Nf");
	
	// Create spatial & filter schemes from mesh
	spSch_.reset(spatialScheme::New(*this));
	
	flSch_.reset(filterScheme::New(*this));
	
	// Determine face normal directions for internal faces by taking dot
	// product of face unit normal vector nf with vector (0,1,2). Magnitude
	// of the result (0, 1 or 2) corresponds to the face normal direction
	// (x, y or z).
	internalFaceDir_.setSize(this->nInternalFaces());
	
	forAll(internalFaceDir_, facei)
	{
		const vector& nfi = this->nf().primitiveField()[facei];
		
		internalFaceDir_[facei] = label(mag(nfi & vector(0,1,2)));
	}
	
	// Determine face normal directions for boundary faces in the same manner.
	const fvBoundaryMesh& bm = this->boundary();
	
	boundaryFaceDir_.setSize(bm.size());
	
	forAll(boundaryFaceDir_, patchi)
	{
		const vectorField& pnf = this->nf().boundaryField()[patchi];
		
		boundaryFaceDir_[patchi].setSize(pnf.size());
		
		forAll(pnf, facei)
		{
			boundaryFaceDir_[patchi][facei]
				= label(mag(pnf[facei] & vector(0,1,2)));
		}
	}
}

// * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * * * //

const Foam::scalar& Foam::strFvMesh::delta() const
{
	return this->delta_;
}

const Foam::vector& Foam::strFvMesh::dims() const
{
	return this->dims_;
}

const Foam::globalIndex& Foam::strFvMesh::gCells() const
{
	return this->gCells_;
}

const Foam::pointField& Foam::strFvMesh::gC() const
{
	return this->gC_;
}

const Foam::surfaceVectorField& Foam::strFvMesh::nf() const
{
	return this->tnf_();
}

const Foam::labelList& Foam::strFvMesh::internalFaceDir() const
{
	return this->internalFaceDir_;
}

const Foam::labelList& Foam::strFvMesh::patchFaceDir(const label patchi) const
{
	return this->boundaryFaceDir_[patchi];
}

const Foam::spatialScheme& Foam::strFvMesh::scheme() const
{
	return this->spSch_();
}

const Foam::filterScheme& Foam::strFvMesh::filter() const
{
	return this->flSch_();
}

void Foam::strFvMesh::calcLinearInterpAddr
(
	const point& xi, // face centroid
	const point& xp, // non-linear mesh cell centroid
	labelList& addr,
	scalarList& wght
) const
{
	// Find global index of cell containing xi and get its centroid xc
	label gcelli; point xc;
	
	const vector deltaNL(xi - xp);
	
	if (!findGlobalCell(xi + 1e-6*deltaNL, gcelli, xc))
	{
		FatalErrorInFunction
			<< "Point to interpolate " << xi << " lies outside mesh."
			<< exit(FatalError);
	}
	
	// Vectors from interpolation point (xi) to linear cell centroid (xc)
	// and non-linear cell centroid (xp)
	vector deltaL(xc - xi);
	
	scalar magDeltaL(mag(deltaL));

	// The current cell size in all dimensions
	//vector deltaXYZ = delta(gcelli);
	
	// If xc with xi OR xc lies on the same side of xi as xp, 
	// check surrounding cells.
	bool newCellFound(false);

	if (magDeltaL < SMALL || neg(deltaL & deltaNL))
	{
		// Shift vector
		labelList s(3);
		s[0] = -1;
		s[1] =  0;
		s[2] =  1;
		
		magDeltaL = GREAT;
		
		const vector xcOld(xc);
		
		// Permutate over surrounding cells
		for(register label i = 0; i < 3; ++i)
		{
		for(register label j = 0; j < 3; ++j)
		{
		for(register label k = 0; k < 3; ++k)
		{
			// Skip current cell
			if (i == 0 && j == 0 && k == 0) continue;
			
			const point searchPt(xcOld + vector(s[i],s[j],s[k])*delta_);
			//const point searchPt(xcOld + vector(s[i]*deltaXYZ.x(),s[j]*deltaXYZ.y(),s[k]*deltaXYZ.z()));
			
			label gcelliNew; point xcNew;
			
			// Skip if searchPt lies outside nearest cell gcelliNew
			if (!findGlobalCell(searchPt, gcelliNew, xcNew)) continue;
			
			vector deltaLNew(xcNew - xi);
			
			// xcNew lies on the opposite side of xi as xp
			if (pos(deltaLNew & deltaNL))
			{
				scalar magDeltaLNew(mag(deltaLNew));
				
				// xcNew is closer to xi than xc BUT does not coincide
				// with xi
				if (magDeltaLNew < magDeltaL && magDeltaLNew > SMALL)
				{
					gcelli = gcelliNew;
					
					xc = xcNew;
					
					magDeltaL = magDeltaLNew;
					
					newCellFound = true;
				}
			}
		}}}
		
		if (!newCellFound)
		{
			FatalErrorInFunction
				<< "No suitable cell found for interpolation point "
				<< xi << "."
				<< exit(FatalError);
		}
	}
	
	vector deltaC(xc - xp);
	
	addr.setSize(1); addr[0] = gcelli;
	
	wght.setSize(1); wght[0] = (deltaNL & deltaC)/magSqr(deltaC);
}

void Foam::strFvMesh::calcLagrangeInterAddr
(
	const point& xi,
	const label order,
	labelList& addr,
	scalarList& wght
) const
{
	// Find global index of cell containing xi and get its centroid xc
	label gcelli; point xc;
	
	if (!findGlobalCell(xi, gcelli, xc))
	{
		FatalErrorInFunction
			<< "Point to interpolate " << xi << " lies outside mesh."
			<< exit(FatalError);
	}

	// The current cell size in all dimensions
        //vector deltaXYZ = delta(gcelli);
	
	// Find nearest corner point to xi
	const vector dx(cmptMultiply(xi - xc, dims_));
	
	// Set reference point as cell centroid (for odd orders)
	vector refPt(xc);
	
	// Set reference point to nearest corner point (for even orders)
	if (order % 2 == 0)
	{
		forAll(refPt, d)
		{
			//refPt[d] += 0.5*sign(dx[d])*deltaXYZ[d];
			refPt[d] += 0.5*sign(dx[d])*delta_;
		}
	}
	 
	// Find list of x, y and z coordinates assuming that a symmetric stencil 
	// can be found about refPt.
	scalarListList X(3);
	
	forAll(X, d)
	{
		const scalar start(0.5*(1 - order));
		
		if (mag(dx[d]) > SMALL)
		{
			X[d].setSize(order);
			
			for(register label j = 0; j < order; ++j)
			{
				//X[d][j] = refPt[d] + (start + j)*deltaXYZ[d];
				X[d][j] = refPt[d] + (start + j)*delta_;
			}
		}
		else
		{
			// xi's ith coordinate coincides with that of xc.
			// No need to interpolate along ith direction.
			X[d].setSize(1);
			
			X[d][0] = xc[d];
		}
	}
	
	// Check if corner points of stencil lie inside the mesh. Repeat check to 
	// make sure.
	bool cornersOk(false);
	
	for(register label repeat = 0; repeat < 2; ++repeat)
	{
		cornersOk = checkCorners(X);
		
		if (cornersOk) break;
	}
	
	// If corners are not ok after second pass, mesh is probably irregular in
	// shape.
	if (!cornersOk)
	{
		FatalErrorInFunction
			<< "Stencil for point " << xi << " lies outside mesh."
			<< exit(FatalError);
	}
	
	// Compute one-dimensional weights for Largrange interpolating polynomial
	scalarListList W(3);
	
	forAll(W, d)
	{
		const scalarList& Xd = X[d];
		
		scalarList& Wd = W[d];
		
		Wd.setSize(Xd.size(), 1.0);
		
		forAll(Wd, i)
		{
			forAll(Wd, m)
			{
				if (m != i)
				{
					Wd[i] *= (xi[d] - Xd[m])/(Xd[i] - Xd[m]);
				}
			}
		}
	}
	
	// Total number of cells in stencil
	const label nPts = X[0].size() * X[1].size() * X[2].size();
	
	// Find centroids of cells in stencil using combinations of X and compute
	// their weights from W. 
	addr.setSize(nPts); wght.setSize(nPts);
	
	label pti(0); point pt;
	
	for(register label i = 0; i < X[0].size(); ++i)
	{
	for(register label j = 0; j < X[1].size(); ++j)
	{
	for(register label k = 0; k < X[2].size(); ++k)
	{
		findGlobalCell
		(
			point(X[0][i], X[1][j], X[2][k]),
			addr[pti],
			pt
		);
		
		wght[pti] = W[0][i] * W[1][j] * W[2][k];
		
		++pti;
	}}}
}

// ************************************************************************* //
