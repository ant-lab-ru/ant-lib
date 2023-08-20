#include <gtest/gtest.h>

#include "libs/static-queue.h"

#define QUEUE_TEST_MEMORY_SIZE 8
uint8_t test_uint8_t_buff[QUEUE_TEST_MEMORY_SIZE] = {0};

uint8_t test_var_for_get_ptr = 0;

int init_test_queue() {

    StaticQueue test_queue;
    test_queue.init(sizeof(uint8_t), QUEUE_TEST_MEMORY_SIZE, test_uint8_t_buff);

    EXPECT_EQ(test_queue.get_capacity(), QUEUE_TEST_MEMORY_SIZE);
    EXPECT_EQ(test_queue.get_num_first_elem(), 0);
    EXPECT_EQ(test_queue.get_num_last_elem(), 0);
    EXPECT_EQ(test_queue.get_number_of_elem(), 0);

  //  test_queue.deinit();

    return 0;
}

int add_remove_test_queue() {

    StaticQueue test_queue;
    test_queue.init(sizeof(uint8_t), QUEUE_TEST_MEMORY_SIZE, test_uint8_t_buff);
    EXPECT_EQ(test_queue.get_num_last_elem(), 0);
    uint8_t test_elem_val = 5;
    uint8_t* ptr = &test_elem_val;

    std::cout<< "\n=================== ADD ELEMENT TO QUEUE ===================" << std::endl;
    EXPECT_EQ(test_queue.add(ptr), true);
    EXPECT_EQ(test_queue.get_num_first_elem(), 0);
    EXPECT_EQ(test_queue.get_num_last_elem(), 1);
    EXPECT_EQ(test_queue.get_number_of_elem(), 1);

    EXPECT_EQ(test_queue.get_value_first_elem(), 5);
    EXPECT_EQ(test_queue.get_value_last_elem(), 0);

    EXPECT_EQ(test_queue.empty(), 0);
    std::cout<< "\n=================== REMOVE AN ELEMENT FROM QUEUE ===================" << std::endl;
   

    uint8_t* re =(uint8_t*)test_queue.remove();

    EXPECT_EQ(test_queue.get_num_first_elem(), 1);
    EXPECT_EQ(test_queue.get_num_last_elem(), 1);

    EXPECT_EQ(test_queue.get_number_of_elem(), 0);

    EXPECT_EQ(test_queue.get_value_first_elem(), 0);
    EXPECT_EQ(test_queue.get_value_last_elem(), 0);
    
    EXPECT_EQ(test_queue.empty(), 1);

    std::cout<< "\n=================== ADD ELEMENTS TO QUEUE ===================" << std::endl;
    for( int i = 0; i < QUEUE_TEST_MEMORY_SIZE + 10; i++) {
        uint8_t test_elem = i;
        uint8_t* ptr_test = &test_elem;
        if (i < QUEUE_TEST_MEMORY_SIZE ) {
            EXPECT_EQ(test_queue.add(ptr_test), true);
        }
        else {
            EXPECT_EQ(test_queue.add(ptr_test), false);
        }
    }
    EXPECT_EQ(test_queue.get_num_first_elem(), 1);
    EXPECT_EQ(test_queue.full(), 1);
    for( int i = test_queue.get_num_first_elem(); i < QUEUE_TEST_MEMORY_SIZE; i++) 
    {
        EXPECT_EQ(test_queue.get_elem_by_number(i), i - 1);
    }
    std::cout<< "\n=================== REMOVE AN ELEMENT FROM FULL QUEUE ===================" << std::endl;
   

    uint8_t* re_full =(uint8_t*)test_queue.remove();
    EXPECT_EQ(*re_full, 0);
    EXPECT_EQ(test_queue.full(), 0);
   
    return 0;
}

int add_blocking_test_queue() {

    StaticQueue test_queue;
    test_queue.init(sizeof(uint8_t), QUEUE_TEST_MEMORY_SIZE, test_uint8_t_buff);
    EXPECT_EQ(test_queue.get_num_last_elem(), 0);
    uint8_t test_elem_val = 5;
    uint8_t* ptr = &test_elem_val;

    std::cout<< "\n=================== ADD ELEMENT TO QUEUE ===================" << std::endl;
    EXPECT_EQ(test_queue.add(ptr), true);
    EXPECT_EQ(test_queue.get_num_first_elem(), 0);
    EXPECT_EQ(test_queue.get_num_last_elem(), 1);
    EXPECT_EQ(test_queue.get_number_of_elem(), 1);

    EXPECT_EQ(test_queue.get_value_first_elem(), 5);
    EXPECT_EQ(test_queue.get_value_last_elem(), 0);

    EXPECT_EQ(test_queue.empty(), 0);
    std::cout<< "\n=================== REMOVE AN ELEMENT FROM QUEUE ===================" << std::endl;
   

    uint8_t* re =(uint8_t*)test_queue.remove();

    EXPECT_EQ(test_queue.get_num_first_elem(), 1);
    EXPECT_EQ(test_queue.get_num_last_elem(), 1);

    EXPECT_EQ(test_queue.get_number_of_elem(), 0);

    EXPECT_EQ(test_queue.get_value_first_elem(), 0);
    EXPECT_EQ(test_queue.get_value_last_elem(), 0);
    
    EXPECT_EQ(test_queue.empty(), 1);

    std::cout<< "\n=================== ADD ELEMENTS TO QUEUE ===================" << std::endl;
    for( int i = 0; i < QUEUE_TEST_MEMORY_SIZE + 10; i++) {
        uint8_t test_elem = i;
        uint8_t* ptr_test = &test_elem;
        if (i < QUEUE_TEST_MEMORY_SIZE ) {
            EXPECT_EQ(test_queue.add(ptr_test), true);
        }
        else {
            EXPECT_EQ(test_queue.add(ptr_test), false);
        }
    }
    EXPECT_EQ(test_queue.get_num_first_elem(), 1);
    EXPECT_EQ(test_queue.full(), 1);
    for( int i = test_queue.get_num_first_elem(); i < QUEUE_TEST_MEMORY_SIZE; i++) 
    {
        EXPECT_EQ(test_queue.get_elem_by_number(i), i - 1);
    }

    std::cout<< "\n=================== ADD BLOCKING ELEMENT FROM FULL QUEUE ===================" << std::endl;
    uint8_t test_blocking_elem = 19;
    uint8_t* ptr_blocking_test = &test_blocking_elem;

    // for (int i = 0; i < 1000; i++){
    //     if (i % 100 == 0) {
    //         uint8_t* re_full =(uint8_t*)test_queue.remove();
    //     }
    //     test_queue.add_blocking(ptr_blocking_test);
    // }
    std::cout<< "\n=================== REMOVE AN ELEMENT FROM FULL QUEUE ===================" << std::endl;

   
    return 0;
}



TEST(StaticQueueReserved, base)
{
    init_test_queue();
}

TEST(StaticQueueAddRemove, base)
{
    add_remove_test_queue();
}

TEST(StaticQueueAddBlock, base)
{
    add_blocking_test_queue();
}
