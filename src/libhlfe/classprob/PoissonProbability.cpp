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
// PoissonProbability.cpp: implementation of the PoissonProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "PoissonProbability.h"
#include <cmath>
#include <cstdlib>
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*  The Poisson probability.
	The equation for this probability is:
	P(Y) = lambda^Y * e^lambda / Y!
*/
PoissonProbability::PoissonProbability(int Y, double Lambda, Probability::RandomVariableType rvt)
		:m_changes(Y),m_poisson_average(Lambda),Probability(Y,rvt)
{
	assert(Y >=0);
	assert(Lambda >=0);
}

/*	Computing the result.
	Similar in form to the Binomial and the Negative Binomial functions.
	Components are broken up into factors of lambda, e, and digits of
	the factorial in the denominator.  Lambda is broken down into its
	integral and fractional form with the first calculation being multiplying
	the result by a power of e to this fractional form.
	Like the Binomial, the running result is made to hover around the value
	of one. If the result is over one then we divide by a denominator digit.
	If the result is less than one then we multiply by lambda or e.
*/
double PoissonProbability::ComputeResult() throw (ProbabilityException)
{
	int powers, denom,integral;
	double fractional, temp;
	powers = denom = m_changes;
	fractional	 = modf(m_poisson_average, &temp);
	integral = static_cast<int>(temp);
	double result = 1.0;
	result *= pow(exp(1),-1.0 * fractional);
	while (powers >0 || denom > 0 || integral > 0)
	{
		if (result > 1.0 || powers == 0)
		{
			if (denom > 0)
			{
				result /= denom;
				--denom;
			}
			else if (integral > 0 )
			{
				result /= exp(1);
				--integral;
			}
			else
				throw ProbabilityException("Poisson error- premature end of denominator counter");

		}
		else if (result <=1.0 || integral ==0)
		{
			if (powers > 0)
			{
				result *= m_poisson_average;
				--powers;
			}
			else
				throw ProbabilityException("Poisson error- premature end of power or integral counter");
		}
		else
			throw ProbabilityException("Poisson error- result is NaN");
	}
	return result;
}

void PoissonProbability::SetRV(int Y)
{
	assert(Y>=0);
	m_changes = Y;
}

//	The mean and the variance of a Poisson probability are the same.
double PoissonProbability::GetExpectedValue() const
{
	return m_poisson_average;
}

double PoissonProbability::GetVariance() const
{
	return m_poisson_average;
}
