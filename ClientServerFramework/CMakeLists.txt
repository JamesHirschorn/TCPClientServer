# Collect sources into the variable MATH_SOURCES without
# having to explicitly list each header and source file.
#
# CMake documentation states "We do not recommend using GLOB to collect a list of
# source files from your source tree. If no CMakeLists.txt file changes when a 
# source is added or removed then the generated build system cannot know when to 
# ask CMake to regenerate".
file(GLOB SOURCES "*.cpp" "*.hpp")
file(GLOB CLIENT_SOURCES "Client/*.h" "Client/*.hpp" "Client/*.cpp")
#file(GLOB CLIENTSERVER_SOURCES "CleintServer/*.hpp")
file(GLOB SHARED_SOURCES "Shared/*.hpp")
file(GLOB SHARED_MESSAGING_SOURCES "Shared/Messaging/*.h" "Shared/Messaging/*.hpp")
file(GLOB SHARED_DESIGNPATTERNS_SOURCES "Shared/DesignPatterns/*.hpp")

set(ALL_SOURCES ${SOURCES};${CLIENT_SOURCES};${CLIENTSERVER_SOURCES};${SHARED_SOURCES};${SHARED_MESSAGING_SOURCES};${SHARED_DESIGNPATTERNS_SOURCES})
#message("Sources: " ${ALL_SOURCES})

# Collect sources into the variable SIMPLE_FUNCTION_SOURCES
#file (GLOB SIMPLE_FUNCTION_SOURCES
#      "simple/*.h",
#      "simple/*.cxx")

# The recommended way to collect sources in variable 
# ADVANCED_FUNCTION_SOURCES by explicitly specifying the source files
#set  (ADVANCED_FUNCTION_SOURCES
#      "advanced/AdvancedFunctions.h"
#      "advanced/AdvancedFunctions.cxx")

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
set(BASE_DIR "ClientServerFramework")
source_group(${BASE_DIR} FILES ${SOURCES})
source_group("${BASE_DIR}\\Client" FILES ${CLIENT_SOURCES})
#source_group("${BASE_DIR}\\ClientServer" FILES ${CLIENTSERVER_SOURCES})
source_group("${BASE_DIR}\\Shared" FILES ${SHARED_SOURCES})
source_group("${BASE_DIR}\\Shared\\Messaging" FILES ${SHARED_MESSAGING_SOURCES})
source_group("${BASE_DIR}\\Shared\\DesignPatterns" FILES ${SHARED_DESIGNPATTERNS_SOURCES})

# Properties->C/C++->General->Additional Include Directories
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set Properties->General->Configuration Type to Dynamic Library(.dll)
# Creates math.dll with the listed sources collected in the variables
# Also adds sources to the Solution Explorer
#add_library(math SHARED ${MATH_SOURCES} 
#                        ${SIMPLE_FUNCTION_SOURCES} 
#                        ${ADVANCED_FUNCTION_SOURCES})
#add_library(ClientServerFramework INTERFACE)
#target_include_directories(ClientServerFramework INTERFACE
#  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_library(ClientServerFramework ${ALL_SOURCES})

# Creates folder "libraries" and adds target project (math.vcproj)
#set_property(TARGET math PROPERTY FOLDER "libraries")
#set_property(TARGET ClientServerFramework)

# Adds logic to INSTALL.vcproj to copy math.dll to destination directory
#install (TARGETS math
#         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)