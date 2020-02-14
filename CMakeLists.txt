cmake_minimum_required(VERSION 3.10.2)

project(VirtualJukebox-CLI-Client)

cmake_policy(SET CMP0054 NEW)

################################################################################
# Set default values for input parameter
################################################################################

# Build debug version by default, unless specified otherwise
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

################################################################################
# Compiler settings
################################################################################

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(SRC_COMPILER_OPTIONS
        -Wall
        -Wextra)

################################################################################
# Include other CMake files
################################################################################

include(cmake/ClangFormat.cmake)
include(cmake/LibRestClient-cpp.cmake)

################################################################################
# Find packages with built-in support
################################################################################

# find_package(Threads)

################################################################################
# Source files
################################################################################

# Application and tests
#-------------------------------------------------------------------------------

# Source file containing the main entrypoint
set(ENTRYPOINT_SOURCE   src/main.cpp)
# All source file (excluding main) for the application
set(APP_SOURCES         src/api/api-v1.cpp)

set(APP_HEADER          src/api/api-v1.h)

# Libraries and include directories of dependencies used by the application
set(APP_LIBRARIES       ${LIBRESTCLIENT_LIBRARIES}
                        # ${CMAKE_THREAD_LIBS_INIT}
                        )
set(APP_INCLUDE_DIRS    src/
                        lib/
                        ${LIBRESTCLIENT_INCLUDE_DIRS})

# Specify sources for formatting
set(FORMATTING_SOURCES ${ENTRYPOINT_SOURCE} ${APP_SOURCES} ${APP_HEADER} ${TEST_SOURCES} ${TEST_HEADER})

################################################################################
# Targets
################################################################################

# Compile the sources of the main application into an OBJECT library
#-------------------------------------------------------------------------------
set(APP_OBJECTS app_object_library)
add_library(${APP_OBJECTS} OBJECT ${APP_SOURCES} ${APP_HEADER})
target_compile_options(${APP_OBJECTS} PRIVATE ${SRC_COMPILER_OPTIONS})


# Main program executable
#-------------------------------------------------------------------------------
include_directories(${APP_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${ENTRYPOINT_SOURCE} $<TARGET_OBJECTS:${APP_OBJECTS}>)
target_link_libraries(${PROJECT_NAME} ${APP_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${SRC_COMPILER_OPTIONS})

# Other
#-------------------------------------------------------------------------------
clang_format_add_target(${FORMATTING_SOURCES})
