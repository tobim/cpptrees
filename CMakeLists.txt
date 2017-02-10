# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")

# If Hunter is cloned this line will not
# be used if fact. See 'testing' section for me info
HunterGate(
    URL "https://url/to/your/hunter-archive.tar.gz"
    SHA1 "put-archive-sha1-here"
)

project(cpptrees)

option(CPPTREES_ENABLE_TESTING "dont build tests" ON)

hunter_add_package(mpark_variant)

find_package(mpark_variant CONFIG REQUIRED)

add_library(cpptrees INTERFACE)
target_link_libraries(cpptrees INTERFACE mpark_variant)
target_include_directories(cpptrees INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(CPPTREES_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
