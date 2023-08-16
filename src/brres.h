#pragma once

#include <unpac/types.h>

class Brres {
public:
    struct Header {
        char magic[4];
        u16 bom;
        u16 pad;
        u32 size;
        u16 rootOff;
        u16 sectionCount;
    };

    struct Entry {
        u16 id;
        u16 unused;
        u16 left;
        u16 right;
        u32 nameOff;
        u32 dataOff;
    };

    struct Group {
        u32 size;
        u32 entryCount;
        
    };

    struct Root {
        char magic[4];
        u32 size;
        struct Group group;
    };
};
