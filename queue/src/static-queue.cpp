#pragma once

#include <libs/static-queue.h>

using namespace::std;

/**
 * @brief Инициализация новой Static Queue:: Static Queue object
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
    current_numbers_of_elem_queue = 0;

    first_num_elem_queue = 0;
    last_num_elem_queue = 0;

    if (buffer == NULL) {
        buffer_queue = (uint32_t*) malloc(sizeof(element_size_queue) *  element_count);
    }
	else {
        buffer_queue = (uint32_t*)buffer;
    }

    //buffer_queue = (uint32_t*)buffer;
}

/**
 * @brief Destroy the Static Queue:: Static Queue object
 * Очищение памяти занятой элементом класса статической очереди
 */
void StaticQueue::deinit() {
    element_size_queue = 0;
    capacity_queue = 0;

    current_numbers_of_elem_queue = 0;

    first_num_elem_queue = 0;
    last_num_elem_queue = 0;

    free(buffer_queue);
    //delete[] buffer_queue;
}

bool StaticQueue::empty() {
    if (last_num_elem_queue == 0) {
        cout << "Queue is empty" << endl;
        return true;
    }
    return false;
}

bool StaticQueue::full() {
    if ((first_num_elem_queue = 0) && (last_num_elem_queue == capacity_queue - 1) && current_numbers_of_elem_queue == capacity_queue) {
        cout << "Queue is full" << endl;
        return true;
    }
    return false;
}


bool StaticQueue::add(const void* element) {
    int rc = StaticQueue::full();

    if (rc) {
        return false;//STATIC_QUEUE_IS_FULL;
    }

 
    memcpy((uint32_t*)buffer_queue + (last_num_elem_queue * element_size_queue), element, element_size_queue);

    last_num_elem_queue += 1;
    current_numbers_of_elem_queue += 1;

	return true;
}

void StaticQueue::add_blocking(const void* element) {
    int rc = StaticQueue::full();
    if (rc) {
         cout << "STATIC_QUEUE_IS_FULL - NEED TO FREE SPACE FOR NEW ELEM";
    }
    while(rc){
        rc = StaticQueue::full();
    }

    if (sizeof(element) != element_size_queue) {
        cout << "STATIC_QUEUE_SIZE_ELEM";
    }

    memcpy((uint32_t*)buffer_queue + (last_num_elem_queue * element_size_queue), element, element_size_queue);

    last_num_elem_queue += 1;
    current_numbers_of_elem_queue += 1;

}


void* StaticQueue::remove() {
    int rc = StaticQueue::empty();

    if (rc) {
        cout << "STATIC_QUEUE_IS_EMPTY";
    }

	void* kill_item = (uint32_t*)buffer_queue + (first_num_elem_queue  * element_size_queue);

    first_num_elem_queue += 1;
    current_numbers_of_elem_queue -= 1;

    return kill_item;
}


void* StaticQueue::remove_blocking() {
    int rc = StaticQueue::empty();
    if (rc) {
        cout << "STATIC_QUEUE_IS_EMPTY - NEED TO ADD NEW ELEM";
    }
    while(rc){
        rc = StaticQueue::empty();
    }

	void* kill_item = (uint32_t*)buffer_queue + (first_num_elem_queue  * element_size_queue);

    first_num_elem_queue += 1;
    current_numbers_of_elem_queue -= 1;

    return kill_item;
}

void* StaticQueue::reserve()
{
    int rc = StaticQueue::full();
	if (rc)
		return NULL;


	last_num_elem_queue += 1;
    current_numbers_of_elem_queue += 1;

	return (uint32_t*)buffer_queue + (last_num_elem_queue  * element_size_queue);
}


bool unreserve();
//==================================================================================
//                  Функции геттеры
//==================================================================================

uint32_t StaticQueue::get_num_first_elem() {
    uint32_t tmp = first_num_elem_queue;
		
	return tmp;

}
uint32_t StaticQueue::get_num_last_elem(){
    uint32_t tmp = last_num_elem_queue;
		
	return tmp;

}
uint32_t StaticQueue::get_number_of_elem(){
    uint32_t tmp = current_numbers_of_elem_queue;
		
	return tmp;

}
uint32_t StaticQueue::get_capacity(){
    uint32_t tmp = capacity_queue;
		
	return tmp;

}
void*  StaticQueue::get_value_first_elem() {
    void* value_first_elem = (uint32_t*)buffer_queue + (first_num_elem_queue  * element_size_queue);
	
	return value_first_elem;

}
void*  StaticQueue::get_value_last_elem(){  
    void* value_last_elem = (uint32_t*)buffer_queue + (last_num_elem_queue  * element_size_queue);
	
	return value_last_elem;
}

void*  StaticQueue::get_elem_by_number(uint32_t number_of_queue_elem){  
     if (number_of_queue_elem < first_num_elem_queue) {
        cout << "NUMBER_IS_OUT_CURRENT_QUEUE - NEED TO BE MORE";
        return StaticQueue::get_value_first_elem();
    }

    if (number_of_queue_elem > last_num_elem_queue) {
        cout << "NUMBER_IS_OUT_CURRENT_QUEUE - NEED TO BE LESS";
        return StaticQueue::get_value_last_elem();
    }


    void* value_of_elem_by_number = (uint32_t*)buffer_queue + (number_of_queue_elem  * element_size_queue);
	
	return value_of_elem_by_number;
}