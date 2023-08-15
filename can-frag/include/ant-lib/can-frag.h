#pragma once
#include <stdint.h>

class CanFrag
{
    public:
        int init();
        int deinit();

        int readPacket(uint8_t* buffer, uint32_t lenght);
        int pushFrame(const uint8_t* data, uint32_t size);

        int writePacket(const uint8_t* data, uint32_t size);
        int popFrame(uint8_t* buffer, uint32_t lenght);
};
