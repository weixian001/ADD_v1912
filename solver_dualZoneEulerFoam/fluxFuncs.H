// * * * * * * * * * * * * * * * Flux Functions * * * * * * * * * * * * * * * // 

void fluxCentral
(
	scalar& phi,
	vector& phiUp,
	scalar& phiEp,
	const vector& N,
	const scalar& zeta,
	const scalar& rho,
	const vector& U,
	const scalar& p
)
{
	// Face normal velocity component
	const scalar Un(U & N);
	
	// Total enthalpy
	const scalar hTot
	(
		zeta*p/rho + 0.5*magSqr(U)
	);
	
	// Fluxes
	phi   = rho*Un;
	
	phiUp = phi*U + p*N;
	
	phiEp = phi*hTot;
}

void fluxHLL
(
	scalar& phi,
	vector& phiUp,
	scalar& phiEp,
	vector& Ustar,
	const vector& N,
	const scalar& gamma,
	const scalar& rho_pos,
	const scalar& rho_neg,
	const vector& U_pos,
	const vector& U_neg,
	const scalar& p_pos,
	const scalar& p_neg
)
{
	const scalar gM1(gamma - 1.0);
	
	// Face-normal velocity component
	const scalar Un_pos(U_pos & N), Un_neg(U_neg & N);
	
	// Acoustic speeds
	const scalar cSqr_pos(gamma*p_pos/rho_pos), cSqr_neg(gamma*p_neg/rho_neg);
	
	const scalar c_pos(Foam::sqrt(cSqr_pos)), c_neg(Foam::sqrt(cSqr_neg));
	
	// Total enthalpy
	const scalar hTot_pos(cSqr_pos/gM1 + 0.5*magSqr(U_pos));
	
	const scalar hTot_neg(cSqr_neg/gM1 + 0.5*magSqr(U_neg));
	
	// Roe-averaged wave speeds
	const scalar Rf(Foam::sqrt(rho_pos/rho_neg));
	
	const scalar R_neg(1.0/(Rf + 1.0)), R_pos(Rf*R_neg);

	const scalar Un_Roe(R_pos*Un_pos + R_neg*Un_neg);
	
	const vector U_Roe(R_pos*U_pos + R_neg*U_neg);

	const scalar hTot_Roe(R_pos*hTot_pos + R_neg*hTot_neg);

	const scalar c_Roe(Foam::sqrt(gM1*(hTot_Roe - 0.5*magSqr(U_Roe))));
	
	const scalar s_pos(max(max(Un_neg + c_neg, Un_Roe + c_Roe), 0.0));

	const scalar s_neg(min(min(Un_pos - c_pos, Un_Roe - c_Roe), 0.0));
	
	// Wave speeds
	//const scalar s_pos(max(max(Un_pos + c_pos, Un_neg + c_neg), 0.0));
	
	//const scalar s_neg(min(min(Un_pos - c_pos, Un_neg - c_neg), 0.0));
	
	// Fluxes
	const scalar a_pos(s_pos/(s_pos - s_neg));
	
	const scalar a_neg(1.0 - a_pos);
	
	const scalar aStar( a_pos*s_neg );
	
	const scalar aphiv_pos( a_pos*Un_pos - aStar );
	
	const scalar aphiv_neg( a_neg*Un_neg + aStar );
	
	phi   = aphiv_pos*rho_pos + aphiv_neg*rho_neg;
	
	phiUp = aphiv_pos*rho_pos*U_pos + aphiv_neg*rho_neg*U_neg 
			+ (a_pos*p_pos + a_neg*p_neg)*N;
			
	phiEp = aphiv_pos*rho_pos*hTot_pos + aphiv_neg*rho_neg*hTot_neg 
			+ aStar*(p_pos - p_neg);
			
	// Intermediate velocity
	Ustar = a_pos*U_pos + a_neg*U_neg;
}

