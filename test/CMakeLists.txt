
cmake_minimum_required(VERSION 3.6.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
   "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
  add_compile_options(-Qunused-arguments)
  add_compile_options(-Wno-deprecated-declarations)
  add_compile_options(-Wno-unknown-argument)
endif()

hunter_add_package(Catch)

find_package(Catch CONFIG REQUIRED)

set(SOURCES main.cpp
            builder.cpp
            map.cpp
            iterator.cpp)

add_executable(tree_test ${SOURCES})
target_link_libraries(tree_test PUBLIC Catch::Catch cpptrees)

add_test(NAME tree_test COMMAND tree_test)

#function(gen_test name)
#  add_executable(${name} ${name}.cpp)
#  #set_target_properties(${name} PROPERTIES COMPILE_FLAGS "${cxx_strict}")
#  target_link_libraries(${name} cpptrees)
#  add_test(${name} ${name})
#endfunction(gen_test)
#
#gen_test(builder)
#gen_test(map)
#gen_test(iterator)
