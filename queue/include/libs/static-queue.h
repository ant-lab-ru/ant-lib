#pragma once

#include <stdint.h>

class StaticQueue
{
    public:
        uint32_t first_num_elem_queue;
        uint32_t last_num_elem_queue;

        uint32_t element_size_queue;
        uint32_t capacity_queue;

        uint32_t* buffer_queue;

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
