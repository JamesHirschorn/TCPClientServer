#################################################
# CMake build system for TCPClientServer.       #
#                                               #
# (C) 2014 James Hirschorn                      #
#################################################

cmake_minimum_required(VERSION 2.8.9)

project(TCPClientServer)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#----------------------------
# PREPROCESSOR DEFINITIONS
#----------------------------

# Set the WIN32_WINNT preprocessor flag, if necessary.
macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

        set(${version} "0x${ver}")
    endif()
endmacro()

if(WIN32)
  get_WIN32_WINNT(ver)
  add_definitions(-D_WIN32_WINNT=${ver} -D_SCL_SECURE_NO_WARNINGS) 
endif(WIN32)

# Set compiler flags and options. 
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(UNIX)
#message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

#----------------------------
# LIBRARIES
#----------------------------

# For some reason, in Windows CMake fails when the specific Boost libraries are required.
# We use link_directories as a work around (see below).
#if(WIN32)
#  find_package(Boost 1.57 REQUIRED)
#else(WIN32)
  find_package(Boost 1.57 REQUIRED system thread iostreams serialization date_time)
#endif(WIN32)

if(NOT ${Boost_FOUND})
    set(BOOST_ROOT $ENV{Boost_DIR} CACHE PATH "Boost library root directory")
endif(NOT ${Boost_FOUND})

find_package(OpenSSL)
if(NOT ${OPENSSL_FOUND})
	set(OPENSSL_ROOT $ENV{OPENSSL_DIR} CACHE PATH "OpenSSL library root directory")
endif(NOT ${OPENSSL_FOUND})

#----------------------------
# INCLUDES
#----------------------------

# Include the root source dirctory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
if(${Boost_FOUND})
	include_directories(${Boost_INCLUDE_DIRS})
	# Work around.
	link_directories(${Boost_LIBRARY_DIRS})
endif(${Boost_FOUND})
if(${OPENSSL_FOUND})
	include_directories(${OPENSSL_INCLUDE_DIR})
endif(${OPENSSL_FOUND})

#----------------------------
# INSTALLATION
#----------------------------

# Executables will be installed in the bin subdirectory of the root directory.
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
set(INSTALL_SUBDIR "bin")
# Install the certificate authority file(s).
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ssl DESTINATION ${INSTALL_SUBDIR})
# Make the install directory accessible from the source code.
add_definitions(-DINSTALL_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${INSTALL_SUBDIR}")

#----------------------------
# Sub-directories
#----------------------------

add_subdirectory(ClientServerFramework)
add_subdirectory(ClientServerCalculator)
