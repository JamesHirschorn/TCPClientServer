
source_group("Source Files" FILES Action.hpp)

add_executable(TCPServer TCPServer.cpp Action.hpp)

#target_link_libraries(TCPServer ${Boost_LIBRARIES})

set_target_properties(TCPServer PROPERTIES FOLDER "ClientServerCalculator")