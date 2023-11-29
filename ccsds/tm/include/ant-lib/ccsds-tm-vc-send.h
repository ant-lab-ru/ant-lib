#pragma once

#include "stdint.h"

#include "ant-lib/queue-static.h"

#include "ant-lib/ccsds-tm-types.h"

class CcsdsTmVcSend
{
public:
    CcsdsTmVcSend(uint8_t* buffer, uint32_t buffer_lenght, uint16_t frame_size, uint16_t FSH_size, uint16_t SDLS_size):
        _df_buffer(buffer),
        _frame_size(frame_size),
        _q(buffer+frame_size, frame_size, buffer_lenght/frame_size - 1)
        {};

    int VCP_request(uint8_t* data, uint32_t size);

    int get_frame(uint8_t* buffer, uint32_t lenght);
    int handle();

private:

    uint8_t     _VCID;
    uint8_t     _vc_priority;
    uint16_t    _SCID;
    uint32_t    _vc_frame_counter;
    bool        _encription;
    bool        _frame_request;

    int _packet_processing(uint8_t* data, uint32_t size);
    uint16_t _current_first_header_ptr;

    int _virtual_channel_generation(uint16_t first_header_ptr);

    uint16_t        _df_buffer_current_size;
    uint8_t*        _df_buffer;

    StaticQueue     _q;

    const uint16_t  _frame_size;
    const uint16_t _data_size;
    const uint16_t _data_offs;
    const uint16_t _enc_data_size;
    const uint16_t _enc_data_offs;
};
