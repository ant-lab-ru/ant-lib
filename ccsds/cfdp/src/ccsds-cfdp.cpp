#include "ant-lib/ccsds-cfdp.h"

int CcsdsCfdp::put(const char* dst_name, const char* src_name, ccsds_cfdp_EID_t dst_entity, ccsds_cfdp_NAK_type_t NAK_type)
{
    // Try open file

    // Creating transaction ID
    int id = this->_transaction_start_notification_procedure();
    if (id < 0) {
        return -1;
    }

    return CCSDS_CFDP_OK;
}

int CcsdsCfdp::_transaction_start_notification_procedure()
{
    CcsdsCfdpTransaction* t = &(this->_transactions[0]);
    t->TSN = ++this->_last_TSN;
    t->state = CCSDS_CFDP_STATE_SEND_COPYING;
    return CCSDS_CFDP_OK;
}

int CcsdsCfdp::_copy_file_procedure_send_entity()
{
    
    return 0;
}

int CcsdsCfdp::handle()
{
    
}