//
//  Func_computeWeightedNodeOrderConstraintsScore.cpp
//  RevLanguage
//
//  Created by Bastien Boussau on 11/23/15.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "Func_computeWeightedNodeOrderConstraintsScore.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRelativeNodeAgeWeightedConstraints.h"
#include "ComputeWeightedNodeOrderConstraintsScoreFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_computeWeightedNodeOrderConstraintsScore::Func_computeWeightedNodeOrderConstraintsScore( void ) : TypedFunction<Real>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_computeWeightedNodeOrderConstraintsScore* Func_computeWeightedNodeOrderConstraintsScore::clone( void ) const
{
    
    return new Func_computeWeightedNodeOrderConstraintsScore( *this );
}


RevBayesCore::TypedFunction<double>* Func_computeWeightedNodeOrderConstraintsScore::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RelativeNodeAgeWeightedConstraints>* c = static_cast<const RlRelativeNodeAgeWeightedConstraints &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double>* bet =  static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    
    try {
        RevBayesCore::computeWeightedNodeOrderConstraintsScoreFunction* f = new RevBayesCore::computeWeightedNodeOrderConstraintsScoreFunction( tau, c, bet );
        return f;
    }
    catch (RbException e)
    {
        std::cerr << e.getMessage() << std::endl;
    }
    
    return NULL;
}


/* Get argument rules */
const ArgumentRules& Func_computeWeightedNodeOrderConstraintsScore::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), "The tree which is used to compute the MRCA.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "constraints", RlRelativeNodeAgeWeightedConstraints::getClassTypeSpec()   , "The constraints on relative node ages that are needed to compute the score of the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "beta", RealPos::getClassTypeSpec(), "The beta weight for the constraints.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_computeWeightedNodeOrderConstraintsScore::getClassType(void)
{
    
    static std::string revType = "Func_computeWeightedNodeOrderConstraintsScore";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_computeWeightedNodeOrderConstraintsScore::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_computeWeightedNodeOrderConstraintsScore::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "computeWeightedNodeOrderConstraintsScore";
    
    return f_name;
}


const TypeSpec& Func_computeWeightedNodeOrderConstraintsScore::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}