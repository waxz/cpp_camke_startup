cmake_minimum_required(VERSION 2.8.3)
project(example)


# ### NOTE: *** Adjust version.txt when required ***
# read 'version' file into a variable (stripping any newlines or spaces)
file(READ version.txt versionFile)
if (NOT versionFile)
    message(FATAL_ERROR "Unable to determine version. version.txt file is missing.")
endif()
string(STRIP "${versionFile}" TEST_VERSION)
string(REPLACE "." ";" VERSION_LIST ${TEST_VERSION})
list(GET VERSION_LIST 0 PROJECT_VER_MAJOR)
list(GET VERSION_LIST 1 PROJECT_VER_MINOR)
list(GET VERSION_LIST 2 PROJECT_VER_POINT)
set( PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_POINT}" )


# add cpack config file
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)



# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")


# C++ compile flags
# -std=c++11 -> for c++ standarts
# -Wall for WARNING
# -pedantic -> for warning between c and c++
# -pthread for portability
# -g -O0 -fprofile-arcs -ftest-coverage for code coverage
if(NOT CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage")
# example how to set c++ compiler flags for GNU
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()


# place binaries and libraries according to GNU standards
#include(GNUInstallDirs)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/install/${CMAKE_INSTALL_LIBDIR})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/install/${CMAKE_INSTALL_LIBDIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/install/${CMAKE_INSTALL_BINDIR})
#
#message(CMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
#message(CMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#message(CMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
###############################################################################
## dependencies ###############################################################
###############################################################################

# this defines the variables Boost_LIBRARIES that contain all library names
# that we need to link to


find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)
set(Boost_USE_STATIC_LIBS        OFF) # enable dynamic linking
set(Boost_USE_MULTITHREADED      ON) # enable multithreading
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ALL_DYN_LINK ON)
if(Boost_FOUND)
    message ("boost found" ${Boost_LIBRARY_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()




#################################################
##### LIBRARY #####
#################################################
include_directories(include)
add_library( ${PROJECT_NAME}_lib  src/library.cpp)
set_target_properties(${PROJECT_NAME}_lib PROPERTIES COMPILE_FLAGS "-DBUILDING_UTIL_LIB")
set_target_properties(${PROJECT_NAME}_lib PROPERTIES
        VERSION   ${PROJECT_VER}
        SOVERSION ${PROJECT_VER_MAJOR}
        )
install(TARGETS ${PROJECT_NAME}_lib
        EXPORT ${PROJECT_NAME}_lib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION lib/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h"
        )

#################################################
##### EXECUTABLE #####
#################################################

add_executable(${PROJECT_NAME} src/main.cpp)
# just for example add some compiler flags
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++11 -Wall -Wfloat-conversion)


target_link_libraries(${PROJECT_NAME} PUBLIC
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}_lib
        # here you can add any library dependencies
        )

install (TARGETS ${PROJECT_NAME}
        DESTINATION bin)

###############################################################################
## packaging ##################################################################
###############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.



#install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install(
#        TARGETS ${PROJECT_NAME}
#        ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
#        LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#        RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#)
#install(
#        TARGETS ${PROJECT_NAME}_lib
#        ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
#        LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#        RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#)
#
#install(DIRECTORY include/${PROJECT_NAME}/
#        DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
#        FILES_MATCHING PATTERN "*.h"
#        )
# This is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package.
install(DIRECTORY resources DESTINATION share/${PROJECT_NAME})




############################################################
# Create DEB
# in shell run  "make package"
############################################################


include(CpackConfig)