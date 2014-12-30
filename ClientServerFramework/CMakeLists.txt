# Collect the sources (including header files) into per-folder variables.
file(GLOB SOURCES "*.cpp" "*.hpp")
file(GLOB CLIENT_SOURCES "Client/*.h" "Client/*.hpp" "Client/*.cpp")
file(GLOB SERVER_SOURCES "Server/*.hpp" "Server/*.h")
#file(GLOB CLIENTSERVER_SOURCES "CleintServer/*.hpp")
file(GLOB SHARED_SOURCES "Shared/*.hpp")
file(GLOB SHARED_CONNECTION_SOURCES "Shared/Connection/*.hpp")
file(GLOB SHARED_CONTAINER_SOURCES "Shared/Container/*.h" "Shared/Container/*.hpp")
file(GLOB SHARED_DESIGNPATTERNS_SOURCES "Shared/DesignPatterns/*.hpp")
file(GLOB SHARED_IO_SOURCES "Shared/IO/*.hpp")
file(GLOB SHARED_SERIALIZATION_SOURCES "Shared/Serialization/*.h" "Shared/Serialization/*.hpp")

set(ALL_SOURCES ${SOURCES};${CLIENT_SOURCES};${SERVER_SOURCES};
	${SHARED_SOURCES};${SHARED_CONNECTION_SOURCES};${SHARED_CONTAINER_SOURCES};
	${SHARED_DESIGNPATTERNS_SOURCES};${SHARED_IO_SOURCES};
	${SHARED_SERIALIZATION_SOURCES})

# Set up folders in Visual Studio.
set(BASE_DIR "Source Files")
source_group(${BASE_DIR} FILES ${SOURCES})
source_group("${BASE_DIR}\\Client" FILES ${CLIENT_SOURCES})
source_group("${BASE_DIR}\\Server" FILES ${SERVER_SOURCES})
source_group("${BASE_DIR}\\Shared" FILES ${SHARED_SOURCES})
source_group("${BASE_DIR}\\Shared\\Container" FILES ${SHARED_CONTAINER_SOURCES})
source_group("${BASE_DIR}\\Shared\\Connection" FILES ${SHARED_CONNECTION_SOURCES})
source_group("${BASE_DIR}\\Shared\\DesignPatterns" FILES ${SHARED_DESIGNPATTERNS_SOURCES})
source_group("${BASE_DIR}\\Shared\\IO" FILES ${SHARED_IO_SOURCES})
source_group("${BASE_DIR}\\Shared\\Serialization" FILES ${SHARED_SERIALIZATION_SOURCES})

add_library(ClientServerFramework ${ALL_SOURCES})
