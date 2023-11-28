#pragma once
#include "stdint.h"
#include "ant-lib/ehas.h"
#include "stm32l0xx_hal.h"

// Interfaces
#include "ant-hwi/i2c-if.h"

enum
{
    I2C_OK = 0,
    I2C_ERROR_NullPtr,
	I2C_ERROR_Hal,

    I2C_NumberOfTypes,
};

class I2c : public II2c
{
    public:
	    I2c(I2C_HandleTypeDef* hi2c, uint8_t i2c_addr)
		    : hi2c_(hi2c), i2c_addr_(i2c_addr) {
			    EHAS_INIT_NOINIT_DEVICE("i2c");
			    EHAS_INIT_PACK(I2C_ERROR, NullPtr, EHAS_ERROR);
			    EHAS_INIT_PACK(I2C_ERROR, Hal,     EHAS_ERROR);
		    };

        int read(uint8_t* buffer, uint32_t lenght);
        int write(const uint8_t* data, uint32_t size);

    private:
        I2C_HandleTypeDef* hi2c_;
        const uint8_t i2c_addr_;
	    EhasPack<I2C_NumberOfTypes> ehas;
};
