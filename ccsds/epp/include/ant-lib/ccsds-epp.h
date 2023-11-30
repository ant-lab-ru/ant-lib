#pragma once

#include "stdint.h"
#include "ant-lib/ehas.h"

constexpr uint8_t CCSDS_EPP_PVN = 7;
constexpr uint8_t CCSDS_EPP_EPID = 0;

typedef enum: uint8_t
{
    CCSDS_EPP_IDLE            = 0,
    CCSDS_EPP_LTP             = 1,
    CCSDS_EPP_IPE             = 2,
    CCSDS_EPP_CFDP            = 3,
    CCSDS_EPP_BP              = 4,
    CCSDS_EPP_NoEntry         = 5,
    CCSDS_EPP_EXTENDED        = 6,
    CCSDS_EPP_MissionSpecific = 7,
} ccsds_epp_packet_type_t;

enum
{
    CCSDS_EPP_OK = 0,
    CCSDS_EPP_ERROR_NullPtr,
    CCSDS_EPP_ERROR_SizeTooBig,
    CCSDS_EPP_ERROR_SizeTooSmall,
    CCSDS_EPP_ERROR_SizeInvalid,
    CCSDS_EPP_ERROR_LenghtTooSmall,
    CCSDS_EPP_ERROR_InvalPVN,
    CCSDS_EPP_ERROR_InvalEPID,
    CCSDS_EPP_ERROR_InvalLenOfLen,
    CCSDS_EPP_NumberOfTypes,
};

typedef enum
{
    CCSDS_EPP_LENOFLEN_00 = 0,
    CCSDS_EPP_LENOFLEN_01 = 1,
    CCSDS_EPP_LENOFLEN_10 = 2,
    CCSDS_EPP_LENOFLEN_11 = 3,
    CCSDS_EPP_LENOFLEN_NumberOfTypes,
} ccsds_epp_lenoflen_t;

constexpr uint8_t  ccsds_epp_header_len[CCSDS_EPP_LENOFLEN_NumberOfTypes]     = { 1, 2, 4, 8 };
constexpr uint32_t ccsds_epp_max_data_len[CCSDS_EPP_LENOFLEN_NumberOfTypes]   = { 0, 0xFD, 0xFFFB, 0xFFFFFFF7 };
constexpr uint32_t ccsds_epp_max_packet_len[CCSDS_EPP_LENOFLEN_NumberOfTypes] = { 1, 0xFF, 0xFFFF, 0xFFFFFFFF };

typedef struct
{
    uint8_t lenoflen : 2;
    uint8_t epid: 3;
    uint8_t pvn: 3;
} ccsds_epp_common_header_bf_t;

typedef union
{
    ccsds_epp_common_header_bf_t bf;
    uint8_t byte;
} ccsds_epp_common_header_t;

typedef struct
{
    uint8_t size;
} ccsds_epp_header01_t;

typedef struct
{
    uint8_t udf_epidext;
    uint16_t size;
} ccsds_epp_header10_t;

typedef struct
{
    uint8_t udf_epidext;
    uint16_t cdf;
    uint32_t size;
} ccsds_epp_header11_t;

class CcsdsEpp
{
    public:
        CcsdsEpp() {
                EHAS_INIT_NOINIT_DEVICE("CcsdsEpp");
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, NullPtr,        EHAS_ERROR);
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, SizeTooBig,     EHAS_ERROR);
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, SizeTooSmall,   EHAS_ERROR);
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, SizeInvalid,    EHAS_ERROR);
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, LenghtTooSmall, EHAS_ERROR);
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, InvalPVN,       EHAS_ERROR);
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, InvalEPID,      EHAS_ERROR);
                EHAS_INIT_PACK(CCSDS_EPP_ERROR, InvalLenOfLen,  EHAS_ERROR);
            };

        int encapsulate(uint8_t* data, uint32_t size, ccsds_epp_packet_type_t type, uint8_t* buffer, uint32_t lenght);
        int deencapsulate(uint8_t* data, uint32_t size, ccsds_epp_packet_type_t* type, uint8_t* buffer, uint32_t lenght);
        int generate_idle_packet(uint8_t* buffer, uint32_t size);

    private:
        EhasPack<CCSDS_EPP_NumberOfTypes> ehas;
};
