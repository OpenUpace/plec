#ifndef CATMC_SOURCE_LOCATION_H
#define CATMC_SOURCE_LOCATION_H

#include <stdint.h>

struct SourceLocation
{
    uint64_t line;
    uint64_t column;
    uint64_t offset;
};

#endif