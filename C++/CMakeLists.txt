cmake_minimum_required (VERSION 3.0)

project(RootsToolC)

file(GLOB Roots_src
"*.h"
"*.cpp"
)

add_subdirectory(SupportLibs)


set(JSON_CPP_LIB ${CMAKE_CURRENT_BINARY_DIR}/SupportLibs/src/lib_json/Debug/jsoncpp.lib CACHE STRING "JSON CPP lib" FORCE)
set(JSON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/SupportLibs/jsoncpp/include;${CMAKE_CURRENT_SOURCE_DIR}/SupportLibs/jsoncpp/src CACHE STRING "JSON cpp src dirs" FORCE)
message(${JSON_CPP_LIB})
link_libraries(${JSON_CPP_LIB})

#message(${CMAKE_CURRENT_BINARY_DIR}/SupportLibs/CMakeFiles/Export/lib/cmake/jsoncpp)
add_library(RootsTool SHARED ${Roots_src})
target_include_directories(RootsTool PUBLIC ${JSON_INCLUDES})
target_link_libraries(RootsTool ${JSON_CPP_LIB})
add_executable(RootsEXE ${Roots_src})
target_include_directories(RootsEXE PUBLIC ${JSON_INCLUDES})
target_link_libraries(RootsEXE ${JSON_CPP_LIB})