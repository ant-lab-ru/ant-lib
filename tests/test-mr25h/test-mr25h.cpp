#include "ant-imitators/imitator-mr25h40.h"
#include <gtest/gtest.h>
#include "ant-lib/mr25h.h"

TEST(Mr25h, init)
{
    ImitatorMr25h40 spi_imitator;
    Mr25h mr25h;

    EXPECT_EQ( mr25h.init(&spi_imitator) , 0 );
}

TEST(Mr25h, read_write)
{
    ImitatorMr25h40 spi_imitator;
    Mr25h mr25h;

    EXPECT_EQ( mr25h.init(&spi_imitator) , 0 );
    constexpr uint32_t addr = 0x200;
    const uint8_t data[] = "Hello, I am test string";
    constexpr uint32_t sz = sizeof(data);

    EXPECT_EQ( mr25h.write(addr, data, sz) , sz );

    uint8_t read_data[sz] = { 0 };
    EXPECT_EQ( mr25h.read(addr, read_data, sz) , sz );

    for (int i = 0; i < sz; i++) {
        EXPECT_EQ( read_data[i] , data[i] );
    }
}
