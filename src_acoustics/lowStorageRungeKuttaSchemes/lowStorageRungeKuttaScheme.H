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
    Foam::lowStorageRungeKuttaScheme

Description
    Low Storage Runge-Kutta Scheme in 2N form.

SourceFiles
    lowStorageRungeKuttaSchemeI.H
    lowStorageRungeKuttaScheme.C

\*---------------------------------------------------------------------------*/

#ifndef lowStorageRungeKuttaScheme_H
#define lowStorageRungeKuttaScheme_H

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class lowStorageRungeKuttaScheme Declaration
\*---------------------------------------------------------------------------*/

class lowStorageRungeKuttaScheme
{
    // Private Data
    
    	//- Non-const reference to Time object
    	Time& time_;
    	
    	//- Current time value
    	scalar currTimeValue_;
    	
    	//- Current time index
    	label currTimeIndex_;
    	
    	//- Stage counter
    	label s_;

    // Private Member Functions

        //- No copy construct
        lowStorageRungeKuttaScheme(const lowStorageRungeKuttaScheme&) = delete;

        //- No copy assignment
        void operator=(const lowStorageRungeKuttaScheme&) = delete;

protected:

	// Protected Data
	
		//- No. of stages
		label nStages_;
		
		//- Coefficients
		scalarField alpha_, beta_, c_;
		
	// Protected Member Functions
	
		// Check no. of stages matches with size of coeffs
		void checkCoeffsSize();


public:

    //- Runtime type information
    TypeName("lowStorageRungeKuttaScheme");
    
    // Declare run-time constructor selection table(s)
		
		declareRunTimeSelectionTable
		(
		    autoPtr,
		    lowStorageRungeKuttaScheme,
		    word,
            (Time& time),
            (time)
		);


    // Constructors

        //- Construct from components
        lowStorageRungeKuttaScheme(Time& time);


    // Selectors

        //- Select null constructed
        static autoPtr<lowStorageRungeKuttaScheme> New
        (
        	const fvMesh& mesh
        );


    //- Destructor
    virtual ~lowStorageRungeKuttaScheme() {};

    // Member Functions

        // Access
        const Time&         time() const;
        
        const scalarField& alpha() const;
        
        const scalarField&  beta() const;
        
        const scalarField&     c() const;
        
        inline label nStages() const
        {
        	return nStages_;
        }
        
        inline label s() const
        {
        	return s_;
        }
        
        // Store time properties at start
        void start();
        
        // Reset time properties at finish
        void finish();
        
        // Check if loop should continue to run
        inline bool run()
        {
        	//return (s_ < nStages_);
        	if (s_ < nStages_)
        	{
	        	if (s_ == 0)
	        	{
	        		start();
	        	}
	        	
	        	return true;
        	}
        	else
        	{
        		finish();
        		
        		return false;
        	}
        }
        
        // Advance in time
        virtual void advance(volScalarField& u) const;
        virtual void advance(volVectorField& u) const;
        
	// Member operators
	
		lowStorageRungeKuttaScheme& operator++();
		
		lowStorageRungeKuttaScheme& operator++(int);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
