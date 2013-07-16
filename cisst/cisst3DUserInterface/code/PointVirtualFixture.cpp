/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
$Id: PointVirtualFixture.cpp 3148 2013-06-26 15:46:31Z oozgune1 $

Author(s):	Orhan Ozguner
Created on:	2013-06-26

(C) Copyright 2013 Johns Hopkins University (JHU), All Rights
Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#include <cisst3DUserInterface/PointVirtualFixture.h>
#include <cisstVector.h>

// PointVirtualFixture contructor that takes no argument.
PointVirtualFixture::PointVirtualFixture(void){
    PositionStiffnessPositive.SetAll(-500.0);
    PositionStiffnessNegative.SetAll(-500.0);
    PositionDampingPositive.SetAll(0.0);
    PositionDampingNegative.SetAll(0.0);
    ForceBiasPositive.SetAll(0.0);
    ForceBiasNegative.SetAll(0.0);
    OrientationStiffnessPositive.SetAll(0.0);
    OrientationStiffnessNegative.SetAll(0.0);
    OrientationDampingPositive.SetAll(0.0);
    OrientationDampingNegative.SetAll(0.0);
    TorqueBiasPositive.SetAll(0.0);
    TorqueBiasNegative.SetAll(0.0);
}

// PointVirtualFixture contructor that takes the point position and sets it.
PointVirtualFixture::PointVirtualFixture(const vct3 pt){
    setPoint(pt); //set point position

    PositionStiffnessPositive.SetAll(-500.0);
    PositionStiffnessNegative.SetAll(-500.0);
    PositionDampingPositive.SetAll(0.0);
    PositionDampingNegative.SetAll(0.0);
    ForceBiasPositive.SetAll(0.0);
    ForceBiasNegative.SetAll(0.0);
    OrientationStiffnessPositive.SetAll(0.0);
    OrientationStiffnessNegative.SetAll(0.0);
    OrientationDampingPositive.SetAll(0.0);
    OrientationDampingNegative.SetAll(0.0);
    TorqueBiasPositive.SetAll(0.0);
    TorqueBiasNegative.SetAll(0.0);
}

// Updates the point virtual fixture parameters.
void PointVirtualFixture::update(const vctFrm3 & pos , prmFixtureGainCartesianSet & vfParams) {
    vct3 position; //<! Force position.
    vctMatRot3 rotation; //<! Force orientation.
    //we set rotation to the initial condition
    //we do not need force orientation
    rotation = vctMatRot3::Identity(); 
    //force position is the given position
    position = getPoint();
    //set force position
    vfParams.SetForcePosition(position);
    //set force orientation
    vfParams.SetForceOrientation(rotation);
    //set torque orientation
    vfParams.SetTorqueOrientation(rotation);

    //set Position Stiffness
    vfParams.SetPositionStiffnessPos(PositionStiffnessPositive);
    vfParams.SetPositionStiffnessNeg(PositionStiffnessNegative);

    //Temporary hard code solution ask Anton for better way
    vfParams.SetPositionDampingPos(PositionDampingPositive);
    vfParams.SetPositionDampingNeg(PositionDampingNegative);
    vfParams.SetForceBiasPos(ForceBiasPositive);
    vfParams.SetForceBiasNeg(ForceBiasNegative);
    vfParams.SetOrientationStiffnessPos(OrientationStiffnessPositive);
    vfParams.SetOrientationStiffnessNeg(OrientationStiffnessNegative);
    vfParams.SetOrientationDampingPos(OrientationDampingPositive);
    vfParams.SetOrientationDampingNeg(OrientationDampingNegative);
    vfParams.SetTorqueBiasPos(TorqueBiasPositive);
    vfParams.SetTorqueBiasNeg(TorqueBiasNegative);
}

// Sets the point position.
void PointVirtualFixture::setPoint(const vct3 & pt){
    point = pt;
}

// Returns the point position.
vct3 PointVirtualFixture::getPoint(void){
    return point;
}

// Sets the given positive position stiffness constant.
void PointVirtualFixture::setPositionStiffnessPositive(const vct3 stiffPos){
    this->PositionStiffnessPositive = stiffPos;
}

