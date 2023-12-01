#ifndef CRYSTALOS__UTIL__SCREEN_H
#define CRYSTALOS__UTIL__SCREEN_H

#include <common/types.h>
#include <util/system.h>

namespace crystalos
{
    namespace util
    {
        void clearScreen();
        void print(char*);
        void printHex32(common::uint32_t);
        void printHex8(common::uint8_t);
    }
}

#endif