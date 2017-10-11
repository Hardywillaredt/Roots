cmake_minimum_required (VERSION 3.4)

project(RootsToolC)

file(GLOB Roots_src
"*.h"
"*.cpp"
)

add_library(RootsTool SHARED ${Roots_src})
add_executable(RootsEXE ${Roots_src})