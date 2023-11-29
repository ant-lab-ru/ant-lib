#include "ant-lib/ccsds-tm-vc-send.h"

#include "string.h"

#include "ant-lib/math-defs.h"

int CcsdsTmVcSend::VCP_request(uint8_t* data, uint32_t size)
{
    return _packet_processing(data, size);
}

int CcsdsTmVcSend::handle()
{
    if (_frame_request) {
        _virtual_channel_generation(_current_first_header_ptr);
        _frame_request = false;
    }

    return CCSDS_TM_OK;
}

int CcsdsTmVcSend::_packet_processing(uint8_t* data, uint32_t size)
{
    int frame_max_data_size = _encription ? _enc_data_size : _data_size;
    _current_first_header_ptr = _df_buffer_current_size; // ERROR FIXME

    uint32_t bytes_moved = 0;

    while (bytes_moved < size)
    {
        uint32_t df_max_size = ANT_MIN(_df_buffer_current_size, frame_max_data_size);
        uint32_t df_size = ANT_MIN(size - bytes_moved, df_max_size);
        uint8_t* df_ptr = _df_buffer + _df_buffer_current_size;

        memcpy(df_ptr, data + bytes_moved, df_size);
        bytes_moved += df_size;
        _df_buffer_current_size += df_size;

        if (_df_buffer_current_size >= frame_max_data_size) {
            _virtual_channel_generation(_current_first_header_ptr);
            _df_buffer_current_size = 0;
            _current_first_header_ptr = CCSDS_TM_NO_FIRST_HEADER_PTR;
        }
    }

    return bytes_moved;
}

int CcsdsTmVcSend::_virtual_channel_generation(uint16_t first_header_ptr)
{
    uint8_t* new_frame = (uint8_t*)_q.reserve();

    sdlp_tm_primary_header_t pheader = {0};
    pheader.bf.TFVN     = CCSDS_TM_TFVN;
    pheader.bf.SCID     = _SCID;
    pheader.bf.VCID     = _VCID;
    pheader.bf.OCF_flag = 0;

    pheader.bf.MC_frame_count = 0; // Fills later
    pheader.bf.VC_frame_count = _vc_frame_counter % 0xFF;

    pheader.bf.secondary_header_flag = CCSDS_TM_SECONDARY_HEADER_FLAG;
    pheader.bf.sync_flag             = CCSDS_TM_SYNC_FLAG;
    pheader.bf.packet_order_flag     = CCSDS_TM_PACKET_ORDER_FLAG;
    pheader.bf.segment_length_id     = CCSDS_TM_SEGMENT_LENGHT_ID;
    pheader.bf.first_header_ptr      = first_header_ptr;

    new_frame[0] = pheader.bytes.d1; // |-> fliped for correct bit fields order
    new_frame[1] = pheader.bytes.d0; // |
    new_frame[2] = pheader.bytes.d2;
    new_frame[3] = pheader.bytes.d3;
    new_frame[4] = pheader.bytes.d5; // |-> fliped for correct bit fields order
    new_frame[5] = pheader.bytes.d4; // |

    // Insert VC_FSH

    // SDLS encription

    return CCSDS_TM_OK;
}

int CcsdsTmVcSend::get_frame(uint8_t* buffer, uint32_t lenght)
{
    if (_q.empty()) {
        _frame_request = true;
        return 0;
    }

    uint8_t* frame = (uint8_t*)_q.remove();

    if (frame) {
        memcpy(buffer, frame, _frame_size);
    }

    return _frame_size;
}
