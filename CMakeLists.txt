cmake_minimum_required(VERSION 3.24)

# Read project metadata
file(READ "${CMAKE_SOURCE_DIR}/.private/project.json" projectInfo)
string(JSON projectName GET "${projectInfo}" exeName)
string(JSON projectVersion GET "${projectInfo}" version)
string(JSON projectVersionState GET "${projectInfo}" versionState)
string(JSON projectVersionSTD GET "${projectInfo}" versionSTD)

set(VERSION "${projectVersion} ${versionState}")
set(STD_VERSION "${projectVersionSTD}")

project(${projectName} VERSION ${projectVersion} LANGUAGES CXX)

# Use C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Enable policy for install RPATH handling (CMP0147)
cmake_policy(SET CMP0147 NEW)

# Directories
set(srcDir "${CMAKE_SOURCE_DIR}/src")
set(incDir "${CMAKE_SOURCE_DIR}/inc")
set(testsDir "${CMAKE_SOURCE_DIR}/tests")

# Project files
set(headers
    ${incDir}/macros.hpp
    ${incDir}/types.hpp
    ${incDir}/Argx.hpp
    ${incDir}/ARGXAddError.hpp
)

set(sources
    ${srcDir}/Argx.cpp
    ${srcDir}/ARGXAddError.cpp
)

# Build static library
add_library(${projectName}_static STATIC ${sources} ${headers})
target_include_directories(${projectName}_static
  PUBLIC
    $<BUILD_INTERFACE:${incDir}>
    $<INSTALL_INTERFACE:include>
)

# Build shared library
add_library(${projectName}_shared SHARED ${sources} ${headers})
target_include_directories(${projectName}_shared
  PUBLIC
    $<BUILD_INTERFACE:${incDir}>
    $<INSTALL_INTERFACE:include>
)

# Build executable linking to shared library
add_executable(${projectName} ${testsDir}/main.cpp)
target_link_libraries(${projectName} PRIVATE ${projectName}_shared)
target_include_directories(${projectName}
  PRIVATE
    ${incDir}
)

# Compiler and linker flags per target
foreach(tgt IN ITEMS ${projectName}_static ${projectName}_shared ${projectName})
  target_compile_options(${tgt} PRIVATE
    $<$<CONFIG:DEBUG>:-g;-Og;-Wall;-Wextra;-Wpedantic;-fsanitize=address>
    $<$<CONFIG:RELEASE>:-O3;-march=native;-DNDEBUG>
  )
  target_link_options(${tgt} PRIVATE
    $<$<CONFIG:DEBUG>:-fsanitize=address>
  )
endforeach()

# Output directory for binaries
set_target_properties(${projectName} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install rules
install(TARGETS ${projectName}_static ${projectName}_shared ${projectName}
  EXPORT ${projectName}Targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY ${incDir}/ DESTINATION include)

# Generate and install export config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
  INSTALL_DESTINATION lib/cmake/${projectName}
)

install(EXPORT ${projectName}Targets
  FILE ${projectName}Targets.cmake
  NAMESPACE ${projectName}::
  DESTINATION lib/cmake/${projectName}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
  DESTINATION lib/cmake/${projectName}
)

