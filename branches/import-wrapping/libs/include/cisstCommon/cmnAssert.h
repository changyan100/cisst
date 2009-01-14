/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: cmnAssert.h 20 2009-01-08 16:09:57Z adeguet1 $

  Author(s):  Ankur Kapoor
  Created on: 2003-06-25

  (C) Copyright 2003-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


/*!
  \file
  \brief Assert macros definitions.
*/

#ifndef _cmnAssert_h
#define _cmnAssert_h


#include <cisstCommon/cmnPortability.h>
#include <cisstCommon/cmnThrow.h>

#include <assert.h>
#include <cstdlib>
#include <sstream>

#include <cisstCommon/cmnLogger.h>


#ifndef DOXYGEN

// No __FUNCTION__ for g++ version < 2.6 __FUNCDNAME__ Valid only
// within a function and returns the decorated name of the enclosing
// function (as a string). __FUNCDNAME__ is not expanded if you use
// the /EP or /P compiler option.  __FUNCSIG__ Valid only within a
// function and returns the signature of the enclosing function (as a
// string). __FUNCSIG__ is not expanded if you use the /EP or /P
// compiler option.  __FUNCTION__ Valid only within a function and
// returns the undecorated name of the enclosing function (as a
// string). __FUNCTION__ is not expanded if you use the /EP or /P
// compiler option.


// Visual C++
#ifdef CISST_COMPILER_IS_MSVC
    #ifdef __FUNCSIG__
        #define CMN_PRETTY_FUNCTION	__FUNCSIG__
    #else
        #warning "With Visual Studio, you need /EP or /P to have __FUNCSIG__"
    #endif

// GNU CC and Intel CC
#elif (CISST_COMPILER == CISST_GCC) || (CISST_COMPILER == CISST_INTEL_CC)
    #define CMN_PRETTY_FUNCTION __PRETTY_FUNCTION__

// Otherwise
#else
	#warning "Visual C++, GNU C++ and Intel CC are supported so far"
#endif

// Set a default value
#ifndef CMN_PRETTY_FUNCTION
    #define CMN_PRETTY_FUNCTION ""
#endif

#endif // DOXYGEN


/*!
  \ingroup cisstCommon

  Assert a condition.  This macro should be used whenever one needs to
  assert a condition.

  This macro has two main advantages over the system \c assert:

  \li The error message is log using #CMN_LOG (level of detail 1).

  \li CMN_ASSERT behavior can be modified using the defined variables
  CMN_ASSERT_DISABLED and CMN_ASSERT_THROWS_EXCEPTION.  The first
  variable allows to not compile the assertion.  It is similar to the
  \c NDEBUG for the standard \c assert .<br>The second variable allows
  to throw an exception (of type \c std::logic_error) instead of using
  the system \c abort.  This can be convenient if the libraries are
  wrapped for an interpreted language such as Python.  In this case,
  an \c abort() has the annoying effect of aborting the interpreter
  itself.<br>Both these variables can be modified using the CMake
  advanced mode.

  \note When compiling on linux make sure ulimit -c is unlimited!
  Otherwise, no core file will be generated with \c abort() .  By
  default, Redhat and Debian are set to prevent any core dump.

  \note On windows \c abort() uses stderr in console apps, message box
  API with OK button for release builds and message box API with
  "Abort, Ignore, Retry" for debug builds.

  \sa cmnThrow
*/
#ifdef CMN_ASSERT_DISABLED
    #define CMN_ASSERT(expr)
#else // CMN_ASSERT_DISABLED

#ifdef CMN_ASSERT_THROWS_EXCEPTION

#define CMN_ASSERT(expr) \
	if (!(expr)) { \
        std::stringstream messageBuffer; \
        messageBuffer << __FILE__ << ": Assertion '" << #expr \
			          << "' failed in: " << CMN_PRETTY_FUNCTION \
			          << ", line #" << __LINE__; \
		cmnThrow(std::logic_error(messageBuffer.str())); \
	}

#else // CMN_ASSERT_THROWS_EXCEPTION

#define CMN_ASSERT(expr) \
	if (!(expr)) { \
        std::stringstream messageBuffer; \
        messageBuffer << __FILE__ << ": Assertion '" << #expr \
			          << "' failed in: " << CMN_PRETTY_FUNCTION \
			          << ", line #" << __LINE__; \
        std::cerr << messageBuffer.str() << std::endl; \
		CMN_LOG(1) << messageBuffer.str() << std::endl; \
		abort(); \
	}

#endif // CMN_ASSERT_THROWS_EXCEPTION

#endif // CMN_ASSERT_DISABLED


#endif // _cmnAssert_h

