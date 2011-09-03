/**
 * @file
 * This file contains the declaration of Integer, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Declaration of Integer
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef Integer_H
#define Integer_H

#include "RbBoolean.h"
#include "RbLanguageObject.h"

#include <ostream>
#include <string>

class VectorString;

const std::string Integer_name = "Integer";

class Integer : public RbLanguageObject {

public:
    Integer(void);                                                                                      //!< Default constructor
    Integer(const bool v);                                                                              //!< Constructor from bool
    Integer(const int v);                                                                               //!< Constructor from int
    Integer(const unsigned int v);                                                                      //!< Constructor from unsigned int
    Integer(const unsigned long v);                                                                     //!< Constructor from unsigned long

    // Overloaded operator
    operator int(void) const { return value; }                                                          //!< Type conversion to int

    // Basic utility functions
    std::string                 briefInfo(void) const;                                                  //!< Brief info about the object
    virtual Integer*            clone(void) const;                                                      //!< Clone object
    virtual RbObject*           convertTo(const TypeSpec& type) const;                                  //!< Convert to type
    virtual const VectorString& getClass(void) const;                                                   //!< Get class vector
    virtual const TypeSpec&     getTypeSpec(void) const;                                                //!< Get language type of the object
    int&                        getValueReference(void) { return value; }                               //!< Get value reference for VectorInteger
    virtual bool                isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type and dim?
    virtual void                printValue(std::ostream& o) const;                                      //!< Print value (for user)
    virtual std::string         richInfo(void) const;                                                   //!< Complete info about object

    // Getters and setters
    virtual void                setValue(int x) { value = x; }                                          //!< Set value
    virtual int                 getValue(void) const { return value; }                                  //!< Get value

protected:
    int                         value;                                                                  //!< Value member
    
private:
    static const TypeSpec       typeSpec;
};

        // Operators defined outside of the class
        Integer                     operator+ (const Integer& A);                                           //!< Unary operator + 
        Integer                     operator- (const Integer& A);                                           //!< Unary operator - 
        RbBoolean                   operator! (const Integer& A);                                           //!< Unary operator !

#if 0
        Integer                     operator+ (const Integer& A, const Integer& B);                         //!< operator + 
        Integer                     operator- (const Integer& A, const Integer& B);                         //!< operator - 
        Integer                     operator* (const Integer& A, const Integer& B);                         //!< operator *
        Integer                     operator/ (const Integer& A, const Integer& B);                         //!< operator / 
        RbBoolean                   operator==(const Integer& A, const Integer& B);                         //!< operator == 
        RbBoolean                   operator!=(const Integer& A, const Integer& B);                         //!< operator != 
        RbBoolean                   operator< (const Integer& A, const Integer& B);                         //!< operator < 
        RbBoolean                   operator<=(const Integer& A, const Integer& B);                         //!< operator <= 
        RbBoolean                   operator> (const Integer& A, const Integer& B);                         //!< operator > 
        RbBoolean                   operator>=(const Integer& A, const Integer& B);                         //!< operator >= 
        Integer                     operator+ (const Integer& A);                                           //!< unary operator + 
        Integer                     operator- (const Integer& A);                                           //!< unary operator - 
#endif

#endif

