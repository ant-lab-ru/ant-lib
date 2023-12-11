// Std includes
#include <stdint.h>

// Lib includes
#include "hmc5883l-regs.h"

// Interfaces
#include "ant-lib/if-i2c.h"
#include "ant-lib/if-gpio.h"

// Ant-lib includes
#include "ant-lib/ehas.h"

enum
{
    HMC5883L_OK = 0,
    HMC5883L_ERROR_NullPtr,
	HMC5883L_ERROR_I2cError,
	HMC5883L_ERROR_RegVerif,
	HMC5883L_ERROR_ChipVerif,
	HMC5883L_ERROR_SelfTest,

    HMC5883L_NumberOfTypes,
};

class Hmc5883l
{
public:
	Hmc5883l()
		: EHAS_INIT(Hmc5883l, "hmc") {};

    int init(II2c* i2c, IGpio* DRDY);
    int check();

	int get_single_xyz(int16_t* x, int16_t* y, int16_t* z);
	int get_single_xyz_calibrated(int16_t* x, int16_t* y, int16_t* z);

	int self_test();

	int get_xyz_raw(int16_t* x, int16_t* y, int16_t* z);

	int set_averaging(hmc5883l_averaging_t averaging);
	int set_datarate(hmc5883l_datarate_t value);
	int set_bias_mode(hmc5883l_bias_mode_t value);
	int set_gain(hmc5883l_gain_t value);
	int set_HS_mode(bool value);
	int set_mode(hmc5883l_mode_t value);

	int get_status(hmc5883l_reg_status_bf_t* value);
	
	uint8_t self_test_averaging = 10;
	
	// ref - reference magnetic field values for calibration. User shall store this values. Used for get_single_xyz_calibrated
	int16_t ref_x = 0;
	int16_t ref_y = 0;
	int16_t ref_z = 0;

	// This value store last test calibration values. Used for get_single_xyz_calibrated
	int16_t last_test_x = 0;
	int16_t last_test_y = 0;
	int16_t last_test_z = 0;

	EHAS_PACK(HMC5883L_NumberOfTypes);

private:
	II2c* _i2c;
	IGpio* _drdy;
	bool _read_reg(uint8_t addr, uint8_t* value);
	bool _write_reg(uint8_t addr, uint8_t value);
	bool _read_reg_array(uint8_t addr, uint8_t* buffer, uint8_t lenght);
	bool _write_reg_array(uint8_t addr, uint8_t* values, uint8_t size);
};

