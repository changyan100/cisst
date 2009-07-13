/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
Author(s):	Marcin Balicki
Created on:   2008-09-14

(C) Copyright 2008 Johns Hopkins University (JHU), All Rights
Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


/*! 
\file 
\brief Actuator State query parameters.
*/


#ifndef _prmActuatorState_h
#define _prmActuatorState_h

//basic includes
#include <cisstCommon/cmnGenericObject.h>
#include <cisstOSAbstraction.h>
#include <cisstCommon/cmnClassRegisterMacros.h>
#include <cisstMultiTask/mtsStateIndex.h>
#include <cisstParameterTypes/prmTypes.h>
#include <cisstMultiTask/mtsMacros.h>
#include <cisstMultiTask/mtsVector.h>

// Always include last
#include <cisstParameterTypes/prmExport.h>

/*! A container for Controlloer's Actuator states
   Position, Velocity, etc. for each actuator
*/
class CISST_EXPORT prmActuatorState: public mtsGenericObject
{
    CMN_DECLARE_SERVICES(CMN_DYNAMIC_CREATION, 5);

public:
    typedef prmActuatorState ThisType;
    typedef unsigned int size_type;

    /*! default constructor - does nothing for now */
    inline prmActuatorState() {}

    prmActuatorState(size_type size);

    void SetSize(size_type size);

    ///*! constructor with all possible parameters */
    //inline prmActuatorState(const prmPosition & position,
    //                           const mtsStateIndex & stateIndex):
    //    PositionMember(position),
    //    StateIndexMember(stateIndex)
    //{}

    /*! destructor */
    virtual ~prmActuatorState() {};

    /*! Allocate memory based on an existing object of the same type.  The
    object is provided via a cmnGenericObject pointer.  If a dynamic cast
    to this type is not possible, this method returns false. */
    bool inline Allocate(const cmnGenericObject * model)
    {
        const ThisType * pointer = dynamic_cast<const ThisType *>(model);
        if (pointer == 0) {
            return false;
        }
        this->PositionMember.SetSize(pointer->Position().size());
        this->VelocityMember.SetSize(pointer->Velocity().size());
        this->InMotionMember.SetSize(pointer->InMotion().size());
        this->MotorOffMember.SetSize(pointer->MotorOff().size());
        this->SoftFwdLimitHitMember.SetSize(pointer->SoftFwdLimitHit().size());
        this->SoftRevLimitHitMember.SetSize(pointer->SoftRevLimitHit().size());
        this->HardFwdLimitHitMember.SetSize(pointer->HardFwdLimitHit().size());
        this->HardRevLimitHitMember.SetSize(pointer->HardRevLimitHit().size());
        this->IsHomedMember.SetSize(pointer->IsHomed().size());

        return true;
    }

    /*! Set and Get methods for the the position. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(mtsDoubleVec, Position);
    //@}

    /*! Set and Get methods for the the Velocity. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(mtsDoubleVec, Velocity);
    //@}


    /*! Set and Get methods for the the position. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, InMotion);
    //@}

    /*! Set and Get methods for the the position. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, MotorOff);
    //@}


    /*! Set and Get methods for the forward SOFTWARE limits hit. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, SoftFwdLimitHit);
    //@}


    /*! Set and Get methods for the reverse SOFTWARE Limit. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, SoftRevLimitHit);
    //@}

        /*! Set and Get methods for the forward HARDWARE limits hit. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, HardFwdLimitHit);
    //@}

    /*! Set and Get methods for the reverse HARDWARE Limit. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, HardRevLimitHit);
    //@}


    /*! Set and Get methods for the  HomeSwitch . */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, HomeSwitchOn);
    //@}

    /*! Set and Get methods for state of homing variable on the controller. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(prmBoolVec, IsHomed);
    //@}

    /*! Set and Get methods for the state of the estop button. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(bool, EStopON);
    //@}


    /*! Set and Get methods for time index.  Current time index, as
    provided for writer of the task providing the position
    data. */
    //@{
    MTS_DECLARE_MEMBER_AND_ACCESSORS(mtsStateIndex, StateIndex);
    //@}

    /*! Human readable output to stream. */
    void ToStream(std::ostream & outputStream) const;


}; 


CMN_DECLARE_SERVICES_INSTANTIATION(prmActuatorState);

#endif // _prmActuatorState_h
