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

#include "lowStorageRungeKuttaScheme.H"

namespace Foam
{
	defineTypeNameAndDebug(lowStorageRungeKuttaScheme, 0);
	defineRunTimeSelectionTable(lowStorageRungeKuttaScheme, word);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

void Foam::lowStorageRungeKuttaScheme::checkCoeffsSize()
{
	if (alpha_.size() != nStages_)
	{
		FatalErrorInFunction
        	<< "alpha coefficients for " << nStages_ << "-stage scheme "
        	<< "has size " << alpha_.size() << nl << nl
            << exit(FatalError);
	}
	
	if (beta_.size() != nStages_)
	{
		FatalErrorInFunction
        	<< "beta coefficients for " << nStages_ << "-stage scheme "
        	<< "has size " << beta_.size() << nl << nl
            << exit(FatalError);
	}
	
	// Size of c must be 1 more than the no. of stages!
	if (c_.size() != nStages_ + 1)
	{
		FatalErrorInFunction
        	<< "c coefficients for " << nStages_ << "-stage scheme "
        	<< "has size " << c_.size() << nl << nl
            << exit(FatalError);
	}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::lowStorageRungeKuttaScheme::lowStorageRungeKuttaScheme
(
	Time& time
)
:
	time_(time),
	currTimeValue_(),
	currTimeIndex_(),
	s_(0),
	nStages_(0),
	alpha_(),
	beta_(),
	c_()
{
	start();
}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::lowStorageRungeKuttaScheme>
Foam::lowStorageRungeKuttaScheme::New
(
    const fvMesh& mesh
)
{
    if (!mesh.schemesDict().found("temporalScheme"))
    {
        FatalErrorInFunction
        	<< "temporalScheme not specified in fvSchemes" << nl << nl
            << "Valid temporalSchemes are :" << endl
            << wordConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }
    
    const word schemeName(mesh.schemesDict()["temporalScheme"]);
    	
    auto cstrIter = wordConstructorTablePtr_->cfind(schemeName);

    if (!cstrIter.found())
    {
        FatalErrorInFunction
        	<< "Unknown temporalScheme " << schemeName << nl << nl
            << "Valid temporalSchemes are :" << endl
            << wordConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }
    
    Info<< "\nTemporal scheme: " << schemeName << nl << endl;
    
    Time& time = const_cast<Time &>(mesh.time());
    
    return cstrIter()(time);
}


// * * * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * //

const Foam::Time& Foam::lowStorageRungeKuttaScheme::time() const
{
	return time_;
}

const Foam::scalarField& Foam::lowStorageRungeKuttaScheme::alpha() const
{
	return alpha_;
}

const Foam::scalarField& Foam::lowStorageRungeKuttaScheme::beta() const
{
	return beta_;
}

const Foam::scalarField& Foam::lowStorageRungeKuttaScheme::c() const
{
	return c_;
}

void Foam::lowStorageRungeKuttaScheme::start()
{
	// Store time & time index
	currTimeValue_ = time_.timeOutputValue();
    	
    currTimeIndex_ = time_.timeIndex();
}

void Foam::lowStorageRungeKuttaScheme::finish()
{
	// Reset stage counter to zero
	s_ = 0;
	
	// Set time & time index back to original values
	time_.setTime
	(
		currTimeValue_,
		currTimeIndex_
	);
}

Foam::lowStorageRungeKuttaScheme&
Foam::lowStorageRungeKuttaScheme::operator++()
{
	s_++;
	
	time_.setTime
	(
		currTimeValue_ + c_[s_]*time_.deltaTValue(),
		currTimeIndex_
	);
	
	return *this;
}

Foam::lowStorageRungeKuttaScheme&
Foam::lowStorageRungeKuttaScheme::operator++(int)
{
	return ++(*this);
}

// Replaces templated function
#define makeAdvanceType(vfType)                                               \
void Foam::lowStorageRungeKuttaScheme::advance                                \
(                                                                             \
	vfType& u                                                                 \
) const                                                                       \
{                                                                             \
	const fvMesh& mesh = u.mesh();                                            \
	                                                                          \
	vfType& uTmp = mesh.lookupObjectRef<vfType>(u.name() + "Tmp");            \
	                                                                          \
	const vfType& F = mesh.lookupObject<vfType>(u.name() + "Flux");           \
	                                                                          \
    uTmp.primitiveFieldRef() *= alpha_[s_];                                   \
                                                                              \
    uTmp.primitiveFieldRef() += time_.deltaTValue()*F.primitiveField();       \
                                                                              \
    u.primitiveFieldRef() += beta_[s_]*uTmp.primitiveField();                 \
}

makeAdvanceType(volScalarField);
makeAdvanceType(volVectorField);

#undef makeAdvanceType

// ************************************************************************* //
