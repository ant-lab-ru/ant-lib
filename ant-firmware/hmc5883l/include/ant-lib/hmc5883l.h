// Std includes
#include <stdint.h>

// Lib includes
#include "hmc5883l-regs.h"

// Interfaces
#include "ant-hwi/i2c-if.h"

// Ant-lib includes
#include "ant-lib/ehas.h"

enum
{
    HMC5883L_OK = 0,
    HMC5883L_ERROR_NullPtr,

    HMC5883L_NumberOfTypes,
};

class Hmc5883l
{
    public:
        int init(II2c* i_i2c);
        int check();

    private:
        II2c* i2c_;
        int read_reg(uint8_t addr);
        int write_reg(uint8_t addr, uint8_t value);
        EhasPack<HMC5883L_NumberOfTypes> ehas;
};

