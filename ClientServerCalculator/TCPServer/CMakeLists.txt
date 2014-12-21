file(GLOB SOURCES "*.h" "*.hpp" "*.cpp")

source_group("Source Files" FILES ${SOURCES})

add_executable(Calculator-TCPServer ${SOURCES})

#target_link_libraries(Calculator-TCPServer ${Boost_LIBRARIES})

set_target_properties(Calculator-TCPServer PROPERTIES FOLDER "ClientServerCalculator")

install(TARGETS Calculator-TCPServer DESTINATION ${INSTALL_SUBDIR})