/**
 * @file
 * This file contains the declaration of RealPos, which
 * is the primitive RevBayes type for positive real numbers.
 *
 * @brief Declaration of RealPos
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RealPos_H
#define RealPos_H

#include "Real.h"

#include <ostream>
#include <string>

class VectorString;

const std::string RealPos_name = "+Real";

class RealPos : public Real {

    public:
                                        RealPos(void);                          //!< Default constructor
                                        RealPos(double x);                      //!< Construct from double
                                        RealPos(int x);                         //!< Construct from int 
                                        RealPos(unsigned int x);                //!< Construct from unsigned int 
                                        RealPos(bool x);                        //!< Construct from bool

        // Basic utility functions
        virtual std::string             briefInfo(void) const;                  //!< Brief info about the object
        virtual RealPos*                clone(void) const;                      //!< Clone object
        virtual const VectorString&     getClass(void) const;                   //!< Get class vector
        virtual const TypeSpec&         getTypeSpec(void) const;                //!< Get language type of the object
        virtual std::string             richInfo(void) const;                   //!< Complete info about object

        // Setter disallowing nonpositive values
        virtual void                    setValue(double x);                     //!< Set value
    
    private:
        static const TypeSpec           typeSpec;
};

#endif

