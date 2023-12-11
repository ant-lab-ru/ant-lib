#include "ant-lib/hmc5883l.h"
#include "ant-lib/hmc5883l-regs.h"

//============================================================================================
//  Public
//============================================================================================

int Hmc5883l::init(II2c* i2c, IGpio* DRDY)
{
	_i2c = i2c;
	_drdy = DRDY;

	return check();
}

int Hmc5883l::check()
{
	uint8_t id[HMC5883L_ID_REG_SIZE] = { 0 };
	if (!_read_reg_array(HMC5883L_ID_REG_ADDR, id, HMC5883L_ID_REG_SIZE)) {
		EHAS_UP(HMC5883L_ERROR_I2cError);
		return -HMC5883L_ERROR_I2cError;
	}

	if (id[0] != HMC5883L_ID_REG_A_VALUE || id[1] != HMC5883L_ID_REG_B_VALUE || id[2] != HMC5883L_ID_REG_C_VALUE) {
		EHAS_UP(HMC5883L_ERROR_ChipVerif);
		return -HMC5883L_ERROR_ChipVerif;
	}

	return HMC5883L_OK;
}

int Hmc5883l::get_single_xyz_calibrated(int16_t* x, int16_t* y, int16_t* z)
{
	int16_t raw_x = 0, raw_y = 0, raw_z = 0;
	int rc = get_single_xyz(&raw_x, &raw_y, &raw_z);
	if (rc != HMC5883L_OK) {
		return rc;
	}
	
	if (!last_test_x || !last_test_y || !last_test_z || !ref_x || !ref_y || !ref_z) {
		*x = raw_x;
		*y = raw_y;
		*z = raw_z;
		return HMC5883L_OK;
	}
	
	*x = HMC5883L_CALC_CALIBRATED(raw_x, ref_x, last_test_x);
	*y = HMC5883L_CALC_CALIBRATED(raw_y, ref_y, last_test_y);
	*z = HMC5883L_CALC_CALIBRATED(raw_z, ref_z, last_test_z);
	return HMC5883L_OK;
}

int Hmc5883l::get_single_xyz(int16_t* x, int16_t* y, int16_t* z)
{
	int rc = set_mode(HMC5883L_MODE_SINGLE_MEASURE);
	if (rc != HMC5883L_OK) {
		return rc;
	}

	// If DRDY gpio used
	if (_drdy)
	{
		while (_drdy->get()) {}
	}
	else
	{
		hmc5883l_reg_status_bf_t status = { 0 };
		while (1)
		{
			rc = get_status(&status);
			if (rc != HMC5883L_OK) {
				return rc;	
			}
			if (status.RDY) {
				break;
			}
		}
	}

	return get_xyz_raw(x, y, z);
}

int Hmc5883l::self_test()
{
	int rc = set_gain(HMC5883L_GAIN5_390_LSB_PER_GAUSS);
	if (rc != HMC5883L_OK) {
		return rc;
	}

	rc = set_averaging(HMC5883L_AVERAGING_8);
	if (rc != HMC5883L_OK) {
		return rc;
	}

	rc = set_bias_mode(HMC5883L_BIAS_MODE_POSITIVE);
	if (rc != HMC5883L_OK) {
		return rc;
	}

	int16_t x = 0, y = 0, z = 0;
	int32_t sum_x = 0, sum_y = 0, sum_z = 0;

	for (int i = 0; i < self_test_averaging; i++) {
		rc = get_single_xyz(&x, &y, &z);
		if (rc != HMC5883L_OK) {
			set_bias_mode(HMC5883L_BIAS_MODE_NORMAL);
			return rc;
		}
		sum_x += x;
		sum_y += y;
		sum_z += z;
	}
	
	rc = set_bias_mode(HMC5883L_BIAS_MODE_NORMAL);
	if (rc != HMC5883L_OK) {
		return rc;
	}

	last_test_x = sum_x / self_test_averaging;
	last_test_y = sum_y / self_test_averaging;
	last_test_z = sum_z / self_test_averaging;

	if (last_test_x < hmc5883l_ref_value_low || last_test_x > hmc5883l_ref_value_high) {
		EHAS_UP(HMC5883L_ERROR_SelfTest);
		return -HMC5883L_ERROR_SelfTest;
	}
	if (last_test_y < hmc5883l_ref_value_low || last_test_y > hmc5883l_ref_value_high) {
		EHAS_UP(HMC5883L_ERROR_SelfTest);
		return -HMC5883L_ERROR_SelfTest;
	}
	if (last_test_z < hmc5883l_ref_value_low || last_test_z > hmc5883l_ref_value_high) {
		EHAS_UP(HMC5883L_ERROR_SelfTest);
		return -HMC5883L_ERROR_SelfTest;
	}

	return HMC5883L_OK;
}

