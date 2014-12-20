
source_group("Source Files" FILES ServerAction.hpp)

add_executable(Calculator-TCPServer Calculator-TCPServer.cpp ServerAction.hpp)

#target_link_libraries(Calculator-TCPServer ${Boost_LIBRARIES})

set_target_properties(Calculator-TCPServer PROPERTIES FOLDER "ClientServerCalculator")

install(TARGETS Calculator-TCPServer DESTINATION ${INSTALL_SUBDIR})