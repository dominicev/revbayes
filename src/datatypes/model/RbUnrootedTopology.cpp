/*
 * MCMC.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Sebastian
 */

#include <cmath>
#include "DAGNode.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbUnrootedTopology.h"
#include "RbModel.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"

RbUnrootedTopology::RbUnrootedTopology(RbNode* bn) : RbTopology(bn){

}

RbUnrootedTopology::RbUnrootedTopology(const RbUnrootedTopology& m) : RbTopology(m) {

	// TODO: Deep copy of model needs to be performed
}

RbUnrootedTopology::~RbUnrootedTopology(void) {

}



RbObject* RbUnrootedTopology::clone() const {
	RbUnrootedTopology* x = new RbUnrootedTopology(*this);
	return (RbObject*)x;
}

bool RbUnrootedTopology::equals(const RbObject* obj) const {
	return (this == obj);
}

const StringVector& RbUnrootedTopology::getClass(void) const {
    static StringVector rbClass = StringVector(RbNames::UnrootedTopology::name) + RbTopology::getClass();
    return rbClass;
}

RbObject& RbUnrootedTopology::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbUnrootedTopology& x = dynamic_cast<const RbUnrootedTopology&> (obj);

        RbUnrootedTopology& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbUnrootedTopology& x = dynamic_cast<const RbUnrootedTopology&> (*(obj.convertTo(RbNames::UnrootedTopology::name)));

            RbUnrootedTopology& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::UnrootedTopology::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbUnrootedTopology& RbUnrootedTopology::operator=(const RbUnrootedTopology& obj) {

    
    return (*this);
}

void RbUnrootedTopology::print(std::ostream& o) const {
	o << "Unrooted Tree" << std::endl;
}

void RbUnrootedTopology::printValue(std::ostream& o) const {
//	o << "Unrooted Tree" << std::endl;
}

std::string RbUnrootedTopology::toString(void) const {
	std::string tmp = "Unrooted Topology";
	return tmp;
}

