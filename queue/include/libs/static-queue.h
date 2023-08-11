#pragma once

#include <stdint.h>

class StaticQueue
{
    public:
        void init(uint32_t element_size, uint32_t element_count, void* buffer);
        void deinit();

        bool empty();
        bool full();

        bool add(const void* element);
        void add_blocking(const void* element);
        
        void* reserve();
        bool unreserve();

        void* remove();
        void* remove_blocking();
};
