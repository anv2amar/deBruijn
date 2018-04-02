cmake_minimum_required(VERSION 3.6)
project(cppCode)

set(CMAKE_CXX_STANDARD 11)
if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                googletest
                 PREFIX              lib
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

#add_subdirectory(lib/googletest-master)
include_directories(${googletest_SOURCE_DIR}/include)
include_directories(${googletest_BINARY_DIR}/include)


##install boost

find_package(Boost COMPONENTS filesystem program_options REQUIRED)
#IF (Boost_FOUND)
#find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

#endif()
#add_executable(BoostTest main.cpp)
#target_link_libraries(BoostTest ${Boost_LIBRARIES})
#set(BOOST_REQUESTED_VERSION 1.61)
##list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
#include(FindBoost.cmake)
#find_package(Boost)

#target_include_directories(cppCode PRIVATE ${Boost_INCLUDE_DIRS})
set(SOURCE_FILES main.cpp src/RabinKarpHash.cpp src/RabinKarpHash.h tests/RabinKarpTest.cpp src/ConstructKMers.h tests/ConstructKMersTest.cpp tests/BBHashTest.cpp src/BBHash.h src/GraphDS.cpp src/GraphDS.h tests/GraphDSTest.cpp src/PreProcess.h tests/PreProcessTest.cpp)
add_executable(cppCode ${SOURCE_FILES})
target_link_libraries(cppCode gtest gtest_main ${Boost_LIBRARIES})
