cmake_minimum_required( VERSION 3.16 )

project( AoC )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

add_executable( day14 day14/main.cpp )
target_compile_features( day14 PRIVATE cxx_std_20 )
