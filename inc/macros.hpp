#pragma once

/*
 * NOTE: 
 * Macro `ARGX_AS_PYTHON_PACKAGE` is for the Argx python module, DO NOT enable it if using C++
 */

// Versioning
#define ARGX_VERSION_MAJOR            1
#define ARGX_VERSION_MINOR            1
#define ARGX_VERSION_PATCH            0

#define ARGX_VERSION_STD              20250721

// Version states:
// * dev                    0
// * beta                   1
// * snap                   2
// * build (AKA: Release)   3
#ifndef ARGX_AS_PYTHON_PACKAGE
#   define ARGX_VERSION_STATE           "build"
#else
#   define ARGX_VERSION_STATE           3
#endif

#define ARGX_VERSION                ((ARGX_VERSION_MAJOR<<16)|(ARGX_VERSION_MINOR<<8)|(ARGX_VERSION_PATCH)|(ARGX_VERSION_STATE << 24))

#define ARGX_VERSION_CHECK(ARGX_VERSION_MAJOR, ARGX_VERSION_MINOR, ARGX_VERSION_PATCH, ARGX_VERSION_STATE) \
    (((ARGX_VERSION_MAJOR)<<16)|((ARGX_VERSION_MINOR)<<8)|(ARGX_VERSION_PATCH)|((ARGX_VERSION_STATE) << 24))

// DEV or PROD
#define ARGX_DEV false

// Macro utils
#define ARGX_STRINGIFY(x) #x
#define ARGX_TOSTRING(x) ARGX_STRINGIFY(x)

