#pragma once
#include "ccsds-cfdp-types.h"
#include "ccsds-cfdp-transaction.h"



typedef enum : uint8_t
{
    CCSDS_CFDP_NAK_Immediate = 1,
} ccsds_cfdp_NAK_type_t;

class CcsdsCfdp
{
    public:
        CcsdsCfdp(){}

        int init();
        int handle();

        int put(const char* dst_name, const char* src_name, ccsds_cfdp_EID_t dst_entity, ccsds_cfdp_NAK_type_t NAK_type);
        int report();
        int cancel();
        int suspend();
        int resume();

    private:
        CcsdsCfdpTransaction _transactions[2];
        ccsds_cfdp_TSN_t _last_TSN;

        int _transaction_start_notification_procedure();
        int _copy_file_procedure_send_entity();

        int _checksum_procedure();
        int _PDU_forwarding_procedure();
        int _acknowledged_mode_procedure_send_entity();
        int _incremental_lost_segment_detection_send_entity();
        int _keep_alive_procedure_send_entity();
        int _cancel_response_procedure_send_entity();
        int _resume_procedure();
        int _report_procedure();
        int _positive_ack_procedure_PDU_sending_end();
        int _positive_ack_procedure_PDU_receiving_end();
        int _fault_handling_procedure();
        int _filestore_procedure();
        int _inactivity_monitor_procedure();
        int _internal_procedure();
        int _link_state_change_procedure();

        int _send_KEEP_ALIVE_PDU(ccsds_cfdp_FSS_t progress);
        
};
