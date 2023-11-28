#include "ant-lib/ccsds-cfdp-transaction.h"

int CcsdsCfdpTransaction::start(ccsds_cfdp_TSN_t TSN, const char* dst_filename, const char* src_filename)
{
    if (!this->is_ready()) {
        return -1;
    }

    // FIXME File Open
    this->_src_filename = src_filename;
    this->_dst_filename = dst_filename;
    this->_file_id = 0;
    this->_file_checksum = 0;
    this->_file_size = 0;

    // Src file opened succesfully
    this->_state = CCSDS_CFDP_STATE_SEND_COPYING;
    this->_TSN = TSN;
    this->_last_offset = 0;
    this->_send_type = CCSDS_CFDP_SEND_TYPE_METADATA;

    return CCSDS_CFDP_OK;
}

int CcsdsCfdpTransaction::cancel();

int CcsdsCfdpTransaction::handle()
{
    switch (this->_state)
    {
    case CCSDS_CFDP_STATE_IDLE:
        return CCSDS_CFDP_OK;

    case CCSDS_CFDP_STATE_SEND_METADATA: {
        int rc = this->_send_metadata_PDU();
        this->_state = CCSDS_CFDP_STATE_SEND_DATA;
    } break;

    case CCSDS_CFDP_STATE_SEND_DATA: {
        int rc = this->_send_file_data_PDU();
        this->_state = CCSDS_CFDP_STATE_SEND_EOF;
     } break;

    case CCSDS_CFDP_STATE_SEND_EOF:
        int rc = this->_send_EOF_PDU();
        this->_state = CCSDS_CFDP_STATE_SEND_ACK_WAIT;
        break;

    case CCSDS_CFDP_STATE_SEND_ACK_WAIT:
        this->_state = CCSDS_CFDP_STATE_SEND_FINISHED_WAIT;
        break;

    case CCSDS_CFDP_STATE_SEND_FINISHED_WAIT:
        this->_state = CCSDS_CFDP_STATE_IDLE;
        break;

    default:
        EHAS_RISE(CCSDS_CFDP_RC_TrStateInval);
        this->cancel();
        break;
    }

    return CCSDS_CFDP_OK;
}

bool CcsdsCfdpTransaction::is_ready() {
    return (this->_state == CCSDS_CFDP_STATE_IDLE);
}
