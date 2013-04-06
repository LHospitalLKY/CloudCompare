//##########################################################################
//#                                                                        #
//#                               CCLIB                                    #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 of the License.  #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef GENERIC_DISTRIBUTION_HEADER
#define GENERIC_DISTRIBUTION_HEADER

#ifdef _MSC_VER
//To get rid of the really annoying warnings about template class exportation
#pragma warning( disable: 4251 )
#pragma warning( disable: 4530 )
#endif

#include "CCTypes.h"

//system
#include <vector>

namespace CCLib
{

class GenericCloud;

//! A generic class to handle a probability distribution
/** Custom parametric distributions can be implemented through this
	interface and used for filtering data (see StatisticalTestingTools).
**/

#ifdef CC_USE_AS_DLL
#include "CloudCompareDll.h"


class CC_DLL_API GenericDistribution
#else
class GenericDistribution
#endif
{
public:

    //! Default constructor
    GenericDistribution() : m_isValid(false) {};

    //! Default destructor
    virtual ~GenericDistribution() {};

	//! Returns distribution name
	virtual const char* getName() const = 0; 

	//! Indicates if the distribution parameters are valid
	/** This function is related to 'computeParameters()'. If the parameters
		computation failed, then the parameters will be marked as 'invalid'. In this
		case, the dsitribution should'nt be used (most of the methods won't work anyway).
		\return true (if the distribution parameters are valid) or false (if not)
	**/
	virtual bool isValid() const { return m_isValid; }

	//! Computes the distribution parameters from a group of sample points
	/**	Virtual method to determine the distribution parameters from the
		scalar values.
		Warning: be sure to activate an OUTPUT scalar field on the input cloud
		\param cloud a point cloud with associated scalar values
		\return true (if the computation succeeded) or false (if not)
	**/
	virtual bool computeParameters(const GenericCloud* cloud) = 0;

	//! Computes the probability of x
	/** \param x the variable
		\return the probabilty
	**/
	virtual double computeP(ScalarType x) const = 0;

	//! Computes the cumulative probability between 0 and x
	/** \param x the upper boundary
		\return the cumulative probabilty
	**/
	virtual double computePfromZero(ScalarType x) const = 0;

	//! Computes the cumulative probability between x1 and x2
	/** x1 should be lower than x2
		\param x1 the lower boundary
		\param x2 the upper boundary
		\return the cumulative probabilty
	**/
	virtual double computeP(ScalarType x1, ScalarType x2) const = 0;

	//! Computes the Chi2 distance (related to the Chi2 Test)
	/** Computes the Chi2 distance from a group of point, accordingly to
		a certain number of classes (see Chi2 test theory for more information).
		The result of projecting each point (or more precisely each scalar value
		associated to each point) in the different classes can be stored in an array
		(of the same size as the number of classes). To do so, such an array (already
		allocated in memory) should be passed as an argument.
		Warning: be sure to activate an OUTPUT scalar field on the input cloud
		\param Yk a group of points
		\param numberOfClasses the number of classes for the Chi2 Test
		\param histo an array to store the values projection result (optionnal)
		\return the Chi2 distance (or -1.0 if an error occured)
	**/
	virtual double computeChi2Dist(const GenericCloud* Yk, unsigned numberOfClasses, int* histo=0) = 0;

protected:

	//! Sets distribution current validity
	void setValid(bool state) { m_isValid = state; }

	//! Whether the distribution is in a valid state or not
	bool m_isValid;
};

}

#endif //GENERIC_DISTRIBUTION_HEADER
