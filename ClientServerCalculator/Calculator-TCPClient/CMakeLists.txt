

add_executable(Calculator-TCPClient CalculatorClient.cpp)
#target_link_libraries(Calculator-TCPClient ${Boost_LIBRARIES})

set_target_properties(Calculator-TCPClient PROPERTIES FOLDER "ClientServerCalculator")