#pragma once
#include <stdint.h>

class ICan
{
    public:
        virtual int read(uint8_t *buffer, uint32_t lenght) = 0;
        virtual int write(uint8_t *data, uint32_t size) = 0;
        virtual int filter_init(uint32_t can_id) = 0;
        virtual int filter_deinit(uint32_t can_id) = 0;
};