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
// Probability.cpp: implementation of the Probability class.
//
//////////////////////////////////////////////////////////////////////
#include "Probability.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*	This is the public function that is callable to compute a probability
	or a cumulative probability. For cumulative probabilities it is assumed
	that the Random Variables range is at least computable from zero to Y.
	Optimizations could be made to compute cumulative probabilities faster
	for events that have a very large number of FINITE trials by taking the
	shorter of the computable ranges and possibly subtracting from one if
	necessary.

	If the probability is set in the constructor then the value won't
	be undefined and will be immediately returned.
*/
double Probability::GetResult()  throw (ProbabilityException)
{
	if (m_probability != UNDEFINED )
		return m_probability;
	try {
		m_probability = 0.0;
		int i = 0;
		switch (m_RVT)
		{
		case EQUAL:
			m_probability = ComputeResult();
			break;
		case LESSTHAN:
			for (SetRV(i); i < m_RV; SetRV(++i))
				m_probability += ComputeResult();
			break;
		case GREATERTHANOREQUAL:
			for (SetRV(i),m_probability = 1.0; i < m_RV; SetRV(++i))
				m_probability -= ComputeResult();
			break;
		case GREATERTHAN:
			for (SetRV(i),m_probability = 1.0; i <= m_RV; SetRV(++i))
				m_probability -= ComputeResult();
			break;
		case LESSTHANOREQUAL:
			for (SetRV(i); i <= m_RV; SetRV(++i))
				m_probability += ComputeResult();
			break;
		case NOTEQUAL:
			m_probability = 1.0 - ComputeResult();
			return m_probability;
		default:
			throw ProbabilityException("Probability error- Random Variable type is unset");
		}
	}
	catch (ProbabilityException pe)
	{
		m_probability = UNDEFINED;
		SetRV(m_RV);
		throw pe;
	}
	SetRV(m_RV);
	return m_probability;
}
