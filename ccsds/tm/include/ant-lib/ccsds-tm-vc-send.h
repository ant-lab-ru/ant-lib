#pragma once

#include "stdint.h"

#include "ant-lib/queue-static.h"
#include "ant-lib/ehas.h"

#include "ant-lib/ccsds-tm-types.h"
#include "ant-lib/ccsds-epp.h"
#include "ant-lib/ccsds-sdls.h"

enum
{
    CCSDS_TM_VC_OK = 0,
    CCSDS_TM_VC_RC_VcNullPtr,
    CCSDS_TM_VC_RC_VcNotInit,
    CCSDS_TM_VC_RC_VcFshNotEnable,
    CCSDS_TM_VC_RC_VcMathError,
    CCSDS_TM_VC_RC_VcEppIdle,
    CCSDS_TM_VC_RC_VcQueueFull,
    CCSDS_TM_VC_RC_VcQueueEmpty,
    CCSDS_TM_VC_RC_VcSizeInval,
    CCSDS_TM_VC_RC_VcLenghtSmall,
    CCSDS_TM_VC_RC_VcSdlsError,
    CCSDS_TM_VC_RC_NumberOfTypes,
};

/**
 * @brief CcsdsTmVcSend class
 * 
 * @tparam F - frame size;
 * @tparam FSH - frame secondary header data size;
 * @tparam SDLSH - frame SDLS header size (used if SDLS_enable == true)
 * @tparam SDLST - frame SDLS trailer size (used if SDLS_enable == true)
 */

template<uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
class CcsdsTmVcSend
{
public:
    CcsdsTmVcSend(const char* ehas_name):
        _data_size    (F - CCSDS_TM_PHEADER_SIZE - CCSDS_TM_SHEADER_HEAD_SIZE - FSH                 ),
        _enc_data_size(F - CCSDS_TM_PHEADER_SIZE - CCSDS_TM_SHEADER_HEAD_SIZE - FSH - SDLSH - SDLST ),

        _data_offs    (CCSDS_TM_PHEADER_SIZE + CCSDS_TM_SHEADER_HEAD_SIZE + FSH           ),
        _enc_data_offs(CCSDS_TM_PHEADER_SIZE + CCSDS_TM_SHEADER_HEAD_SIZE + FSH + SDLSH   )
        {
            EHAS_INIT(ehas_name);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcNullPtr,      EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcNotInit,      EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcFshNotEnable, EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcMathError,    EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcEppIdle,      EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcQueueFull,    EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcQueueEmpty,   EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcSizeInval,    EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcLenghtSmall,  EHAS_ERROR);
            EHAS_INIT_RC(CCSDS_TM_VC, RC_VcSdlsError,    EHAS_ERROR);
        };

    // Services
    int VCP_request(uint8_t* data, uint32_t size);
    int VC_FSH_request(uint8_t* data, uint16_t size);

    // Parameters
    bool VC_FSH_enable = false;
    bool SDLS_enable   = false;
    uint8_t  VCID      = 0;
    uint16_t SCID      = 0;

    // Internal interfaces
    int get_VC_frame(uint8_t* buffer, uint16_t lenght);

    // Internal methods
    int handle();
    int init(ccsds_tm_vc_send_init_struct_t* init_struct);

    // EHAS
    EHAS_PACK(CCSDS_TM_VC_RC_NumberOfTypes);

private:
    // Buffers
    uint16_t    _first_header_ptr   = CCSDS_TM_NO_FIRST_HEADER_PTR;
    uint8_t     _VC_FSH_buffer[FSH] = {0};
    uint8_t*    _data_field_buffer;
    StaticQueue _q;

    // Private Methods and fields
    int _packet_processing_add_packet(uint8_t* data, uint32_t size);
    int _packet_processing_release();
    uint16_t _data_field_size = 0;

    int _virtual_channel_generation(uint8_t* data, uint16_t size, uint16_t first_header_ptr);

    // Private
    CcsdsEpp*  _ep;
    CcsdsSdls* _sdls;
    bool       _init_done        = false;
    bool       _release_signal   = false;
    uint32_t   _VC_frame_counter = 0;

    // Frame info
    const uint16_t _data_size;
    const uint16_t _data_offs;
    const uint16_t _enc_data_size;
    const uint16_t _enc_data_offs;
};
