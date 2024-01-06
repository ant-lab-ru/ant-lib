#pragma once

#include "stdint.h"

constexpr uint8_t EBYTE_E22_RW_CMD_LEN      = 3;

constexpr uint8_t EBYTE_E22_ADDRH_REG_ADDR  = 0x00;
constexpr uint8_t EBYTE_E22_ADDRL_REG_ADDR  = 0x01;
constexpr uint8_t EBYTE_E22_NETID_REG_ADDR  = 0x02;
constexpr uint8_t EBYTE_E22_REG0_REG_ADDR   = 0x03;
constexpr uint8_t EBYTE_E22_REG1_REG_ADDR   = 0x04;
constexpr uint8_t EBYTE_E22_REG2_REG_ADDR   = 0x05;
constexpr uint8_t EBYTE_E22_REG3_REG_ADDR   = 0x06;
constexpr uint8_t EBYTE_E22_CRYPTH_REG_ADDR = 0x07;
constexpr uint8_t EBYTE_E22_CRYPTL_REG_ADDR = 0x08;

constexpr uint8_t EBYTE_E22_PID_REG_ADDR    = 0x80;
constexpr uint8_t EBYTE_E22_PID_REG_SIZE    = 7;

constexpr uint8_t EBYTE_E22_CMD_WRITE = 0xC0;
constexpr uint8_t EBYTE_E22_CMD_READ  = 0xC1;

typedef enum : uint8_t 
{
    EBYTE_E22_MODE_NORMAL = 0,
    EBYTE_E22_MODE_WOR = 1,
    EBYTE_E22_MODE_CONFIG = 2,
    EBYTE_E22_MODE_SLEEP = 3,
} ebyte_e22_mode_t;
