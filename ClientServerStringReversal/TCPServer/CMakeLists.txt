
add_executable(TCPServer TCPServer.cpp)

#target_link_libraries(TCPServer ${Boost_LIBRARIES})

set_target_properties(TCPServer PROPERTIES FOLDER "ClientServerStringReversal")