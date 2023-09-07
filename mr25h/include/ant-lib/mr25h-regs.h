#pragma once

#include <stdint.h>

// Write enable and disable
#define MR25H_CMD_WREN  0x06
#define MR25H_CMD_WRDI  0x04

// Status register
#define MR25H_CMD_RDSR  0x05
#define MR25H_CMD_WRSR  0x01

// Data read and write
#define MR25H_CMD_READ  0x03
#define MR25H_CMD_WRITE 0x02

// Sleep mode
#define MR25H_CMD_SLEEP 0xB9
#define MR25H_CMD_WAKE  0xAB

typedef struct {
    uint8_t : 1;
    uint8_t WEL : 1;
    uint8_t BP0 : 1;
    uint8_t BP1 : 1;
    uint8_t : 3;
    uint8_t SRWD : 1;
} mr25h_reg_status_bf_t;

typedef union {
    uint8_t word;
    mr25h_reg_status_bf_t bf;
} mr25h_reg_status_t;

