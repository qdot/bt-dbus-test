######################################################################################
# CMake directives
######################################################################################

#Require 2.6 or higher.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

INCLUDE( ${CMAKE_SOURCE_DIR}/BuildSysCMakeLib.cmake )

#setting link directory policy

IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######################################################################################
# Project declaration and options
######################################################################################

#Project declaration

PROJECT(BT-TEST)
INITIALIZE_BUILD()

######################################################################################
# Project specific package finding
######################################################################################

FIND_PACKAGE(DBus)
# FIND_PACKAGE(Glib)

######################################################################################
# Build function for liblightstone_test
######################################################################################

SET(SRCS raw.cpp)
INCLUDE_DIRECTORIES(${DBUS_INCLUDE_DIR} ${DBUS_INCLUDE_LIB_DIR})
BUILDSYS_BUILD_EXE(
  NAME bt-test
  SOURCES "${SRCS}" 
  CXX_FLAGS FALSE
  LINK_LIBS ${DBUS_LIBRARY} pthread
  LINK_FLAGS FALSE 
  SHOULD_INSTALL TRUE
  )

# SET(SRCS simple.cpp)
# BUILDSYS_BUILD_EXE(
#   NAME bt-simple
#   SOURCES "${SRCS}" 
#   CXX_FLAGS FALSE
#   LINK_LIBS "bluetooth"
#   LINK_FLAGS FALSE 
#   SHOULD_INSTALL TRUE
#   )