void fluxHLLC
(
	scalar& phi,
	vector& phiUp,
	scalar& phiEp,
	vector& Ustar,
	const vector& N,
	const scalar& gamma,
	const scalar& rho_pos,
	const scalar& rho_neg,
	const vector& U_pos,
	const vector& U_neg,
	const scalar& p_pos,
	const scalar& p_neg
)
{
	const scalar gM1(gamma - 1.0);
	
	// Face-normal velocity component
	const scalar Un_pos(U_pos & N), Un_neg(U_neg & N);
	
	// Acoustic speeds
	const scalar cSqr_pos(gamma*p_pos/rho_pos), cSqr_neg(gamma*p_neg/rho_neg);
	
	const scalar c_pos(Foam::sqrt(cSqr_pos)), c_neg(Foam::sqrt(cSqr_neg));
	
	// Roe-averaged wave speeds
	const scalar hTot_pos(cSqr_pos/gM1 + 0.5*magSqr(U_pos));
	
	const scalar hTot_neg(cSqr_neg/gM1 + 0.5*magSqr(U_neg));
	
	const scalar Rf(Foam::sqrt(rho_pos/rho_neg));
	
	const scalar R_neg(1.0/(Rf + 1.0)), R_pos(Rf*R_neg);

	const scalar Un_Roe(R_pos*Un_pos + R_neg*Un_neg);
	
	const vector U_Roe(R_pos*U_pos + R_neg*U_neg);

	const scalar hTot_Roe(R_pos*hTot_pos + R_neg*hTot_neg);

	const scalar c_Roe(Foam::sqrt(gM1*(hTot_Roe - 0.5*magSqr(U_Roe))));
	
	const scalar s_pos(max(Un_neg + c_neg, Un_Roe + c_Roe));

	const scalar s_neg(min(Un_pos - c_pos, Un_Roe - c_Roe));
	
	// Biased state variables '_'
	scalar rho_, p_, Un_, s_(0.0);
	
	vector U_;
	
	// Intermediate state corrections
	scalar drho(0.0), drhoE(0.0);
	
	vector drhoU(vector::zero);
	
	if(s_pos < 1e-12)
	{
		rho_ = rho_neg; p_ = p_neg;
		
		Un_ = Un_neg; U_ = U_neg;
		
		Ustar = U_neg;
	}
	else if(s_neg > -1e-12)
	{
		rho_ = rho_pos; p_ = p_pos;
		
		Un_ = Un_pos; U_ = U_pos;
		
		Ustar = U_pos;
	}
	else
	{
		const scalar a_pos(rho_pos*(s_neg - Un_pos));
		
		const scalar a_neg(rho_neg*(s_pos - Un_neg));
		
		// Intermediate normal velocity (speed of contact discontinuity)
		const scalar Un_star
		(
			(p_neg - p_pos + a_pos*Un_pos - a_neg*Un_neg)
			/(a_pos - a_neg)
		);
		
		// Intermediate pressure
		//const scalar p_star(p_pos + a_pos*(Un_star - Un_pos));
		
		scalar a_;
		
		if(Un_star < 0)
		{
			rho_ = rho_neg; p_ = p_neg;
		
			Un_ = Un_neg; U_ = U_neg;
			
			s_ = s_pos; a_ = a_neg;
		}
		else
		{
			rho_ = rho_pos; p_ = p_pos;
		
			Un_ = Un_pos; U_ = U_pos;
			
			s_ = s_neg; a_ = a_pos;
		}
		
		const scalar rho_star(a_/(s_ - Un_star));
		
		Ustar = U_ + (Un_star - Un_)*N; // intermediate velocity
		
		drho  = rho_star - rho_;
		
		drhoU = rho_star*Ustar - rho_*U_;
		
		drhoE = drho*(p_/(gM1*rho_) + 0.5*magSqr(U_)) 
		        + rho_star*(Un_star - Un_)*(Un_star + p_/a_);
	}
	
	phi   = rho_*Un_;
	
	phiUp = phi*U_ + p_*N;
			
	phiEp = phi*(0.5*magSqr(U_)) + Un_*(gamma*p_/gM1);
	
	phi   += s_*drho;

	phiUp += s_*drhoU;

	phiEp += s_*drhoE;
}

