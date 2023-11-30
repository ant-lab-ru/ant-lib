#pragma once
#include "stdint.h"

enum
{
    CCSDS_TM_OK = 0,
    CCSDS_TM_RC_NumberOfTypes,
};

constexpr uint8_t CCSDS_TM_TFVN = 0; // CCSDS SDLP TM 4.1.2.2.2.2
constexpr uint8_t CCSDS_TM_SECONDARY_HEADER_FLAG = 1; // Secondary header is present. CCSDS SDLP TM 4.1.2.7.2.2
constexpr uint8_t CCSDS_TM_SYNC_FLAG = 0; // VCA_SDU not used. CCSDS SDLP TM 4.1.2.7.3.2
constexpr uint8_t CCSDS_TM_PACKET_ORDER_FLAG = 0; // CCSDS SDLP TM 4.1.2.7.4
constexpr uint8_t CCSDS_TM_SEGMENT_LENGHT_ID = 3; // CCSDS SDLP TM 4.1.2.7.5.2
constexpr uint8_t CCSDS_TM_FSH_VERSION = 0; // CCSDS SDLP TM 

constexpr uint16_t CCSDS_TM_NO_FIRST_HEADER_PTR = 0x7FF;

constexpr uint8_t CCSDS_TM_PHEADER_SIZE = 6;
constexpr uint8_t CCSDS_TM_SHEADER_HEAD_SIZE = 1;

typedef struct {
    uint16_t OCF_flag: 1;
    uint16_t VCID: 3;
    uint16_t SCID: 10;
    uint16_t TFVN: 2;

    uint8_t MC_frame_count;
    uint8_t VC_frame_count;

    uint16_t first_header_ptr: 11;
    uint16_t segment_length_id: 2;
    uint16_t packet_order_flag: 1;
    uint16_t sync_flag: 1;
    uint16_t secondary_header_flag: 1;
} ccsds_tm_primary_header_bf_t;

typedef union {
    ccsds_tm_primary_header_bf_t bf;
    struct {
        uint8_t d0; uint8_t d1; uint8_t d2; uint8_t d3; uint8_t d4; uint8_t d5;
    } bytes;
} ccsds_tm_primary_header_t;

typedef struct {
    uint8_t length : 6;
    uint8_t version : 2;
    uint8_t data[];
} ccsds_tm_secondary_header_head_t;
