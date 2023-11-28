#include "ant-lib/hmc5883l.h"
#include "ant-lib/hmc5883l-regs.h"

//============================================================================================
//  Public
//============================================================================================

int Hmc5883l::init(II2c* i_i2c)
{
	this->i2c_ = i_i2c;
	return HMC5883L_OK;
}

int Hmc5883l::check()
{
	if (this->read_reg(HMC5883L_ID_REG_A_ADDR) != HMC5883L_ID_REG_A_VALUE) {
		return -1;
	}
	if (this->read_reg(HMC5883L_ID_REG_B_ADDR) != HMC5883L_ID_REG_B_VALUE) {
		return -2;
	}
	if (this->read_reg(HMC5883L_ID_REG_C_ADDR) != HMC5883L_ID_REG_C_VALUE) {
		return -3;
	}
	return HMC5883L_OK;
}

//============================================================================================
//  Private
//============================================================================================

int Hmc5883l::write_reg(uint8_t addr, uint8_t value)
{
	uint8_t buf[] = { addr, value };
	this->i2c_->write(buf, sizeof(buf));
	return HMC5883L_OK;
}

int Hmc5883l::read_reg(uint8_t addr)
{
	uint8_t buf[] = { addr };
	this->i2c_->write(buf, sizeof(buf));
	
	uint8_t read_buf[1] = { 0 };
	this->i2c_->read(read_buf, sizeof(read_buf));

	return read_buf[0];
}


