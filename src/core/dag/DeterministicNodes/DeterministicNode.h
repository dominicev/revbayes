/**
 * @file
 * This file contains the declaration of the deterministic DAG node class, which is our base class for all deterministc DAG nodes.
 * Deterministic DAG nodes reevaluate their value when the value of their parents change. Deterministic DAG nodes are used for
 * parameter transformations. Derived class are simple functions, e.g. exp, ln or GTR.
 *
 * @brief Declaration of the deterministic DAG node base class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id$
 */



#ifndef DeterministicNode_H
#define DeterministicNode_H

#include "DynamicNode.h"
#include "TypedFunction.h"

namespace RevBayesCore {
    
    template<class valueType>
    class DeterministicNode : public DynamicNode<valueType> {
        
    public:
        DeterministicNode(const std::string &n, TypedFunction<valueType> *f);
        DeterministicNode(const DeterministicNode<valueType> &n);                                                                       //!< Copy constructor
        virtual                                            ~DeterministicNode(void);                                                    //!< Virtual destructor
        
        DeterministicNode&                                  operator=(const DeterministicNode& n);                                      //!< Assignment operator
        
        // public methods
        DeterministicNode<valueType>*                       clone(void) const;
        virtual TypedFunction<valueType>&                   getFunction(void);
        virtual const TypedFunction<valueType>&             getFunction(void) const;
        double                                              getLnProbability(void);
        double                                              getLnProbabilityRatio(void);
        valueType&                                          getValue(void);
        const valueType&                                    getValue(void) const;
        bool                                                isConstant(void) const;                                                     //!< Is this DAG node constant?
        virtual void                                        printStructureInfo(std::ostream &o, bool verbose=false) const;              //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)
        void                                                update(void);                                                               //!< Update the current value by recomputation
        void                                                redraw(void);
        void                                                reInitializeMe(void);                                                       //!< The DAG was re-initialized so maybe you want to reset some stuff (delegate to distribution)
        
        // Parent DAG nodes management functions
        virtual std::set<const DagNode*>                    getParents(void) const;                                                     //!< Get the set of parents
        virtual void                                        swapParent(const DagNode *oldParent, const DagNode *newParent);             //!< Exchange the parent (function parameter)
        
    protected:
        void                                                getAffected(std::set<DagNode *>& affected, DagNode* affecter);              //!< Mark and get affected nodes
        bool                                                isFunctionDirty(void) const;                                                //!< Is my function dirty?
        void                                                keepMe(DagNode* affecter);                                                  //!< Keep value of this and affected nodes
        void                                                restoreMe(DagNode *restorer);                                               //!< Restore value of this nodes
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                    //!< Swap the parameter of this node (needs overwriting in deterministic and stochastic nodes)
        virtual void                                        touchMe(DagNode *toucher);                                                  //!< Touch myself and tell affected nodes value is reset
        void                                                touchFunction(DagNode* toucher) const;                                      //!< Touch my function
        
    private:
        TypedFunction<valueType>*                           function;
    };
    
}

#include <cassert>
#include "RbOptions.h"


template<class valueType>
RevBayesCore::DeterministicNode<valueType>::DeterministicNode( const std::string &n, TypedFunction<valueType> *f ) :
DynamicNode<valueType>( n ),
function( f )
{
    this->type = DagNode::DETERMINISTIC;
    
    // Get the parameters from the function and add us as a child of them in the DAG
    const std::set<const DagNode*>& funcParents = function->getParameters();
    for (std::set<const DagNode*>::iterator it = funcParents.begin(); it != funcParents.end(); ++it)
    {
        (*it)->addChild( this );
        
        // Increment the reference count
        // We don't want this parent to get deleted while we are still alive
        (*it)->incrementReferenceCount();
    }
    
    // Set us as the DAG node of the function
    function->setDeterministicNode( this );
}


template<class valueType>
RevBayesCore::DeterministicNode<valueType>::DeterministicNode( const DeterministicNode<valueType> &n ) :
DynamicNode<valueType>( n ),
function( n.function->clone() )
{
    this->type = DagNode::DETERMINISTIC;
    
    // Get the parameters from the function and add us as a child of them in the DAG
    const std::set<const DagNode*>& funcParents = function->getParameters();
    for (std::set<const DagNode*>::iterator it = funcParents.begin(); it != funcParents.end(); ++it)
    {
        (*it)->addChild( this );
        
        // Increment the reference count
        // We don't want this parent to get deleted while we are still alive
        (*it)->incrementReferenceCount();
    }
    
    // Set us as the DAG node of the function
    function->setDeterministicNode( this );
}


