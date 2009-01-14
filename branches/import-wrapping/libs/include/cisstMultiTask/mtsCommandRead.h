/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: mtsCommandRead.h,v 1.5 2008/09/05 04:31:10 anton Exp $

  Author(s):  Ankur Kapoor, Peter Kazanzides, Anton Deguet
  Created on: 2004-04-30

  (C) Copyright 2004-2008 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


/*!
  \file
  \brief Defines a command with one argument 
*/

#ifndef _mtsCommandRead_h
#define _mtsCommandRead_h


#include <cisstMultiTask/mtsCommandReadOrWriteBase.h>
#include <cisstMultiTask/mtsDevice.h>


/*!
  \ingroup cisstMultiTask

  A templated version of command object with one argument for
  execute. The template argument is the interface type whose method is
  contained in the command object. */
template <class _classType, class _argumentType>
class mtsCommandRead: public mtsCommandReadBase {
public:
    typedef _argumentType ArgumentType;
    typedef mtsCommandReadBase BaseType;

    /*! Typedef for the specific interface. */
    typedef _classType ClassType;

    /*! Typedef for pointer to member function of the specific interface
      class. */
    typedef void(_classType::*ActionType)(ArgumentType &) const;

protected:
    /*! The pointer to member function of the receiver class that
      is to be invoked for a particular instance of the command*/
    ActionType Action;

    /*! Stores the receiver object of the command */
    ClassType * ClassInstantiation;

    /*! Argument prototype */
    ArgumentType ArgumentPrototype;

public:
    /*! The constructor. Does nothing */
    mtsCommandRead(void): BaseType(), ClassInstantiation(0) {}
    
    
    /*! The constructor.
      \param action Pointer to the member function that is to be called
      by the invoker of the command
      \param interface Pointer to the receiver of the command
      \param name A string to identify the command. */
    mtsCommandRead(ActionType action, ClassType * classInstantiation, const std::string & name,
                   const ArgumentType & argumentPrototype):
        BaseType(name),
        Action(action),
        ClassInstantiation(classInstantiation),
        ArgumentPrototype(argumentPrototype)
    {}


    /*! The destructor. Does nothing */
    virtual ~mtsCommandRead() {}

    
    /*! The execute method. Calling the execute method from the invoker
      applies the operation on the receiver. 
      \param obj The data passed to the operation method
    */
    virtual mtsCommandBase::ReturnType Execute(cmnGenericObject & argument) {
        ArgumentType * data = dynamic_cast< ArgumentType * >(&argument);
        if (data == NULL)
            return mtsCommandBase::BAD_INPUT;
        (ClassInstantiation->*Action)(*data);
        return mtsCommandBase::DEV_OK;
    }

    /* commented in base class */
    const cmnGenericObject * GetArgumentPrototype(void) const {
        return &ArgumentPrototype;
    }

    /* commented in base class */
    virtual void ToStream(std::ostream & outputStream) const {
        outputStream << "mtsCommandRead: ";
        if (this->ClassInstantiation) {
            outputStream << this->Name << "(" << this->ArgumentPrototype.ClassServices()->GetName() << "&) using class/object \""
                         << mtsObjectName(this->ClassInstantiation) << "\"";
        } else {
            outputStream << "Not initialized properly";
        }
    }
    
};


#endif // _mtsCommandRead_h

