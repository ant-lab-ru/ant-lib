#pragma once

#include "stdint.h"

#include "ant-lib/queue-static.h"

#include "ant-lib/ccsds-tm-types.h"
#include "ant-lib/ccsds-epp.h"

template<uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
class CcsdsTmVcSend
{
public:
    CcsdsTmVcSend(uint8_t* buffer, uint32_t buffer_lenght, CcsdsEpp* ep):
        _df_buffer(buffer),
        _ep(ep),

        _data_size    (F - CCSDS_TM_PHEADER_SIZE - CCSDS_TM_SHEADER_HEAD_SIZE - FSH                 ),
        _enc_data_size(F - CCSDS_TM_PHEADER_SIZE - CCSDS_TM_SHEADER_HEAD_SIZE - FSH - SDLSH - SDLST ),

        _data_offs    (CCSDS_TM_PHEADER_SIZE + CCSDS_TM_SHEADER_HEAD_SIZE + FSH           ),
        _enc_data_offs(CCSDS_TM_PHEADER_SIZE + CCSDS_TM_SHEADER_HEAD_SIZE + FSH + SDLSH   ),

        _q(buffer+_data_size, F, (buffer_lenght - _data_size)/F)
        {};

    int VCP_request(uint8_t* data, uint32_t size);
    int VC_FSH_request(uint8_t* data, uint32_t size);

    int get_frame(uint8_t* buffer, uint32_t lenght);
    int handle();

private:
    CcsdsEpp* _ep;

    uint8_t     _VCID;
    uint8_t     _vc_priority;
    uint16_t    _SCID;
    uint32_t    _vc_frame_counter;
    bool        _encription;
    bool        _frame_request;
    uint8_t     _FSH_data[FSH];

    int _packet_processing_add_packet(uint8_t* data, uint32_t size);
    int _packet_processing_release();
    uint16_t _current_first_header_ptr;
    uint16_t _df_buffer_current_size;
    uint8_t* _df_buffer;

    int _virtual_channel_generation(uint8_t* data, uint16_t size, uint16_t first_header_ptr);
    StaticQueue _q;

    const uint16_t _data_size;
    const uint16_t _enc_data_size;
    const uint16_t _data_offs;
    const uint16_t _enc_data_offs;
};
