#
#
# (C) Copyright 2009-2011 Johns Hopkins University (JHU), All Rights
# Reserved.
#
# --- begin cisst license - do not edit ---
#
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
#
# --- end cisst license ---

find_package (cisst COMPONENTS cisstCommon)

if (cisst_FOUND_AS_REQUIRED)
  include (${CISST_USE_FILE})
  add_executable (cmnExGetChar
                  getChar.cpp)
  set_property (TARGET cmnExGetChar PROPERTY FOLDER "cisstCommon/examples")

  cisst_target_link_libraries (cmnExGetChar cisstCommon)
else (cisst_FOUND_AS_REQUIRED)
  message ("Information: code in ${CMAKE_CURRENT_SOURCE_DIR} will not be compiled, it requires cisstCommon")
endif (cisst_FOUND_AS_REQUIRED)
