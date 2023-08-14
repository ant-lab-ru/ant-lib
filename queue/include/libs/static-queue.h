#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class StaticQueue
{
    private:
        uint32_t first_num_elem_queue;
        uint32_t last_num_elem_queue;
        uint32_t current_numbers_of_elem_queue;

        uint32_t element_size_queue;
        uint32_t capacity_queue;

        uint32_t* buffer_queue;
    public:
        void init(uint32_t element_size, uint32_t element_count, void* buffer);
        void deinit();

        bool empty();
        bool full();

        bool add(const void* element);
        void add_blocking(const void* element);
        
        void* reserve();
        bool unreserve();

        uint32_t get_first_elem();
        uint32_t get_last_elem();
        uint32_t get_number_of_elem();
        uint32_t get_capacity();

        void* remove();
        void* remove_blocking();
};