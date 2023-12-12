#pragma once

#include <stdint.h>

constexpr int16_t hmc5883l_ref_value_low  = 243; 
constexpr int16_t hmc5883l_ref_value_high = 575; 

#define HMC5883L_CALC_CALIBRATED(raw, ref, test) ((int16_t)( (int32_t)(raw) * (int32_t)(ref) / (int32_t)(test) ))

#define HMC5883L_UINT16_TO_INT16(u) (int16_t)(((u) & (1 << 15)) ? (-(~(u) + 1)) : (u))

// Configuration registers

// REG A
constexpr uint8_t HMC5883L_CONFIG_REG_A_ADDR = 0;
typedef enum : uint8_t
{
	HMC5883L_AVERAGING_1 = 0, // Default
	HMC5883L_AVERAGING_2 = 1,
	HMC5883L_AVERAGING_4 = 2,
	HMC5883L_AVERAGING_8 = 3,
} hmc5883l_averaging_t;

typedef enum : uint8_t
{
	HMC5883L_DATARATE_0_75Hz = 0,
	HMC5883L_DATARATE_1_5Hz  = 1,
	HMC5883L_DATARATE_3Hz    = 2,
	HMC5883L_DATARATE_7_5Hz  = 3,
	HMC5883L_DATARATE_15Hz   = 4, // Default
	HMC5883L_DATARATE_30Hz   = 5,
	HMC5883L_DATARATE_75Hz   = 6,
	HMC5883L_DATARATE_RSVD   = 7,
} hmc5883l_datarate_t;

typedef enum : uint8_t
{
	HMC5883L_BIAS_MODE_NORMAL   = 0, // Default
	HMC5883L_BIAS_MODE_POSITIVE = 1,
	HMC5883L_BIAS_MODE_NEGATIVE = 2,
	HMC5883L_BIAS_MODE_RSVD     = 3,
} hmc5883l_bias_mode_t;

typedef struct 
{
	uint8_t MS : 2;
	uint8_t DO : 3;
	uint8_t MA : 2;
	uint8_t    : 1;
} hmc5883l_reg_a_bf_t;

typedef union
{
	hmc5883l_reg_a_bf_t bf;
	uint8_t byte;
} hmc5883l_reg_a_t;


// REG B
constexpr uint8_t HMC5883L_CONFIG_REG_B_ADDR = 1;
typedef enum : uint8_t
{
	HMC5883L_GAIN0_1370_LSB_PER_GAUSS = 0,
	HMC5883L_GAIN1_1090_LSB_PER_GAUSS = 1, // Default
	HMC5883L_GAIN2_820_LSB_PER_GAUSS  = 2,
	HMC5883L_GAIN3_660_LSB_PER_GAUSS  = 3,
	HMC5883L_GAIN4_440_LSB_PER_GAUSS  = 4,
	HMC5883L_GAIN5_390_LSB_PER_GAUSS  = 5,
	HMC5883L_GAIN6_330_LSB_PER_GAUSS  = 6,
	HMC5883L_GAIN7_230_LSB_PER_GAUSS  = 7,
} hmc5883l_gain_t;

typedef struct 
{
	uint8_t    : 5;
	uint8_t GN : 3;
} hmc5883l_reg_b_bf_t;

typedef union
{
	hmc5883l_reg_b_bf_t bf;
	uint8_t byte;
} hmc5883l_reg_b_t;

// Mode register
constexpr uint8_t HMC5883L_MODE_REG_ADDR = 2;
typedef enum : uint8_t
{
	HMC5883L_MODE_CONTINUOUS     = 0, 
	HMC5883L_MODE_SINGLE_MEASURE = 1, // Default
	HMC5883L_MODE_IDLE0          = 2, // 
	HMC5883L_MODE_IDLE1          = 3, // IDLE0 is equal to IDLE1
} hmc5883l_mode_t;

typedef struct 
{
	uint8_t MD : 2;
	uint8_t    : 5;
	uint8_t HS : 1;
} hmc5883l_reg_mode_bf_t;

typedef union
{
	hmc5883l_reg_mode_bf_t bf;
	uint8_t byte;
} hmc5883l_reg_mode_t;

// Data registers
constexpr uint8_t HMC5883L_XYZ_REG_ADDR = 3;
constexpr uint8_t HMC5883L_XYZ_REG_SIZE = 6;

constexpr uint8_t HMC5883L_X_REG_ADDR = 3;
constexpr uint8_t HMC5883L_X_REG_SIZE = 2;
constexpr uint8_t HMC5883L_X_MSB_REG_ADDR = 3;
constexpr uint8_t HMC5883L_X_LSB_REG_ADDR = 4;

constexpr uint8_t HMC5883L_Z_REG_ADDR = 5;
constexpr uint8_t HMC5883L_Z_REG_SIZE = 2;
constexpr uint8_t HMC5883L_Z_MSB_REG_ADDR = 5;
constexpr uint8_t HMC5883L_Z_LSB_REG_ADDR = 6;

constexpr uint8_t HMC5883L_Y_REG_ADDR = 7;
constexpr uint8_t HMC5883L_Y_REG_SIZE = 2;
constexpr uint8_t HMC5883L_Y_MSB_REG_ADDR = 7;
constexpr uint8_t HMC5883L_Y_LSB_REG_ADDR = 8;

// Status register
constexpr uint8_t HMC5883L_STATUS_REG_ADDR = 9;
typedef struct 
{
	uint8_t RDY  : 1;
	uint8_t LOCK : 1;
	uint8_t      : 6;
} hmc5883l_reg_status_bf_t;

typedef union
{
	hmc5883l_reg_status_bf_t bf;
	uint8_t byte;
} hmc5883l_reg_status_t;

// Identification registers
constexpr uint8_t HMC5883L_ID_REG_ADDR = 10;
constexpr uint8_t HMC5883L_ID_REG_SIZE = 3;

constexpr uint8_t HMC5883L_ID_REG_A_ADDR = 10;
constexpr uint8_t HMC5883L_ID_REG_A_VALUE = 'H';
constexpr uint8_t HMC5883L_ID_REG_B_ADDR = 11;
constexpr uint8_t HMC5883L_ID_REG_B_VALUE = '4';
constexpr uint8_t HMC5883L_ID_REG_C_ADDR = 12;
constexpr uint8_t HMC5883L_ID_REG_C_VALUE = '3';
