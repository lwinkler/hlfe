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
// Probability.h: interface for the Probability class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4290)

#if !defined(AFX_PROBABILITY_H__34941F48_695B_4409_9785_75DD232B50C0__INCLUDED_)
#define AFX_PROBABILITY_H__34941F48_695B_4409_9785_75DD232B50C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define UNDEFINED -1.0

class Probability
{
public:
	enum RandomVariableType { EQUAL, LESSTHAN, GREATERTHAN, LESSTHANOREQUAL, GREATERTHANOREQUAL, NOTEQUAL };

	/* Constructor Parameters:
		rv - The Random Variable value.
		rvt - Random Variable type, whether it is cumulative and which way it is cumulative.
		prob - The probability of the event.  This can be set before hand if it is known
				and the GetResult function will return it.
	*/
	Probability( int rv=0, RandomVariableType rvt=EQUAL, double prob=UNDEFINED)
			:m_RV(rv), m_RVT(rvt), m_probability(prob) {};
	virtual ~Probability() {};

	class ProbabilityException
	{
	public:
		ProbabilityException(const char* whatString):What(whatString) {}
		inline const char* what() const { return What; }
	protected:
		const char* What;
	};

	virtual double GetResult() throw (ProbabilityException) ;
	virtual double GetExpectedValue() const = 0;
	virtual double GetVariance() const = 0;
protected:
	virtual double ComputeResult() = 0;
	virtual void   SetRV(int Y)    = 0;
	int m_RV;
	RandomVariableType m_RVT;
	double m_probability;
};

#endif // !defined(AFX_PROBABILITY_H__34941F48_695B_4409_9785_75DD232B50C0__INCLUDED_)
