#include "ant-lib/ccsds-tm-vc-send.h"

#include "string.h"
#include "stdlib.h"

#include "ant-lib/math-defs.h"

int CcsdsTmVcSend::VCP_request(uint8_t* data, uint32_t size)
{
    if (!_init_done) {
        EHAS_UP(CCSDS_TM_VC_RC_VcNotInit);
        return -CCSDS_TM_VC_RC_VcNotInit;
    }

    if (!data) {
        EHAS_UP(CCSDS_TM_VC_RC_VcNullPtr);
        return -CCSDS_TM_VC_RC_VcNullPtr;
    }

    return _packet_processing_add_packet(data, size);
}

int CcsdsTmVcSend::VC_FSH_request(uint8_t* data, uint16_t size)
{
    if (!_init_done) {
        EHAS_UP(CCSDS_TM_VC_RC_VcNotInit);
        return -CCSDS_TM_VC_RC_VcNotInit;
    }

    if (!VC_FSH_service_enable) {
        EHAS_UP(CCSDS_TM_VC_RC_VcFshNotEnable);
        return -CCSDS_TM_VC_RC_VcFshNotEnable;
    }

    if (!data) {
        EHAS_UP(CCSDS_TM_VC_RC_VcNullPtr);
        return -CCSDS_TM_VC_RC_VcNullPtr;
    }

    if (size > FSH) {
        size = FSH;
    }

    memcpy(_FSH_data, data, size);

    if (size < FSH) {
        memset(_FSH_data + size, 0, FSH - size);
    }

    return size;
}

int CcsdsTmVcSend::handle()
{
    if (!_init_done) {
        return -CCSDS_TM_VC_RC_VcNotInit;
    }

    if (_release_signal) {
        _packet_processing_release();
        _release_signal = false;
    }

    return CCSDS_TM_OK;
}

int CcsdsTmVcSend::_packet_processing_add_packet(uint8_t* data, uint32_t size)
{
    int frame_data_size = SDLS_enable ? _enc_data_size : _data_size;
    if (_first_header_ptr == CCSDS_TM_NO_FIRST_HEADER_PTR) {
        _first_header_ptr = _data_field_size;
    }

    uint32_t bytes_moved = 0;

    while (bytes_moved < size)
    {
        uint32_t df_size = ANT_MIN(size - bytes_moved, frame_data_size - _data_field_size);
        uint8_t* df_ptr = _data_field_buffer + _data_field_size;

        memcpy(df_ptr, data + bytes_moved, df_size);
        bytes_moved += df_size;
        _data_field_size += df_size;

        if (_data_field_size >= frame_data_size) {
            _virtual_channel_generation(_data_field_buffer, frame_data_size, _first_header_ptr);
            _data_field_size = 0;
            _first_header_ptr = CCSDS_TM_NO_FIRST_HEADER_PTR;
        }
    }

    return bytes_moved;
}

int CcsdsTmVcSend::_packet_processing_release()
{
    // There is no pending data
    if (_data_field_size == 0) {
        return 0;
    }

    uint16_t frame_data_size = SDLS_enable ? _enc_data_size : _data_size;

    if (_data_field_size > frame_data_size) {
        _data_field_size = 0;
        _first_header_ptr = CCSDS_TM_NO_FIRST_HEADER_PTR;
        EHAR_UP(CCSDS_TM_VC_RC_VcMathError);
        return -CCSDS_TM_VC_RC_VcMathError;
    }

    uint16_t idle_size = frame_data_size - _data_field_size;
    int rc = _ep->generate_idle_packet(_data_field_buffer + _data_field_size, idle_size);
    if (rc != idle_size) {
        EHAS_UP(CCSDS_TM_VC_RC_VcEppIdle);
        return -CCSDS_TM_VC_RC_VcEppIdle;
    }

    return _virtual_channel_generation(_data_field_buffer, frame_data_size, _first_header_ptr);
}

int CcsdsTmVcSend::_virtual_channel_generation(uint8_t* data, uint16_t size, uint16_t first_header_ptr)
{
    uint16_t frame_data_size = SDLS_enable ? _enc_data_size : _data_size;
    if (size != frame_data_size) {
        EHAR_UP(CCSDS_TM_VC_RC_VcSizeInval);
        return -CCSDS_TM_VC_RC_VcSizeInval;
    }

    uint8_t* new_frame = (uint8_t*)_q.reserve();
    if (!new_frame) {
        EHAS_UP(CCSDS_TM_VC_RC_VcQueueFull);
        return -CCSDS_TM_VC_RC_VcQueueFull;
    }

    ccsds_tm_primary_header_t pheader = {0};
    pheader.bf.TFVN     = CCSDS_TM_TFVN;
    pheader.bf.SCID     = SCID;
    pheader.bf.VCID     = VCID;
    pheader.bf.OCF_flag = 0;

    pheader.bf.MC_frame_count = 0; // Fills later
    pheader.bf.VC_frame_count = (_VC_frame_counter++) % 0xFF;

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

    // Insert VC_FSH if enabled
    if (VC_FSH_enable) {
        ccsds_tm_secondary_header_head_t* sheader = (ccsds_tm_secondary_header_head_t*)(new_frame + CCSDS_TM_PHEADER_SIZE);
        sheader->version = CCSDS_TM_FSH_VERSION;
        sheader->length = FSH;
        memcpy(sheader->data, _VC_FSH_buffer, FSH);
    }
    else {
        memset(sheader, 0, FSH + CCSDS_TM_SHEADER_HEAD_SIZE);
    }

    if (_encription) {
        int rc = _sdls->apply_security(data, size, new_frame + _data_offs, _data_size); // OUT buffer len equals _data_size not _enc_data_size
        if (rc != _data_size) {
            EHAS_UP(CCSDS_TM_VC_RC_VcSdlsError);
            return -CCSDS_TM_VC_RC_VcSdlsError;
        }
    }
    else {
        memcpy(new_frame + _data_offs, data, size);
    }

    return frame_data_size;
}

int CcsdsTmVcSend::get_VC_frame(uint8_t* buffer, uint16_t lenght)
{
    if (!_init_done) {
        EHAS_UP(CCSDS_TM_VC_RC_VcNotInit);
        return -CCSDS_TM_VC_RC_VcNotInit;
    }

    if (!buffer) {
        EHAS_UP(CCSDS_TM_VC_RC_VcNullPtr);
        return -CCSDS_TM_VC_RC_VcNullPtr;
    }

    if (_q.empty()) {
        _release_signal = true;
        return 0;
    }

    if (lenght < F) {
        EHAS_UP(CCSDS_TM_VC_RC_VcLenghtSmall);
        return -CCSDS_TM_VC_RC_VcLenghtSmall;
    }

    uint8_t* frame = (uint8_t*)_q.remove();

    if (!frame) {
        EHAS_UP(CCSDS_TM_VC_RC_VcQueueEmpty);
        return -CCSDS_TM_VC_RC_VcQueueEmpty;
    }

    memcpy(buffer, frame, F);
    return F;
}
