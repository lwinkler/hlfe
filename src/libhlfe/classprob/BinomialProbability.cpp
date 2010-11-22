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
// BinomialProbability.cpp: implementation of the BinomialProbability class.
//
//////////////////////////////////////////////////////////////////////


#include "BinomialProbability.h"
#include <assert.h>
#include <cstdlib>
#include <cmath>
#include <exception>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*  The discrete binomial probability.
	The equation for this probability is:
	P(Y) = N!/((N-Y)!Y!)*p^Y*(1-p)^(N-Y)
*/
BinomialProbability::BinomialProbability(int N , int Y, double p, Probability::RandomVariableType rvt)
		:Probability(Y, rvt),
		m_chance_of_success(p),
		m_trials(N),
		m_successes(Y)
{
	assert(Y<=N && Y >=0);
	assert(p >=0.0 && p<=1.0);
}

/*	Computing the Result.
	At first the distinction is made that the factorial part
	of the equation is going to be a large number, possibly
	outside of the range of an int type and causing overflow.
	The second part of the equation involves taking the power
	of two numbers below 1.0 which will yield a very small number
	and here significant digits may be truncated. We know a probability
	MUST be between zero and one so the general algorithm is to
	interleave the computing of the factorial side and the
	fractional power side of the equation so that the running
	result never strays far from being between zero and one.

    There are a couple optimizations for when the number of
	successes is zero or equal to the number of trials. Also
	of note is an optimization on the division of the factorial
	by factorial.  All of the digits in either N-Y or Y are
	reproduced in the numerator. The greatest number of cancellations
	is made depending on which is bigger, N-Y or Y, thus reducing
	significantly the number of multiplications.
*/
double BinomialProbability::ComputeResult() throw (ProbabilityException)
{
	if (m_trials == 0)
		return 0.0;
	// initialize some variables
	double result = 1.0;
	int Y = m_successes;
	int N = m_trials;
	double P = m_chance_of_success;
	double Q = 1.0 - P;
	int range = 0, np =0, nq = 0, nnumer = 0, ndenom = 0;
	// validate
	assert( Y<=N && Y >=0);
	assert( P <= 1.0 && P >=0.0);
	// check optimizations
	if (Y == 0) {
		return result = pow(Q,N);
	}
	if (Y == N) {
		return result = pow(P,Y);
	}
	// reorder the factorials to account for cancellations
	// in numerator and denominator.
	if (Y < N-Y) {
		range = Y;		// N-Y cancels out
	}
	else {
		range = N-Y;	// Y cancels out
	}
	np = Y;
	nq = N-Y;
	ndenom = range;
	nnumer = N;

	while (np > 0 || nq > 0 || ndenom > 0 || nnumer >(N-range)) {
		// If the result is greater than one we want to divide by
		// a denominator digit or multiply by percentage p or q.
		// If we are out of numerator digits then finish multiplying
		// with our powers of p or q or dividing by a denom digit.
		if (result >= 1.0 || nnumer ==(N-range)) {
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
		else if (result < 1.0 || np==0  ) {
			if (nnumer >(N-range)) {
				result *= nnumer;
				--nnumer;
			}
			else {
				throw(ProbabilityException("Binomial Probability computation error- unknown error"));
			}
		}
		else {
			throw(ProbabilityException("Binomial Probability computation error- possible value infinity or NaN"));
		}
	}
	return result;

}

/*	When the base class function, GetResult(), is called, if the probability
	is cumulative then GetResult will call to set the Random variable then
	compute the probability and add it to the cumulative total.
*/
void BinomialProbability::SetRV(int Y)
{
	assert( Y >=0 );
	assert( Y <= m_trials);
	m_successes = Y;
}

// The Expected Value or population mean is defined
// by E(V) = N*p
double BinomialProbability::GetExpectedValue() const
{
	return m_trials * m_chance_of_success;
}

// the Variance is defined by
// o^2 = N*p*q
double BinomialProbability::GetVariance() const
{
	return m_trials * m_chance_of_success * (1.0 - m_chance_of_success);
}
