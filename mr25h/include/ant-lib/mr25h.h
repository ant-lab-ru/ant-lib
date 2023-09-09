// Std includes
#include <stdint.h>

// Lib includes
#include "mr25h-device-api.h"
#include "mr25h-regs.h"

// Interfaces
#include "ant-interfaces/spi-if.h"
#include "ant-interfaces/wdt-if.h"

// Ant-lib includes
#include "ant-lib/ehas.h"


enum
{
    MR25H_OK = 0,
    MR25H_ERR_NullPtr,
    MR25H_ERR_SpiWrite,
    MR25H_ERR_SpiRead,
    MR25H_ERR_SpiSelect,
    MR25H_ERR_SpiDesel,
    MR25H_ERR_OutOfRange,

    MR25H_NumberOfTypes,

};

constexpr uint32_t MR25H_SIZE = 512 * 1024;

class Mr25h : public Mr25hApi
{
    public:
        int init(ISpi* i_spi);
        int read(uint32_t address, uint8_t* buffer, uint32_t lenght);
        int write(uint32_t address, const uint8_t* data, uint32_t size);
        int fill(uint32_t address, uint32_t lenght, uint8_t fill_value);
        int check();

    private:
        ISpi* spi;
        void little2big_address(uint32_t little, uint8_t* big);
        int set_lock_mode(mr25h_lock_t lock_mode);
        int write_cmd(uint8_t cmd);
        int read_status_reg(mr25h_reg_status_bf_t* status_bf);
        int write_status_reg(mr25h_reg_status_bf_t status_bf);

        EhasPack<MR25H_NumberOfTypes> ehas;
};

