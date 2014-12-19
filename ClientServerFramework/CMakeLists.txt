# Collect the sources (including header files) into per-folder variables.
file(GLOB SOURCES "*.cpp" "*.hpp")
file(GLOB CLIENT_SOURCES "Client/*.h" "Client/*.hpp" "Client/*.cpp")
file(GLOB SERVER_SOURCES "Server/*.hpp" "Server/*.h")
#file(GLOB CLIENTSERVER_SOURCES "CleintServer/*.hpp")
file(GLOB SHARED_SOURCES "Shared/*.hpp")
file(GLOB SHARED_DESIGNPATTERNS_SOURCES "Shared/DesignPatterns/*.hpp")
file(GLOB SHARED_CONTAINER_SOURCES "Shared/Container/*.h" "Shared/Container/*.hpp")
file(GLOB SHARED_SERIALIZATION_SOURCES "Shared/Serialization/*.h" "Shared/Serialization/*.hpp")

#set(ALL_SOURCES ${SOURCES};${CLIENT_SOURCES};${SERVER_SOURCES};${SHARED_SOURCES};${SHARED_MESSAGING_SOURCES};${SHARED_DESIGNPATTERNS_SOURCES})
#message("Sources: " ${ALL_SOURCES})

# Set up folders in Visual Studio.
set(BASE_DIR "Source Files")
source_group(${BASE_DIR} FILES ${SOURCES})
source_group("${BASE_DIR}\\Client" FILES ${CLIENT_SOURCES})
source_group("${BASE_DIR}\\Server" FILES ${SERVER_SOURCES})
#source_group("${BASE_DIR}\\ClientServer" FILES ${CLIENTSERVER_SOURCES})
source_group("${BASE_DIR}\\Shared" FILES ${SHARED_SOURCES})
source_group("${BASE_DIR}\\Shared\\DesignPatterns" FILES ${SHARED_DESIGNPATTERNS_SOURCES})
source_group("${BASE_DIR}\\Shared\\Container" FILES ${SHARED_CONTAINER_SOURCES})
source_group("${BASE_DIR}\\Shared\\Serialization" FILES ${SHARED_SERIALIZATION_SOURCES})

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

add_library(ClientServerFramework ${SOURCES})

# Creates folder "libraries" and adds target project (math.vcproj)
#set_property(TARGET math PROPERTY FOLDER "libraries")
#set_property(TARGET ClientServerFramework)

# Adds logic to INSTALL.vcproj to copy math.dll to destination directory
#install (TARGETS math
#         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)