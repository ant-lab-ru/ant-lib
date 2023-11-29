#pragma once

#include "stdint.h"

class StaticQueue
{
public:
    StaticQueue(void* buffer, uint32_t element_size, uint32_t element_count):
        _buffer(buffer), _element_size(element_size), _element_count(element_count)
        {};

    bool empty();
    bool full();

    bool add(const void* element);
    void* reserve();
    void* remove();

    uint32_t get_free();
    uint32_t get_capacity();

private:
    void*    _buffer;
    const uint32_t _element_size;
    const uint32_t _element_count;
};
