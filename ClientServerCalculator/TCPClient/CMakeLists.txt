file(GLOB SOURCES "*.h" "*.hpp" "*.cpp")

source_group("Source Files" FILES ${SOURCES} ../Shared/Data.h)

add_executable(TCPClient ${SOURCES} ../Shared/Data.h)

#target_link_libraries(TCPClient ${Boost_LIBRARIES})

set_target_properties(TCPClient PROPERTIES FOLDER "ClientServerCalculator")