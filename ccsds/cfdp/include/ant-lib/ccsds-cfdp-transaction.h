#pragma once
#include "stdint.h"

#include "ccsds-cfdp-types.h"

typedef enum : uint8_t
{
    CCSDS_CFDP_STATE_IDLE = 0,
    CCSDS_CFDP_STATE_SEND_METADATA,
    CCSDS_CFDP_STATE_SEND_DATA,
    CCSDS_CFDP_STATE_SEND_EOF,
    CCSDS_CFDP_STATE_SEND_ACK_WAIT,
    CCSDS_CFDP_STATE_SEND_FINISHED_WAIT,
} ccsds_cfdp_state_t;

class CcsdsCfdpTransaction
{
    public:
        CcsdsCfdpTransaction(){};

        int start(ccsds_cfdp_TSN_t TSN, const char* dst_filename, const char* src_filename);
        int cancel();
        bool is_ready();

        int handle();

    private:
        ccsds_cfdp_TSN_t _TSN;
        ccsds_cfdp_state_t _state;

        ccsds_cfdp_FSS_t _last_offset;

        const char* _src_filename;
        const char* _dst_filename;
        int _file_id;
        ccsds_cfdp_FSS_t _file_size;
        uint32_t _file_checksum;

        int _send_metadata_PDU();
        int _send_file_data_PDU();
        int _send_EOF_PDU();
};