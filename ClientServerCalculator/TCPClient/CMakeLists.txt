
source_group("Source Files" FILES ../Shared/Data.h)

add_executable(TCPClient TCPClient.cpp ../Shared/Data.h)

#target_link_libraries(TCPClient ${Boost_LIBRARIES})

set_target_properties(TCPClient PROPERTIES FOLDER "ClientServerCalculator")