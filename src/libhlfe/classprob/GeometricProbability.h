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
// GeometricProbability.h: interface for the GeometricProbability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRICPROBABILITY_H__0AC8F099_FF59_4652_9C0B_CA299212896D__INCLUDED_)
#define AFX_GEOMETRICPROBABILITY_H__0AC8F099_FF59_4652_9C0B_CA299212896D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Probability.h"

// When to use a Geometric probability
/*
	The geometric probability asks the question, "Given the chance
	that each trial being successful is the same, what is the probability
	that the first success happens on a the Yth trial."
	This can be referred to as a russian roulette type of question.  If a
	revolver holds one bullet out of six chambers the chance for each
	trial is the same.  The probability answers the percentage that bullet
	will go off in each successive round given all the previous rounds
	were unsuccesful. This can be seen as the simple multiplication rule of
	probability, for example, P(Y=3) = chance of failure * chance of failure *
	chance of success = 5/6 * 5/6 * 1/6 = 25/216 or about 2%
 */
class GeometricProbability : public Probability
{
public:
	/*	Parameters for GeometricProbability
			Y - The number of the trial that the success occurs on. Must be 0 <= Y
			p - Chance of success for each trial.	Must be 0.0 <= p <= 1.0
			rvt -  Random variable comparison. Whether this probability is cumulative and which way it is.
	*/
	GeometricProbability(int Y=0, double p=1.0, Probability::RandomVariableType rvt=EQUAL);
	virtual ~GeometricProbability() {};
	virtual double GetExpectedValue() const;
	virtual double GetVariance() const;

protected:
	virtual void   SetRV(int Y);
	virtual double ComputeResult() throw (ProbabilityException);
	int m_trial_of_success;
	double m_chance_of_success;
};

#endif // !defined(AFX_GEOMETRICPROBABILITY_H__0AC8F099_FF59_4652_9C0B_CA299212896D__INCLUDED_)