void fluxHLLCPlus
(
	scalar& phi,
	vector& phiUp,
	scalar& phiEp,
	vector& Ustar,
	const vector& N,
	const scalar& gamma,
	const scalar& rho_pos,
	const scalar& rho_neg,
	const vector& U_pos,
	const vector& U_neg,
	const scalar& p_pos,
	const scalar& p_neg
)
{
	const scalar gM1(gamma - 1.0);
	
	// Face-normal velocity component
	const scalar Un_pos(U_pos & N), Un_neg(U_neg & N);
	
	// Acoustic speeds
	const scalar cSqr_pos(gamma*p_pos/rho_pos), cSqr_neg(gamma*p_neg/rho_neg);
	
	const scalar c_pos(Foam::sqrt(cSqr_pos)), c_neg(Foam::sqrt(cSqr_neg));
	
	// Roe-averaged wave speeds
	const scalar hTot_pos(cSqr_pos/gM1 + 0.5*magSqr(U_pos));
	
	const scalar hTot_neg(cSqr_neg/gM1 + 0.5*magSqr(U_neg));
	
	const scalar Rf(Foam::sqrt(rho_pos/rho_neg));
	
	const scalar R_neg(1.0/(Rf + 1.0)), R_pos(Rf*R_neg);

	const scalar Un_Roe(R_pos*Un_pos + R_neg*Un_neg);
	
	const vector U_Roe(R_pos*U_pos + R_neg*U_neg);

	const scalar hTot_Roe(R_pos*hTot_pos + R_neg*hTot_neg);

	const scalar c_Roe(Foam::sqrt(gM1*(hTot_Roe - 0.5*magSqr(U_Roe))));
	
	const scalar s_pos(max(Un_neg + c_neg, Un_Roe + c_Roe));

	const scalar s_neg(min(Un_pos - c_pos, Un_Roe - c_Roe));
	
	// Biased state variables '_'
	scalar rho_, p_, Un_, s_(0.0), pf(0.0);
	
	vector U_;
	
	// Intermediate state corrections
	scalar drho(0.0), drhoE(0.0);
	
	vector drhoU(vector::zero);
	
	if(s_pos < 0.0)
	{
		rho_ = rho_neg; p_ = p_neg;
		
		Un_ = Un_neg; U_ = U_neg;
		
		Ustar = U_neg;
	}
	else if(s_neg > 0.0)
	{
		rho_ = rho_pos; p_ = p_pos;
		
		Un_ = Un_pos; U_ = U_pos;
		
		Ustar = U_pos;
	}
	else
	{
		const scalar a_pos(rho_pos*(s_neg - Un_pos));
		
		const scalar a_neg(rho_neg*(s_pos - Un_neg));
		
		// Intermediate normal velocity (speed of contact discontinuity)
		const scalar Un_star
		(
			(p_neg - p_pos + a_pos*Un_pos - a_neg*Un_neg)
			/(a_pos - a_neg)
		);
		
		// Anti-dissiption pressure
		const scalar pd(a_pos*a_neg*(Un_pos - Un_neg)/(a_pos - a_neg));
		
		// Local Mach number (squared)
		const scalar Msqr
		(
			min
			(
				1.0,
				max(magSqr(U_pos)/cSqr_pos, magSqr(U_neg)/cSqr_neg)
			)
		);
		
		// pf = (f(M) - 1) * pd
		pf = pd*(Foam::sqrt(Msqr*(4.0 + sqr(1.0 - Msqr)))/(1.0 + Msqr) - 1.0);
		
		scalar a_;
		
		if(Un_star < 0)
		{
			rho_ = rho_neg; p_ = p_neg;
		
			Un_ = Un_neg; U_ = U_neg;
			
			s_ = s_pos; a_ = a_neg;
		}
		else
		{
			rho_ = rho_pos; p_ = p_pos;
		
			Un_ = Un_pos; U_ = U_pos;
			
			s_ = s_neg; a_ = a_pos;
		}
		
		const scalar rho_star(a_/(s_ - Un_star));
		
		Ustar = U_ + (Un_star - Un_)*N; // intermediate velocity
		
		drho  = rho_star - rho_;
		
		drhoU = rho_star*Ustar - rho_*U_;
		
		drhoE = drho*(p_/(gM1*rho_) + 0.5*magSqr(U_)) 
		        + rho_star*(Un_star - Un_)*(Un_star + p_/a_);
	}
	
	phi   = rho_*Un_;
	
	phiUp = phi*U_ + (p_ + pf)*N ;
			
	phiEp = phi*(0.5*magSqr(U_)) + Un_*(gamma*p_/gM1) + pf*(Ustar & N);
	
	phi   += s_*drho;

	phiUp += s_*drhoU;

	phiEp += s_*drhoE;
}

