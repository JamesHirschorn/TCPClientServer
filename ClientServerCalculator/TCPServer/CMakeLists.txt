
source_group("Source Files" FILES Action.h)

add_executable(TCPServer TCPServer.cpp Action.h)

#target_link_libraries(TCPServer ${Boost_LIBRARIES})

set_target_properties(TCPServer PROPERTIES FOLDER "ClientServerStringReversal")