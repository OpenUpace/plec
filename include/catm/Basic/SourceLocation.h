#ifndef CATMC_SOURCE_LOCATION_H
#define CATMC_SOURCE_LOCATION_H

#include <stdint.h>

struct SourceLocation {
    uint32_t line;
    uint32_t column;
    uint32_t offset;
};

#endif
