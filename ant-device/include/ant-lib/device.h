#pragma once

#include <stdint.h>

class DeviceControl
{
    public:
        int init();
        int writeField(uint8_t fieldId, const uint8_t* data, uint16_t size);
        int readField(uint8_t fieldId, uint8_t* buffer, uint16_t lenght);
};