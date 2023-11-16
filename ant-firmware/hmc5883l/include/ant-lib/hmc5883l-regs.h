#pragma once

#include <stdint.h>

// Configuration registers
constexpr uint8_t HMC5883L_CONFIG_REG_ADDR = 0;
constexpr uint8_t HMC5883L_CONFIG_REG_SIZE = 2;

constexpr uint8_t HMC5883L_CONFIG_REG_A_ADDR = 0;
constexpr uint8_t HMC5883L_CONFIG_REG_B_ADDR = 1;

// Mode register
constexpr uint8_t HMC5883L_MODE_REG_ADDR = 2;

// Data registers
constexpr uint8_t HMC5883L_DATA_REG_ADDR = 3;
constexpr uint8_t HMC5883L_DATA_SIZE_ADDR = 6;

constexpr uint8_t HMC5883L_X_MSB_REG_ADDR = 3;
constexpr uint8_t HMC5883L_X_LSB_REG_ADDR = 4;
constexpr uint8_t HMC5883L_Z_MSB_REG_ADDR = 5;
constexpr uint8_t HMC5883L_Z_LSB_REG_ADDR = 6;
constexpr uint8_t HMC5883L_Y_MSB_REG_ADDR = 7;
constexpr uint8_t HMC5883L_Y_LSB_REG_ADDR = 8;

// Status register
constexpr uint8_t HMC5883L_STATUS_REG_ADDR = 9;

// Identification registers
constexpr uint8_t HMC5883L_ID_REG_ADDR = 10;
constexpr uint8_t HMC5883L_ID_REG_SIZE = 3;

constexpr uint8_t HMC5883L_ID_REG_A_ADDR = 10;
constexpr uint8_t HMC5883L_ID_REG_A_VALUE = 'H';
constexpr uint8_t HMC5883L_ID_REG_B_ADDR = 11;
constexpr uint8_t HMC5883L_ID_REG_B_VALUE = '4';
constexpr uint8_t HMC5883L_ID_REG_C_ADDR = 12;
constexpr uint8_t HMC5883L_ID_REG_C_VALUE = '3';

