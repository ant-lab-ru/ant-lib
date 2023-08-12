#pragma once

#include <stdint.h>

class StaticQueue
{
    private:
        uint32_t first_num_elem_queue;
        uint32_t last_num_elem_queue;

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

        void* remove();
        void* remove_blocking();
};
/**
 * @brief Construct a new Static Queue:: Static Queue object
 * Инициализация очереди размером element_count,
 * каждый из которым размером element_size.
 * Для хранения выделен buffer.
 * @param element_size 
 * @param element_count 
 * @param buffer 
 */
void StaticQueue::init(uint32_t element_size, uint32_t element_count, void* buffer)
{
    element_size_queue = element_size;
    capacity_queue = element_count;

    first_num_elem_queue = 1;
    last_num_elem_queue = 1;

    buffer_queue = (uint32_t*)buffer;
    return ;
}

/**
 * @brief Destroy the Static Queue:: Static Queue object
 * Очищение памяти занятой элементом класса статической очереди
 */
void StaticQueue::deinit() {
    delete[] buffer_queue;
}

bool StaticQueue::empty() {
    if (last_num_elem_queue < first_num_elem_queue) {
        std::cout << "Queue is empty" << std::endl;
        return -1;
    }
    
}