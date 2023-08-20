#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef struct {
    uint32_t reserved_number;
    uint8_t* element;
} reserved_element_t;


class StaticQueue
{
    private:
        uint32_t first_num_elem_queue;
        uint32_t last_num_elem_queue;
        uint32_t current_numbers_of_elem_queue;

        uint32_t element_size_queue;
        uint32_t capacity_queue;
        reserved_element_t reserved_element;
        bool is_reserved_space_in_queue;
        void* buffer_queue;
        
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

        uint32_t get_num_first_elem();
        uint32_t get_num_last_elem();
        uint32_t get_number_of_elem();
        uint32_t get_capacity();
        uint32_t get_value_first_elem();
        uint32_t get_value_last_elem() ;
        uint32_t get_elem_by_number(uint32_t number_of_queue_elem);
        bool get_is_reserved_space_in_queue();
        void get_queue_info();
        bool add_reserve_element(const void* element);
        reserved_element_t get_reserve_element();
};