#pragma once
#include "stdint.h"
#include "ant-lib/ehas.h"
#include "stm32l0xx_hal.h"

// Interfaces
#include "ant-lib/if-i2c.h"

enum
{
    I2C_OK = 0,
    I2C_RC_NullPtr,
	I2C_RC_Hal,

    I2C_NumberOfTypes,
};

class I2c : public II2c
{
    public:
	    I2c(I2C_HandleTypeDef* hi2c, uint8_t i2c_addr)
		: hi2c_(hi2c)
		, i2c_addr_(i2c_addr)
		, EHAS_INIT(I2c, "i2c") {};

        int read(uint8_t* buffer, uint32_t lenght);
        int write(const uint8_t* data, uint32_t size);
	
		EHAS_PACK(I2C_NumberOfTypes);

    private:
        I2C_HandleTypeDef* hi2c_;
        const uint8_t i2c_addr_;
};

