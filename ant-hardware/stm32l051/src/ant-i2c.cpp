#include "stm32l051/inc/ant-i2c.h"

int I2c::read(uint8_t* buffer, uint32_t lenght)
{
	HAL_StatusTypeDef rc = HAL_I2C_Master_Receive(this->hi2c_, ((uint16_t)this->i2c_addr_) << 1, buffer, lenght, 1000);
	if (rc != HAL_OK) {
		EHAS_RETURN(I2C_ERROR_Hal);
	}
	return lenght;
}

int I2c::write(const uint8_t* data, uint32_t size)
{
	HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(this->hi2c_, ((uint16_t)this->i2c_addr_) << 1, (uint8_t*)data, size, 1000);
	if (rc != HAL_OK) {
		EHAS_RETURN(I2C_ERROR_Hal);
	}
	return size;
}

