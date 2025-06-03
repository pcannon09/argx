cmake_minimum_required(VERSION 3.24)

# Read metadata
file(READ ".private/project.json" projectInfo)
string(JSON projectName GET "${projectInfo}" exeName)
string(JSON projectVersion GET "${projectInfo}" version)
string(JSON projectVersionState GET "${projectInfo}" versionState)
string(JSON projectVersionSTD GET "${projectInfo}" versionSTD)

set(VERSION "${projectVersion} ${versionState}")
set(STD_VERSION "${projectVersionSTD}")

project(${projectName} VERSION ${projectVersion})

# Compiler flags
set(programFlags
	-fsanitize=address
    -g
    -Wunused
    -Wall
)

set(programLinkFlags
	-fsanitize=address
    -g
)

set(CPP_STD 14)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_STANDARD ${CPP_STD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Link exec if only changed
cmake_policy(SET CMP0147 NEW)

# Show build config info
message(WARNING "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS  "${projectName} version: ${VERSION}")
message(STATUS  "Standard version: ${STD_VERSION}")

# Directory setup
set(srcDir "${CMAKE_SOURCE_DIR}/src")
set(testsDir "${CMAKE_SOURCE_DIR}/tests")
set(incDir "${CMAKE_SOURCE_DIR}/inc")
set(vendorDir "${CMAKE_SOURCE_DIR}/vendor")

# Project files
set(headers
    ${incDir}/macros.hpp
    ${incDir}/types.hpp

    ${incDir}/Argx.hpp
    ${incDir}/ARGXAddError.hpp
)

set(sources
    ${testsDir}/main.cpp

    ${srcDir}/Argx.cpp
    ${srcDir}/ARGXAddError.cpp
)

# Build the main executable
add_executable(${projectName}
    ${sources}
    ${headers}
)

target_compile_options(${projectName} PRIVATE ${programFlags})
target_link_options(${projectName} PRIVATE ${programLinkFlags})

set_target_properties(${projectName} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

