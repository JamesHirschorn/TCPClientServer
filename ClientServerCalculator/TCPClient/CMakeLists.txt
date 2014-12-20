file(GLOB SOURCES "*.h" "*.hpp" "*.cpp")

source_group("Source Files" FILES ${SOURCES} ../Shared/Data.h)

add_executable(Calculator-TCPClient ${SOURCES} ../Shared/Data.h)

#target_link_libraries(Calculator-TCPClient ${Boost_LIBRARIES})

set_target_properties(Calculator-TCPClient PROPERTIES FOLDER "ClientServerCalculator")

install(TARGETS Calculator-TCPClient DESTINATION ${INSTALL_SUBDIR})