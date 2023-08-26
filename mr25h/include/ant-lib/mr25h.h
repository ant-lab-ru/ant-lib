#include <stdint.h>
#include "interfaces/spi-if.h"

enum
{
    MR25H_OK = 0,
    MR25H_ERR_NullPtr = 1,
    MR25H_NumberOfTypes,
};

class Mr25h
{
    public:
        int init(ISpi* i_spi);
        int read(uint32_t address, uint8_t* buffer, uint32_t lenght);
        int write(uint32_t address, const uint8_t* data, uint32_t size);
        int fill(uint32_t address, uint32_t lenght, uint8_t fill_value);
        int check();

    private:
        ISpi* spi;
}

