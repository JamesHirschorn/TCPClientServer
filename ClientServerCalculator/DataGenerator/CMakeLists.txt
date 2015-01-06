file(GLOB SOURCES "*.h" "*.hpp" "*.cpp")

source_group("Source Files" FILES ${SOURCES})

add_executable(DataGenerator ${SOURCES})

target_link_libraries(DataGenerator)

set_target_properties(DataGenerator PROPERTIES 
	FOLDER "ClientServerCalculator")

install(TARGETS DataGenerator DESTINATION ${INSTALL_SUBDIR})