cmake_minimum_required(VERSION 2.8)
project(JFWebServer)

OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)

if(ENABLE_GCOV AND NOT WIN32 AND NOT APPLE)
    set(GCOV_PREFIX ${PROJECT_BINARY_DIR}/bin)
    set(GCOV_PERFIX_STPIP 1)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION on)
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(SUB_DIR_LIST "Server" "Util")

set(INSTALL_PATH_LIB lib)
set(INSTALL_PATH_INCLUDE include)

#使能GOP缓存
add_definitions(-DENABLE_RING_USEBUF)

find_package(Boost REQUIRED COMPONENTS thread date_time timer)
if (Boost_FOUND)
    message(STATUS "found Boost library\"${Boost_INCLUDE_DIRS}\"")
    include_directories(${Boost_INCLUDE_DIRS})
    list(APPEND LINK_LIB_LIST ${Boost_LIBRARIES})
endif (Boost_FOUND)


find_package(Poco REQUIRED Foundation Net JSON Util)
if (Poco_FOUND)
    message(STATUS "found Poco library\"${Poco_INCLUDE_DIRS}\"")
    include_directories(${Poco_INCLUDE_DIRS})
    list(APPEND LINK_LIB_LIST ${Poco_LIBRARIES})
endif (Poco_FOUND)

foreach(SUB_DIR ${SUB_DIR_LIST})
    #遍历文件
    aux_source_directory(src/${SUB_DIR} SRC_LIST)
    #安装头文件至系统目录
    install(DIRECTORY src/${SUB_DIR} DESTINATION ${INSTALL_PATH_INCLUDE} FILES_MATCHING PATTERN "*.h")
endforeach()


message(STATUS "链接依赖库：${LINK_LIB_LIST}")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_compile_options(-std=c++11)

#编译动态库
add_library(${CMAKE_PROJECT_NAME}_shared SHARED ${SRC_LIST})
target_link_libraries(${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST})
set_target_properties(${CMAKE_PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}")
install(TARGETS ${CMAKE_PROJECT_NAME}_shared  ARCHIVE DESTINATION ${INSTALL_PATH_LIB} LIBRARY DESTINATION ${INSTALL_PATH_LIB})

add_subdirectory(tests)
