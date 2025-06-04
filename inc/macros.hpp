#pragma once

// Versioning
#define ARGX_VERSION_MAJOR            0
#define ARGX_VERSION_MINOR            1
#define ARGX_VERSION_PATCH            2

#define ARGX_VERSION_STD              20250604 // Once released, it will be YyyyMmDd (eg: 20250604)

// Version states:
// * dev
// * beta
// * snap
// * build (AKA: Release)
#define ARGX_VERSION_STATE          "beta"

#define ARGX_VERSION                ((ARGX_VERSION_MAJOR<<16)|(ARGX_VERSION_MINOR<<8)|(ARGX_VERSION_PATCH)|(ARGX_VERSION_STATE << 24))

#define ARGX_VERSION_CHECK(ARGX_VERSION_MAJOR, ARGX_VERSION_MINOR, ARGX_VERSION_PATCH, ARGX_VERSION_STATE) \
    (((ARGX_VERSION_MAJOR)<<16)|((ARGX_VERSION_MINOR)<<8)|(ARGX_VERSION_PATCH)|((ARGX_VERSION_STATE) << 24))

// DEV or PROD
#define ARGX_DEV true

// Macro utils
#define ARGX_STRINGIFY(x) #x
#define ARGX_TOSTRING(x) ARGX_STRINGIFY(x)

