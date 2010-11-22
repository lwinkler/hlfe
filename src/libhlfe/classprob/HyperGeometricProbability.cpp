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
// HyperGeometricProbability.cpp: implementation of the HyperGeometricProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "HyperGeometricProbability.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The HyperGeometric probability is defined by:
// The equation: (rCy)*((N-r)C(n-y))/(NCn)
// where (x C y ) represents a combination, "x choose y"
HyperGeometricProbability::HyperGeometricProbability(int N, int n, int y, int r,
		Probability::RandomVariableType rvt)
		:m_population(N), m_sample(n), m_red(y), m_black(n-y),m_selectedset(r),Probability(y,rvt)
{
	assert( y <= r);
	assert(n-y <= N-r);
	assert(y>=0);
	assert(N>0);
	assert(r>=0);
	assert(n>=0);
}

void HyperGeometricProbability::SetRV(int Y)
{
	assert(Y>=0);
	assert(Y <= m_selectedset);
	assert(m_sample - Y <= m_population - m_selectedset);
	m_red = Y;
	m_black = (m_sample - m_red);
}

// The expected value or population mean is defined by:
// E(Y) = n*r/N
double HyperGeometricProbability::GetExpectedValue() const
{
	return m_sample * m_selectedset / m_population;
}

// The variance is defined by:
// V(Y) = n * (r/N) * (N-r)/N * (N-n)/(N-1)
double HyperGeometricProbability::GetVariance() const
{
	if ( m_population == 1)
		return 0.0;
	return m_sample * m_selectedset / m_population * (m_population - m_selectedset) / m_population * \
		   (m_population - m_sample) / (m_population -1);
}


// The equation: (rCy)*((N-r)C(n-y))/(NCn)
/* Computing the result
	The computation is composed of pure combinations which are
	handled in the same fashion as the Binomial and NegativeBinomial
	probabilities.  The equation is broken up into numerators and
	denominators and then appropriately multiplied by or divided by
	depending on the running result being above or below 1.0.
*/
double HyperGeometricProbability::ComputeResult() throw (ProbabilityException)
{
	if (m_population ==0)
		return 0.0;
	double result = 1.0;
	int numer1,numer2,numer3,numer1cmp,numer2cmp,numer3cmp;
	int denom1,denom2,denom3,range1,range2,range3;
	numer1 = numer1cmp = m_selectedset;
	numer2 = numer2cmp = m_population - m_selectedset;
	numer3 = numer3cmp = m_population;

	//optimization on (rCy)
	if (m_selectedset-m_red > m_red)
		range1 = denom1 = m_red;
	else
		range1 = denom1 = m_selectedset - m_red;

	//optimization on ((N-r)C(n-y))
	if ((m_population - m_selectedset) - (m_sample - m_red) > m_sample - m_red)
		range2 = denom2 = m_sample - m_red;
	else
		range2 = denom2 = (m_population - m_selectedset) - (m_sample - m_red);

	//optimization on (NCn)
	if (m_population - m_sample > m_sample)
		range3 = denom3 = m_sample;
	else
		range3 = denom3 = m_population - m_sample;

	while (numer1 > numer1cmp-range1 || numer2 > numer2cmp-range2 || numer3 > numer3cmp-range3
			|| denom1>0 || denom2>0 || denom3>0)
	{
		if (result > 1.0 || denom3 == 0 )
		{
			if (denom1 > 0 )
				result = result / denom1--;
			else if (denom2 > 0 )
				result = result / denom2--;
			else if (numer3 > numer3cmp - range3 )
				result = result / numer3--;
			else
				throw ProbabilityException("HyperGeometric Probability error- premature end of divisors");
		}
		else if (result <=1.0 || numer3 - range3 == 0 )
		{
			if (numer1 > numer1cmp - range1 )
				result = result * numer1--;
			else if (numer2 > numer2cmp - range2 )
				result = result * numer2--;
			else if (denom3 > 0 )
				result = result * denom3--;
			else
				throw ProbabilityException("HyperGeometric Probability error- premature end of multipliers");
		}
		else
			throw ProbabilityException("HyperGeometric Probability error- result is NaN");
	}
	return result;
}
