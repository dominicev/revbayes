/**
 * @file
 * This file contains the implementation of CharacterStateDiscrete, which is
 * the abstract base class for discrete character data types in RevBayes.
 *
 * @brief Implementation of CharacterStateDiscrete
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "CharacterStateDiscrete.h"
#include "CondLike.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"



/** Constructor taking the number of states */
CharacterStateDiscrete::CharacterStateDiscrete(size_t n) : Character(), numStates(n) {

    value.resize(numStates);
}


/** Get class vector describing type of object */
const VectorString& CharacterStateDiscrete::getClass() const {

    static VectorString rbClass = VectorString( CharacterStateDiscrete_name ) + RbObject::getClass();
    return rbClass;
}


/** Initialize and return a conditional likelihood for this state */
CondLike* CharacterStateDiscrete::getCondLike(void) const {

    CondLike* cl = new CondLike(1, numStates);
    double* p = &(*cl)[0];
    for (size_t i=0; i<numStates; i++)
        {
        if (value[i] == true)
            p[i] = 1.0;
        else
            p[i] = 0.0;
        }
    return cl;
}


/** Return the number of observed states for this character (any number greater than one indicates ambiguity) */
size_t CharacterStateDiscrete::getNumOnStates(void) const {

    size_t nOn = 0;
    for (size_t i=0; i<value.size(); i++)
        {
        if (value[i] == true)
            nOn++;
        }
    return nOn;
}


/** Get the unsigned representation for this character */
unsigned CharacterStateDiscrete::getUnsignedValue(void) const {

    if ( sizeof(unsigned) * 8 < value.size() )
        throw ( RbException("Too many states to represent as an unsigned") );
	unsigned val = 0;
	for (size_t i=0; i<value.size(); i++)
		{
		if (value[i] == true)
			{
			unsigned mask = 1 << i ;
			val |= mask;
			}
		}
	return val;
}


/** Is the character missing or ambiguous */
bool CharacterStateDiscrete::isMissAmbig(void) const {

    if ( getNumOnStates() > 1 )
        return true;
    return false;
}


/** Set the number of states */
void CharacterStateDiscrete::setNumStates(size_t n) {

    numStates = n;
    value.resize(n);
}