// Sets the given negative position stiffness constant.
void PointVirtualFixture::setPositionStiffnessNegative(const vct3 stiffNeg){
    this->PositionStiffnessNegative = stiffNeg;
}

// Sets the given positive position damping constant.
void PointVirtualFixture::setPositionDampingPositive(const vct3 dampPos){
    this->PositionDampingPositive = dampPos;
}

// Sets the given negative position damping constant.
void PointVirtualFixture::setPositionDampingNegative(const vct3 dampNeg){
    this->PositionStiffnessNegative = dampNeg;
}

// Sets the given positive force bias constant.
void PointVirtualFixture::setForceBiasPositive(const vct3 biasPos){
    this->ForceBiasPositive = biasPos;
}

// Sets the given negative force bias constant.
void PointVirtualFixture::setForceBiasNegative(const vct3 biasNeg){
    this->ForceBiasNegative = biasNeg;
}

// Sets the given positive orientation stiffness constant.
void PointVirtualFixture::setOrientationStiffnessPositive(const vct3 orientStiffPos){
    this->OrientationStiffnessPositive = orientStiffPos;
}

// Sets the given negative orientation stiffness constant.
void PointVirtualFixture::setOrientationStiffnessNegative(const vct3 orientStiffNeg){
    this->OrientationStiffnessNegative = orientStiffNeg;
}

// Sets the given positive orientation damping constant.
void PointVirtualFixture::setOrientationDampingPositive(const vct3 orientDampPos){
    this->OrientationDampingPositive = orientDampPos;
}

// Sets the given negative orientation damping constant.
void PointVirtualFixture::setOrientationDampingNegative(const vct3 orientDampNeg){
    this->OrientationDampingNegative = orientDampNeg;
}

// Sets the given positive torque bias constant.
void PointVirtualFixture::setTorqueBiasPositive(const vct3 torqueBiasPos){
    this->TorqueBiasPositive = torqueBiasPos;
}

// Sets the given negative torque bias constant.
void PointVirtualFixture::setTorqueBiasNegative(const vct3 torqueBiasNeg){
    this->TorqueBiasNegative = torqueBiasNeg;
}

// Returns the positive position stiffness constant.
vct3 PointVirtualFixture::getPositionStiffnessPositive(void){
    return this->PositionStiffnessPositive;
}

// Returns the negative position stiffness constant.
vct3 PointVirtualFixture::getPositionStiffnessNegative(void){
    return this->PositionStiffnessNegative;
}

// Returns the positive position damping constant.
vct3 PointVirtualFixture::getPositionDampingPositive(void){
    return this->PositionDampingPositive;
}

// Returns the negative position damping constant.
vct3 PointVirtualFixture::getPositionDampingNegative(void){
    return this->PositionStiffnessNegative;
}

// Returns the positive force bias constant.
vct3 PointVirtualFixture::getForceBiasPositive(void){
    return this->ForceBiasPositive;
}

// Returns the negative force bias constant.
vct3 PointVirtualFixture::getForceBiasNegative(void){
    return this->ForceBiasNegative;
}

// Returns the positive orientation stiffness constant.
vct3 PointVirtualFixture::getOrientationStiffnessPositive(void){
    return this->OrientationStiffnessPositive;
}

// Returns the negative orientation stiffness constant.
vct3 PointVirtualFixture::getOrientationStiffnessNegative(void){
    return this->OrientationStiffnessNegative;
}

// Returns the positive orientation damping constant.
vct3 PointVirtualFixture::getOrientationDampingPositive(void){
    return this->OrientationDampingPositive;
}

// Returns the negative orientation damping constant.
vct3 PointVirtualFixture::getOrientationDampingNegative(void){
    return this->OrientationDampingNegative;
}

// Returns the positive torque bias constant.
vct3 PointVirtualFixture::getTorqueBiasPositive(void){
    return this->TorqueBiasPositive;
}

// Returns the negative torque bias constant.
vct3 PointVirtualFixture::getTorqueBiasNegative(void){
    return this->TorqueBiasNegative;
}
