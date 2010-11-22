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
// NegativeBinomialProbability.cpp: implementation of the NegativeBinomialProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "NegativeBinomialProbability.h"
#include <assert.h>
#include <climits>
#include <cfloat>
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*	The Negative Binomial Probability
	The equation for this probability is:
	P(Y) = (Y-1)!/(((Y-1)-(K-1))!(K-1)!)*p^K*(1-p)^(Y-K)
*/
NegativeBinomialProbability::NegativeBinomialProbability(int Y, int K, double p, Probability::RandomVariableType rvt)
		:m_trials(Y),
		m_kth_success(K),
		m_chance_of_success(p),
		Probability(Y,rvt)
{
	assert(0<=Y && K>=0);
	assert(p >=0.0 && p<=1.0);
}

/* Computing the result.
	This algorithm is exactly the same as the Binomial Probability
	except that Y-1 is replacing N and K-1 is replacing Y in
	the original algorithm for computing the factorial part.
	This directly relates to the fact that we assume K-1 successes
	have happened out of Y-1 trials and that the last trial is
	the Kth success. The factorial part figures out how many possible
	ways we can have K-1 successes in Y-1 trials then the actual
	probabilities of those successes including the last one are
	multiplied along with the Y-K number of failed trial probabilities.
*/
double NegativeBinomialProbability::ComputeResult()
{
	// the base class function GetResult will possibly
	// set our number of trials below K so just return 0.0;
	if (m_kth_success > m_trials || m_trials == 0)
		return 0.0;

	// initialize some variables
	double result = 1.0;
	int K = m_kth_success;
	int Y = m_trials;
	double P = m_chance_of_success;
	double Q = 1.0 - P;
	int range = 0, np =0, nq = 0, nnumer = 0, ndenom = 0;
	// validate
	assert( K<=Y && K >=0 );
	assert( P <= 1.0 && P >=0.0);
	// check optimizations
	if (K == 1) {
		return result = pow(Q,Y);
	}
	if (K == Y) {
		return result = pow(P,K);
	}
	// reorder the factorials to account for cancellations
	// in numerator and denominator.
	if (K < Y-K) {
		range = K-1;		// Y-K cancels out
	}
	else {
		range = (Y-1)-(K-1);	// K cancels out
	}
	np = K;
	nq = Y-K;
	ndenom = range;
	nnumer = Y-1;

	while (np > 0 || nq > 0 || ndenom > 0 || nnumer >(Y-1-range)) {
		// If the result is greater than one we want to divide by
		// a denominator digit or multiply by percentage p or q.
		// If we are out of numerator digits then finish multiplying
		// with our powers of p or q or dividing by a denom digit.
		if (result >= 1.0 || nnumer ==(Y-1-range)) {
			if (ndenom > 0) {
				//m_resut *= (1.0/ndenom);
				result /= ndenom;
				--ndenom;
			}
			else if (nq > 0) {
				result *= Q;
				--nq;
			}
			else if (np > 0) {
				result *= P;
				--np;
			}
			else {
				throw(ProbabilityException("Binomial Probability computation error- check success percentage between 0 and 1"));
			}
		}
		// If the result is less than one then we want to multiply
		// by a numerator digit. If we are out of denominator digits,
		// powers of p or powers of q then multiply rest of result
		// by numerator digits.
		else if (result < 1.0 || np==0 /* || nq ==0 || ndenom ==0 */ ) {
			if (nnumer >(Y-1-range)) {
				result *= nnumer;
				--nnumer;
			}
			else {
				throw(ProbabilityException("Binomial Probability computation error- unknown error"));
			}
		}
		else {
			throw(ProbabilityException("Binomial Probability computation error- possible value infinity or YaY"));
		}
	}
	return result;

}

// The Expected Value is defined by
// E(Y) = K / p
double NegativeBinomialProbability::GetExpectedValue() const
{
	if ( m_chance_of_success == 0.0 )
		return DBL_MAX;
	return m_kth_success/m_chance_of_success;
}

// The variance is defined by
// o^2 = K*(1-p)/p^2
double NegativeBinomialProbability::GetVariance() const
{
	// the limit of K*(1-p)/p^2 as p->0 is infinity
	if ( m_chance_of_success == 0.0)
		return DBL_MAX;
	return m_kth_success*(1.0 - m_chance_of_success)/(m_chance_of_success*m_chance_of_success);
}

void NegativeBinomialProbability::SetRV(int Y)
{
	assert(Y>=0);
	m_trials = Y;
}
