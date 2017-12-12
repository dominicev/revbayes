//
//  AdjacentRateModifierFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/3/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#include "AdjacentRateModifierFunction.h"
#include "AdjacentRateModifier.h"
#include "TypedDagNode.h"
#include <cmath>

using namespace RevBayesCore;

AdjacentRateModifierFunction::AdjacentRateModifierFunction(const TypedDagNode<double>* gf, const TypedDagNode<double>* lf, const TypedDagNode<long>* w, const TypedDagNode<RbVector<RbVector<long> > >* c, size_t ns, size_t nc) : TypedFunction<CharacterHistoryRateModifier>( new AdjacentRateModifier(ns, nc) ),
    gainFactor(gf),
    lossFactor(lf),
    width(w),
    context_matrix(c),
    context_type("width")
{
    if (context_matrix != NULL) {
        context_type = "matrix";
    }
    
    // add the parameters as parents
    addParameter(gainFactor);
    addParameter(lossFactor);
    addParameter(width);
    addParameter(context_matrix);
    
    update();
}

AdjacentRateModifierFunction::AdjacentRateModifierFunction(const AdjacentRateModifierFunction& m) : TypedFunction<CharacterHistoryRateModifier>( m )
{
    gainFactor = m.gainFactor;
    lossFactor = m.lossFactor;
    width = m.width;
    context_matrix = m.context_matrix;
    context_type = m.context_type;
}


AdjacentRateModifierFunction::~AdjacentRateModifierFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}





AdjacentRateModifierFunction* AdjacentRateModifierFunction::clone( void ) const
{
    return new AdjacentRateModifierFunction( *this );
}


void AdjacentRateModifierFunction::update( void )
{
    
    double gf = gainFactor->getValue();
    static_cast<AdjacentRateModifier*>(value)->setGainFactor(gf);
    
    double lf = lossFactor->getValue();
    static_cast<AdjacentRateModifier*>(value)->setLossFactor(lf);
    
    
    if (context_type == "width")
    {
        size_t w = width->getValue();
        static_cast<AdjacentRateModifier*>(value)->setWidth(w);
    }
    else if (context_type=="matrix")
    {
        RbVector<RbVector<long> > c = context_matrix->getValue();
        static_cast<AdjacentRateModifier*>(value)->setContextMatrix(c);
    }
    
}



void AdjacentRateModifierFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == gainFactor)
    {
        gainFactor = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == lossFactor)
    {
        lossFactor = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == width)
    {
        width = static_cast<const TypedDagNode<long>* >( newP );
    }
    else if (oldP == context_matrix)
    {
        context_matrix = static_cast<const TypedDagNode<RbVector<RbVector<long> > >* >( newP );
    }
}