void fluxRoe
(
	scalar& phi,
	vector& phiUp,
	scalar& phiEp,
	vector& Ustar,
	const vector& N,
	const scalar& gamma,
	const scalar& rho_pos,
	const scalar& rho_neg,
	const vector& U_pos,
	const vector& U_neg,
	const scalar& p_pos,
	const scalar& p_neg
)
{
	const scalar gM1(gamma - 1.0);
	
	// Face-normal velocity component
	const scalar Un_pos(U_pos & N), Un_neg(U_neg & N);
	
	// Tangential velovities
	const vector V_pos(U_pos - Un_pos*N), V_neg(U_neg - Un_neg*N);
	
	// Acoustic speeds
	const scalar cSqr_pos(gamma*p_pos/rho_pos), cSqr_neg(gamma*p_neg/rho_neg);
	
	// Roe-averaged quantities
	const scalar rho_Roe(Foam::sqrt(rho_pos*rho_neg));
	
	const scalar hTot_pos(cSqr_pos/gM1 + 0.5*magSqr(U_pos));
	
	const scalar hTot_neg(cSqr_neg/gM1 + 0.5*magSqr(U_neg));
	
	const scalar Rf(Foam::sqrt(rho_pos/rho_neg));
	
	const scalar R_neg(1.0/(Rf + 1.0)), R_pos(Rf*R_neg);

	const scalar Un_Roe(R_pos*Un_pos + R_neg*Un_neg);
	
	const vector U_Roe(R_pos*U_pos + R_neg*U_neg);
	
	const vector V_Roe(U_Roe - Un_Roe*N);
	
	const scalar hTot_Roe(R_pos*hTot_pos + R_neg*hTot_neg);
	
	const scalar k_Roe(0.5*magSqr(U_Roe));
	
	const scalar cSqr_Roe(gM1*(hTot_Roe - k_Roe));

	const scalar c_Roe(Foam::sqrt(cSqr_Roe));
	
	const scalar Unc_Roe(Un_Roe*c_Roe);
	
	// Wave speeds
	const scalar l1(Un_Roe - c_Roe), l5(Un_Roe + c_Roe);
	
	const scalar& l2 = Un_Roe;
	
	const scalar& l34 = Un_Roe;
	
	// Wave strengths (multiplied by absolute wave speeds)
	const scalar drho(rho_neg - rho_pos), dp(p_neg - p_pos);
	
	const scalar rhocdUn(rho_Roe*c_Roe*(Un_neg - Un_pos));
	
	const scalar invcSqr_Roe(1.0/cSqr_Roe);
	
	const scalar al1(0.5*invcSqr_Roe*(dp - rhocdUn)*mag(l1)),
				 al2((drho - dp*invcSqr_Roe)       *mag(l2)),
				 al5(0.5*invcSqr_Roe*(dp + rhocdUn)*mag(l5));
				 
	const vector al34(rho_Roe*(V_neg - V_pos)*mag(l34));
	
	// Right eigenvectors combined with wave speeds and strengths
	const scalar dphi(al1 + al2 + al5);
	
	const vector dphiUp
	(
		(al1*l1 + al2*l2 + al5*l5)*N + dphi*V_Roe + al34
	);
	
	const scalar dphiEp
	(
		  al1*(hTot_Roe - Unc_Roe)
		+ al2*k_Roe
		+ (al34 & V_Roe)
		+ al5*(hTot_Roe + Unc_Roe)
	);
	
	// Fluxes
	const scalar rhoUn_pos(rho_pos*Un_pos), rhoUn_neg(rho_neg*Un_neg);
	
	phi   = 0.5*((rhoUn_pos + rhoUn_neg) - dphi);
	
	phiUp = 0.5*((rhoUn_pos*U_pos + rhoUn_neg*U_neg + (p_pos + p_neg)*N) - dphiUp);
	        
	phiEp = 0.5*((rhoUn_pos*hTot_pos + rhoUn_neg*hTot_neg) - dphiEp);
	
	// Intermediate velocity (Roe average)
	Ustar = U_Roe;
}

// * * * * * * * * * * * * * * * * Shock Sensor * * * * * * * * * * * * * * * // 

// Modified Bhagatwala-Lele shock sensor
tmp<volScalarField> DucrosSensor
(
	const volScalarField& rho,
	const volScalarField& p,
	const volVectorField& U,
	const volScalarField& h,
	const tmp<volScalarField>& gamma
)
{
	const volScalarField divU(fvc::div(U));
	
	const volScalarField divUsqr(sqr(divU));
	
	const volScalarField c(Foam::sqrt(gamma*p/rho));
	
	const volScalarField curlUsqr(magSqr(fvc::curl(U)));
	
	const dimensionedScalar eps("eps", dimless/sqr(dimTime), 1e-40);
	
	tmp<volScalarField> tmpSensor
    (
		new volScalarField
		(
			0.5*(1.0 - Foam::tanh(2.5 + 100.0*h*divU/c))*
			divUsqr/(divUsqr + curlUsqr + eps)
		)
    );
    
    return tmpSensor;
}
