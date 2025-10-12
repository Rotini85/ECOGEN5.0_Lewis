cmake_minimum_required(VERSION 3.10)

set (CMAKE_RELEASE_VERSION_MAJOR "5" )
set (CMAKE_RELEASE_VERSION_MINOR "0" )
set (CMAKE_RELEASE_VERSION_PATCH "0" )
set (CMAKE_RELEASE_DATE "2025-09-30" )

# Set the project name and version
project (ECOGEN
        LANGUAGES CXX
        VERSION "${CMAKE_RELEASE_VERSION_MAJOR}.${CMAKE_RELEASE_VERSION_MINOR}.${CMAKE_RELEASE_VERSION_PATCH}")


# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add custom compilation type
set(CMAKE_CXX_FLAGS_COVERAGE "-O3 -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the CXX compiler during Coverage builds."
    FORCE)
set(CMAKE_C_FLAGS_COVERAGE "-O3 -fprofile-arcs -ftest-coverage" CACHE STRING
    "Flags used by the C compi ler during Coverage builds."
    FORCE)

set(CMAKE_CXX_FLAGS_PROFILE "-O3 -pg" CACHE STRING
    "Flags used by the CXX compiler during Profile builds."
    FORCE)
set(CMAKE_C_FLAGS_PROFILE "-O3 -pg" CACHE STRING
    "Flags used by the C compi ler during Profile builds."
    FORCE)

# List source files
set(ECOGEN_source_dir ${PROJECT_SOURCE_DIR}/src)
FILE(
  GLOB_RECURSE
  ECOGEN_source_files
  ${ECOGEN_source_dir}/*.cpp
  )

# Search for MPI distrib
FIND_PACKAGE( MPI REQUIRED )

# Add the executable
add_executable(ECOGEN ${ECOGEN_source_files})
target_link_libraries(ECOGEN MPI::MPI_CXX)
