#include <stdint.h>

#include "ant-interfaces/spi-if.h"
#include "ant-interfaces/wdt-if.h"
#include "ant-lib/ehas.h"


enum
{
    MR25H_OK = 0,
    MR25H_ERR_NullPtr,
    MR25H_ERR_SpiTx,
    MR25H_ERR_SpiRx,

    MR25H_NumberOfTypes,
};

typedef enum {
    MR25H_UNLOCK = 0,
    MR25H_UPPER_QUARTER_LOCKED,
    MR25H_UPPER_HALF_LOCKED,
    MR25H_ALL_LOCKED,
} mr25h_lock_t;

class Mr25h
{
    public:
        mr25h_lock_t lock;
        int init(ISpi* i_spi, IWdt* i_wdt);
        int read(uint32_t address, uint8_t* buffer, uint32_t lenght);
        int write(uint32_t address, const uint8_t* data, uint32_t size);
        int fill(uint32_t address, uint32_t lenght, uint8_t fill_value);
        int check();

    private:
        ISpi* spi;
        IWdt* wdt;
        void little2big_address(uint32_t little, uint8_t* big);
        EhasPack<MR25H_NumberOfTypes> ehas;
};

