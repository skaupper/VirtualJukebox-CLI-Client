cmake_minimum_required(VERSION 3.16)

project(VirtualJukebox-CLI-Client CXX)

# Include all CMake scripts
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/Cache.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Conan.cmake)

# Add interface libraries
add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)
add_library(lib_options INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_17)
enable_sanitizers(project_options)
set_project_warnings(project_warnings)

target_compile_features(lib_options INTERFACE cxx_std_17)

# Deactivate the ABI-change warning on ARM
target_compile_options(project_warnings INTERFACE "-Wno-psabi")

# Configure and run conan
set(CONAN_EXTRA_REQUIRES tl-optional/1.0.0 nlohmann_json/3.8.0)
set(CONAN_EXTRA_OPTIONS)

run_conan()

# Compile targets
add_subdirectory(lib)
add_subdirectory(src)
