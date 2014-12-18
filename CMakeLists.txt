cmake_minimum_required (VERSION 2.6)

# Maps to Visual Studio solution file (Tutorial.sln)
# The solution will have all targets (exe, lib, dll) 
# as Visual Studio projects (.vcproj)
project(TCPClientServer)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set compiler flags and options. 
# Here it is setting the Visual Studio warning level to 4
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
#message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

find_package(Boost 1.57 REQUIRED)
if(NOT ${Boost_FOUND})
	set(BOOST_ROOT $ENV{Boost_DIR} CACHE PATH "Boost library root directory")
endif(NOT ${Boost_FOUND})

#message("CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
if(${Boost_FOUND})
	include_directories(${Boost_INCLUDE_DIRS})
endif(${Boost_FOUND})

# Set the following preprocessor flag, if necessary.
macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

        set(${version} "0x${ver}")
    endif()
endmacro()

get_WIN32_WINNT(ver)
add_definitions(-D_WIN32_WINNT=${ver})

# Sub-directories where more CMakeLists.txt exist
add_subdirectory(ClientServerFramework)
add_subdirectory(CalculatorTCPClient)
