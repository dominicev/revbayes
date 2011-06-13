/**
 * @file
 * This file contains the declaration of MethodTable, which is
 * used to hold member functions of complex objects.
 *
 * @brief Implementation of MethodTable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"

#include <sstream>


/** Basic constructor, empty table with or without parent */
MethodTable::MethodTable(MethodTable* parent)
    : FunctionTable(parent) {
}


/** Add function to table */
void MethodTable::addFunction( const std::string name, RbFunction* func ) {

    FunctionTable::addFunction( name, func );

    if ( func->isType( MemberFunction_name ) )
        static_cast<MemberFunction*>( func )->setMethodName( name );
}


/** Return brief info about object */
std::string MethodTable::briefInfo () const {

    std::ostringstream   o;
    o << "MethodTable with " << table.size() << " functions";

    return o.str();
}


/** Complete info about object */
std::string MethodTable::richInfo(void) const {

    std::ostringstream o;
    if (table.size() == 0)
        o << "MethodTable with no entries" << std::endl;
    else
        o << "MethodTable with " << table.size() << " entries:" << std::endl;
    printValue(o);

    return o.str();
}