template<class valueType>
RevBayesCore::DeterministicNode<valueType>::~DeterministicNode( void ) {
    
    // Remove us as the child of the function parameters
    std::set<const DagNode*> funcParents = function->getParameters();
    delete function;
    for (std::set<const DagNode*>::iterator it = funcParents.begin(); it != funcParents.end(); ++it)
    {
        (*it)->removeChild( this );
        
        // Decrement the reference count and check whether we need to delete the DAG node
        // The function does not do this for us
        if ( (*it)->decrementReferenceCount() == 0)
            delete (*it);
    }
    
}


/**
 * Assignment operator. Make sure we deal with parent nodes correctly here.
 */
template<class valueType>
RevBayesCore::DeterministicNode<valueType>& RevBayesCore::DeterministicNode<valueType>::operator=( const DeterministicNode<valueType>& n )
{
    
    if ( &n != this )
    {
        // Call base class assignment operator
        DynamicNode<valueType>::operator=( n );
        
        // Remove us as the child of the function parameters
        const std::set<const DagNode*>& funcParents = function->getParameters();
        for (std::set<const DagNode*>::iterator it = funcParents.begin(); it != funcParents.end(); ++it)
        {
            (*it)->removeChild( this );
            
            // Decrement the reference count and check whether we need to delete the DAG node
            // The function does not do this for us
            if ( (*it)->decrementReferenceCount() == 0)
                delete (*it);
        }
        
        // Delete the function
        delete function;
        
        // Recreate the function
        function = n.function->clone();
        
        // Get the parameters from the new function and add us as child of them in the DAG
        funcParents = function->getParameters();
        for (std::set<const DagNode*>::iterator it = funcParents.begin(); it != funcParents.end(); ++it)
        {
            (*it)->addChild( this );
            
            // Increment the reference count
            // We don't want this parent to get deleted while we are still alive
            (*it)->incrementReferenceCount();
        }
        
        // Set us as the DAG node of the new function
        function->setDeterministicNode( this );
    }
    
    return *this;
}


template<class valueType>
RevBayesCore::DeterministicNode<valueType>* RevBayesCore::DeterministicNode<valueType>::clone( void ) const {
    
    return new DeterministicNode<valueType>( *this );
}

/**
 * Get the affected nodes.
 * This call is started by the parent. We need to delegate this call to all our children.
 */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    
    this->getAffectedNodes( affected );
    
}


template<class valueType>
RevBayesCore::TypedFunction<valueType>& RevBayesCore::DeterministicNode<valueType>::getFunction( void ) {
    
    return *function;
}


template<class valueType>
const RevBayesCore::TypedFunction<valueType>& RevBayesCore::DeterministicNode<valueType>::getFunction( void ) const
{
    
    return *function;
}

template<class valueType>
double RevBayesCore::DeterministicNode<valueType>::getLnProbability( void )
{
    
    return 0.0;
}


template<class valueType>
double RevBayesCore::DeterministicNode<valueType>::getLnProbabilityRatio( void )
{
    
    return 0.0;
}


/**
 * Get the parents of this node. Simply ask the function to provide its parameters,
 * no need to keep parents here.
 */
template<class valueType>
std::set<const RevBayesCore::DagNode*> RevBayesCore::DeterministicNode<valueType>::getParents( void ) const
{
    return function->getParameters();
}


template<class valueType>
valueType& RevBayesCore::DeterministicNode<valueType>::getValue( void )
{
    
    return function->getValue();
}


template<class valueType>
const valueType& RevBayesCore::DeterministicNode<valueType>::getValue( void ) const
{
    
    return function->getValue();
}


template<class valueType>
bool RevBayesCore::DeterministicNode<valueType>::isConstant( void ) const
{
    
    // iterate over all parents and only if all parents are constant then this node is constant too
    const std::set<const DagNode*>& parents = function->getParameters();
    for (std::set<const DagNode*>::iterator it = parents.begin(); it != parents.end(); ++it)
    {
        if ( !(*it)->isConstant() )
        {
            return false;
        }
    }
    
    return true;
}


