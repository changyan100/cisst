/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: mtsMailBox.h,v 1.4 2008/09/05 04:31:10 anton Exp $

  Author(s):  Peter Kazanzides
  Created on: 2007-09-05

  (C) Copyright 2007-2008 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


/*!
  \file
  \brief Defines a mailbox for communication between tasks.
*/

#ifndef _mtsMailBox_h
#define _mtsMailBox_h

#include <cisstMultiTask/mtsDeviceInterface.h>
#include <cisstMultiTask/mtsQueue.h>

// Always include last
#include <cisstMultiTask/mtsExport.h>


class CISST_EXPORT mtsMailBox
{
    mtsQueue<mtsCommandBase *> CommandQueue;
    std::string Name;  // for debugging output

public:
    inline mtsMailBox(const std::string & name, unsigned int size):
        CommandQueue(size, 0),
        Name(name)
    {}


    inline ~mtsMailBox(void) {}

    
    inline const std::string & GetName(void) const {
        return Name;
    }


    inline bool Write(mtsCommandBase * command) {
        return (CommandQueue.Put(command) != 0);
    }

    
    bool ExecuteNext(void);
};


#endif // _mtsMailbox_h

