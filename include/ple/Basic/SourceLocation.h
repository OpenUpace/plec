#ifndef PLEC_SOURCE_LOCATION_H
#define PLEC_SOURCE_LOCATION_H

#include <stdint.h>

struct SourceLocation {
    int line;
    int column;
    int offset;
};

#endif