/** check whether function is dirty (function provided for derived classes) */
template<class valueType>
bool RevBayesCore::DeterministicNode<valueType>::isFunctionDirty( void ) const
{
    
    return function->isDirty();
}


/**
 * Keep the current value of the node.
 * At this point, we just delegate to the children.
 */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::keepMe( DagNode* affecter )
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In keepMe of deterministic node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // allow specialized recovery in functions
    function->keep( affecter );
    
    // delegate call
    this->keepAffected();
    
    // clear the list of touched element indices
    this->touchedElements.clear();
    
}


/** Print struct for user */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::printStructureInfo( std::ostream& o, bool verbose ) const
{
    
    if ( verbose == true )
    {
        o << "_dagNode      = " << this->name << " <" << this << ">" << std::endl;
    }
    else
    {
        if ( this->name != "")
            o << "_dagNode      = " << this->name << std::endl;
        else
            o << "_dagNode      = <" << this << ">" << std::endl;
    }
    o << "_dagType      = Deterministic DAG node" << std::endl;
    
    if ( verbose == true )
    {
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
        o << "_function     = <" << function << ">" << std::endl;
        o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    }
    
    
    o << "_parents      = ";
    this->printParents(o, 16, 70, verbose);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o, 16, 70, verbose);
    o << std::endl;
}



template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::redraw( void )
{
    // nothing to do
    // the touch should have called our update
}


template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::reInitializeMe( void )
{
    
    function->reInitialized();
    
}


/** Restore the old value of the node and tell affected */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::restoreMe( DagNode *restorer )
{
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In restoreMe of Deterministic node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // we need to recompute our value?!
    this->update();
    
    // we just mark ourselves as clean
    this->touched = false;
    
    // call for potential specialized handling (e.g. internal flags)
    function->restore(restorer);
    
    // delegate call
    this->restoreAffected();
    
    // clear the list of touched element indices
    this->touchedElements.clear();
    
}


/**
 * This function replaces the earlier swapParameter function. If we rely on the
 * internal RevBayesCore::Function to manage our parents, we simply need to ask
 * the function to swap its parameters, and then manage the connection of the
 * parents (parameters) to this node.
 */
template <class valueType>
void RevBayesCore::DeterministicNode<valueType>::swapParent( const RevBayesCore::DagNode *oldParent, const RevBayesCore::DagNode *newParent )
{
    // We are sure to get into trouble if either one of these is NULL
    if( oldParent == NULL || newParent == NULL )
        throw RbException( "Attempt to swap NULL function parameter of RevBayesCore::DeterministicNode" );
    
    // This throws an error if the oldParent cannot be found
    function->swapParameter( oldParent, newParent );
    
    oldParent->removeChild( this );
    if ( oldParent->decrementReferenceCount() == 0 )
        delete ( oldParent );
    
    newParent->addChild( this );
    newParent->incrementReferenceCount();
    
    this->touch();
}


/** touch my function for recalculation (function provided for derived classes) */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::touchFunction( DagNode* toucher ) const {
    
    // call for potential specialized handling (e.g. internal flags), we might have been touched already by someone else, so we need to delegate regardless
    // This is essential for lazy evaluation
    function->touch( toucher );
}


/**
 * Touch this node for recalculation.
 *
 * @todo The touchAffected() call only needs to be executed if the node
 *       has not been touched before the entry to this function. The
 *       touchFunction call always needs to be executed (at least once
 *       for each toucher).
 *
 * @todo Get rid of the touched flag. It is not used, and any code relying on
 *       it to be set correctly might well fail.
 */
template<class valueType>
void RevBayesCore::DeterministicNode<valueType>::touchMe( DagNode *toucher ) {
    
#ifdef DEBUG_DAG_MESSAGES
    std::cerr << "In touchMe of deterministic node " << this->getName() << " <" << this << ">" << std::endl;
#endif
    
    // To be on the safe side, we set the touched flag here, but the flag is not used by this class and may not
    // be in a consistent state. Beware!
    this->touched = true;
    
    // We need to touch the function always because of specialized touch functionality in some functions, like vector functions.
    // In principle, it would sufficient to do the touch once for each toucher, but we do not keep track of the touchers here.
    this->touchFunction( toucher );
    
    // Dispatch the touch message to downstream nodes
    this->touchAffected();
}


template <class valueType>
void RevBayesCore::DeterministicNode<valueType>::update()
{
    
    function->update();
    
}


#endif