#define HMC5883L_SET_FIELD(field, rtype, raddr) {\
	rtype reg = { 0 };\
	if (!_read_reg(raddr, &reg.byte)) {\
		EHAS_UP(HMC5883L_ERROR_I2cError);\
		return -HMC5883L_ERROR_I2cError;\
	}\
	reg.bf.field = value;\
	if (!_write_reg(raddr, reg.byte)) {\
		EHAS_UP(HMC5883L_ERROR_I2cError);\
		return -HMC5883L_ERROR_I2cError;\
	}\
	uint8_t check = 0;\
	if (!_read_reg(raddr, &check)) {\
		EHAS_UP(HMC5883L_ERROR_I2cError);\
		return -HMC5883L_ERROR_I2cError;\
	}\
	if (check != reg.byte) {\
		EHAS_UP(HMC5883L_ERROR_RegVerif);\
		return -HMC5883L_ERROR_RegVerif;\
	}\
}

int Hmc5883l::set_averaging(hmc5883l_averaging_t value) {
	HMC5883L_SET_FIELD(MA, hmc5883l_reg_a_t, HMC5883L_CONFIG_REG_A_ADDR);
	return HMC5883L_OK;
}

int Hmc5883l::set_datarate(hmc5883l_datarate_t value) {
	HMC5883L_SET_FIELD(DO, hmc5883l_reg_a_t, HMC5883L_CONFIG_REG_A_ADDR);
	return HMC5883L_OK;
}

int Hmc5883l::set_bias_mode(hmc5883l_bias_mode_t value) {
	HMC5883L_SET_FIELD(MS, hmc5883l_reg_a_t, HMC5883L_CONFIG_REG_A_ADDR);
	return HMC5883L_OK;
}

int Hmc5883l::set_gain(hmc5883l_gain_t value) {
	HMC5883L_SET_FIELD(GN, hmc5883l_reg_b_t, HMC5883L_CONFIG_REG_B_ADDR);
	return HMC5883L_OK;
}

int Hmc5883l::set_HS_mode(bool value) {
	HMC5883L_SET_FIELD(HS, hmc5883l_reg_mode_t, HMC5883L_MODE_REG_ADDR);
	return HMC5883L_OK;
}

int Hmc5883l::set_mode(hmc5883l_mode_t value) {
	HMC5883L_SET_FIELD(MD, hmc5883l_reg_mode_t, HMC5883L_MODE_REG_ADDR);
	return HMC5883L_OK;
}

int Hmc5883l::get_status(hmc5883l_reg_status_bf_t* value) {
	hmc5883l_reg_status_t reg = { 0 };
	if (!_read_reg(HMC5883L_STATUS_REG_ADDR, &reg.byte)) {
		EHAS_UP(HMC5883L_ERROR_I2cError);
		return -HMC5883L_ERROR_I2cError;
	}
	*value = reg.bf;
	return HMC5883L_OK;
}


int Hmc5883l::get_xyz_raw(int16_t* x, int16_t* y, int16_t* z) {
	uint8_t data[HMC5883L_XYZ_REG_SIZE] = { 0 };
	if (!_read_reg_array(HMC5883L_XYZ_REG_ADDR, data, HMC5883L_XYZ_REG_SIZE)) {
		EHAS_UP(HMC5883L_ERROR_I2cError);
		return -HMC5883L_ERROR_I2cError;
	}
	
	*x = HMC5883L_UINT16_TO_INT16( ((uint16_t)data[0]) * 256 + ((uint16_t)data[1]) );
	*z = HMC5883L_UINT16_TO_INT16( ((uint16_t)data[2]) * 256 + ((uint16_t)data[3]) );
	*y = HMC5883L_UINT16_TO_INT16( ((uint16_t)data[4]) * 256 + ((uint16_t)data[5]) );

	return HMC5883L_OK;
}


//============================================================================================
//  Private
//============================================================================================

bool Hmc5883l::_write_reg(uint8_t addr, uint8_t value)
{
	uint8_t buf[] = { addr, value };
	if (_i2c->write(buf, sizeof(buf)) != sizeof(buf)) {
		return false;
	}

	return true;
}

bool Hmc5883l::_write_reg_array(uint8_t addr, uint8_t* values, uint8_t size)
{
	uint8_t buf[1 + size];
	buf[0] = addr;
	memcpy(&buf[1], values, size);

	if (_i2c->write(buf, sizeof(buf)) != sizeof(buf)) {
		return false;
	}

	return true;
}

bool Hmc5883l::_read_reg(uint8_t addr, uint8_t* value)
{
	uint8_t buf[] = { addr };
	if (_i2c->write(buf, sizeof(buf)) != sizeof(buf)) {
		return false;		
	}
	
	if (_i2c->read(value, 1) != 1) {
		return false;
	}

	return true;
}

bool Hmc5883l::_read_reg_array(uint8_t addr, uint8_t* buffer, uint8_t lenght)
{
	uint8_t buf[] = { addr };
	if (_i2c->write(buf, sizeof(buf)) != sizeof(buf)) {
		return false;
	}

	if (_i2c->read(buffer, lenght) != lenght) {
		return false;
	}

	return true;
}



ehas_pack_t Hmc5883l::ehas_pack[HMC5883L_NumberOfTypes] = {0};
