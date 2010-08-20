/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: main.cpp 1239 2010-02-27 04:40:27Z mjung5 $
  
  Author(s):  Min Yang Jung
  Created on: 2010-08-20

  (C) Copyright 2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include <cisstCommon/cmnPortability.h>
#include <cisstCommon/cmnLogger.h>
#include <cisstOSAbstraction/osaSleep.h>
#include <cisstOSAbstraction/osaThreadedLogFile.h>
#include <cisstMultiTask/mtsManagerGlobal.h>

#include "sineTask.h"

int main(int CMN_UNUSED(argc), char ** CMN_UNUSED(argv))
{
    // log configuration
    cmnLogger::SetLoD(CMN_LOG_LOD_VERY_VERBOSE);
    cmnLogger::GetMultiplexer()->AddChannel(std::cout, CMN_LOG_LOD_VERY_VERBOSE);
    // add a log per thread
    osaThreadedLogFile threadedLog("GlobalComponentManager");
    cmnLogger::GetMultiplexer()->AddChannel(threadedLog, CMN_LOG_LOD_VERY_VERBOSE);
    // specify a higher, more verbose log level for these classes
    cmnClassRegister::SetLoD("mtsManagerGlobal", CMN_LOG_LOD_VERY_VERBOSE);
    cmnClassRegister::SetLoD("sineTask", CMN_LOG_LOD_VERY_VERBOSE);

    // Create and start global component manager
    mtsManagerGlobal globalComponentManager;
    if (!globalComponentManager.StartServer()) {
        CMN_LOG_INIT_ERROR << "Failed to start global component manager." << std::endl;
        return 1;
    }
    CMN_LOG_INIT_VERBOSE << "Global component manager started..." << std::endl;

    // Get local component manager instance
    mtsManagerLocal * localManager;
    try {
        localManager = mtsManagerLocal::GetInstance(globalComponentManager);
    } catch (...) {
        CMN_LOG_INIT_ERROR << "Failed to initialize local component manager" << std::endl;
        return 1;
    }

    // Create and configure an instance of Sine Task
    const double PeriodSine = 1 * cmn_ms; // in milliseconds
    sineTask * sineTaskObject = new sineTask("SIN", PeriodSine);

    // Add sine task to the LCM that runs with the GCM
    localManager->AddTask(sineTaskObject);

    // create the tasks, i.e. find the commands
    localManager->CreateAll();
    // start the periodic Run
    localManager->StartAll();

    while (1) {
        osaSleep(10 * cmn_ms);
    }

    /*
    // cleanup
    taskManager->KillAll();

    osaSleep(PeriodSine * 2);
    while (!sineTaskObject->IsTerminated()) osaSleep(PeriodSine);

    taskManager->Cleanup();
    */

    return 0;
}
