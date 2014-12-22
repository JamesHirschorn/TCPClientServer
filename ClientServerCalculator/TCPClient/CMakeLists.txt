file(GLOB SOURCES "*.h" "*.hpp" "*.cpp")

source_group("Source Files" FILES ${SOURCES})

add_executable(Calculator-TCPClient ${SOURCES})

target_link_libraries(Calculator-TCPClient ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

set_target_properties(Calculator-TCPClient PROPERTIES 
	FOLDER "ClientServerCalculator")

install(TARGETS Calculator-TCPClient DESTINATION ${INSTALL_SUBDIR})