#include <stdint.h>

#include "interfaces/spi-if.h"
#include "interfaces/wdt-if.h"
#include "ant-lib/ehas.h"

enum
{
    MR25H_OK = 0,
    MR25H_ERR_NullPtr,
    MR25H_NumberOfTypes,
};

class Mr25h
{
    public:
        int init(ISpi* i_spi, IWdt* i_wdt);
        int read(uint32_t address, uint8_t* buffer, uint32_t lenght);
        int write(uint32_t address, const uint8_t* data, uint32_t size);
        int fill(uint32_t address, uint32_t lenght, uint8_t fill_value);
        int check();

    private:
        ISpi* spi;
        void little2big_address(uint32_t little, uint8_t* big);
        EhasPack ehas;
}

