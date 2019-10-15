aux_source_directory(. TEST_SRC_LIST)

set(MODULE test_poco)

foreach(TEST_SRC ${TEST_SRC_LIST})
	string(REGEX REPLACE "^\\./|\\.c[a-zA-Z0-9_]*$" "" TEST_EXE_NAME ${TEST_SRC})
	message(STATUS "add tests: ${TEST_EXE_NAME}")
        add_executable(${TEST_EXE_NAME} ${TEST_SRC})

	if(WIN32)
            target_link_libraries(${TEXT_EXE_NAME} ${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST})
	else()
            target_link_libraries(${TEST_EXE_NAME} ${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST} pthread)
        endif()
endforeach(TEST_SRC ${TEST_SRC_LIST})


file(GLOB SourceFiles source/*)

foreach(SourceFile ${SourceFiles})
   add_custom_command(TARGET ${MODULE} PRE_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy ${SourceFile} $<TARGET_FILE_DIR:${MODULE}>)
endforeach()
