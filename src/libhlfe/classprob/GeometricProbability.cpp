/*
	Author: Joe Nellis
	email: mrknowitall@mtcrossroads.org
	date: Jan 2003

	**Notice and Disclaimer**
		This code is hereby donated to the public domain.  You may modify
	or take from the code what you will, all I ask is for some recognition or this comment block
	be included in said used/modified code. This codes comes AS IS in all its glory, with or
	without bugs, performance hitches, or other behavioral anomolies.
		Concerned users should consider not using this code without further
	scrutiny for the following uses: Delivering babies, guiding missles, controlling pacemakers,
	controlling chicken plucking robots, mixing cocktails, personal banking or fur trapping.
*/
// GeometricProbability.cpp: implementation of the GeometricProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "GeometricProbability.h"
#include <cstdlib>
#include <cmath>
#include <assert.h>
#include <cfloat>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*	The discrete geometric probability
	The equation for this probability is:
	P(Y) = (1.0-p)^(y-1) * p
*/
GeometricProbability::GeometricProbability(int Y, double p, Probability::RandomVariableType rvt)
		:m_trial_of_success(Y), m_chance_of_success(p), Probability(Y,rvt)
{
	assert(Y>=0);
	assert(p >= 0.0  && p<=1.0);
}

double GeometricProbability::ComputeResult() throw (ProbabilityException)
{
	if (m_trial_of_success == 0)
		return 0.0;
	return pow((1.0 - m_chance_of_success),m_trial_of_success -1.0) * m_chance_of_success ;
}

// Expected value or population mean is defined by
// E(Y) = 1/p
double GeometricProbability::GetExpectedValue() const
{
	if (m_chance_of_success == 0.0)
		return DBL_MAX;
	return 1.0/m_chance_of_success;
}

// Variance is defined by
// V(Y)= (1-p)/p^2
double GeometricProbability::GetVariance() const
{
	if (m_chance_of_success == 0.0)
		return DBL_MAX;
	return (1.0 - m_chance_of_success)/(m_chance_of_success * m_chance_of_success);
}

void GeometricProbability::SetRV(int Y)
{
	assert(Y>=0);
	m_trial_of_success = Y;
}

