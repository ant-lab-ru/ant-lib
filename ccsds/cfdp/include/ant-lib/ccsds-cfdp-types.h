#pragma once
#include "stdint.h"

//===================================================================
//                      Configurable types
//===================================================================

typedef uint16_t ccsds_cfdp_TSN_t;
typedef uint32_t ccsds_cfdp_EID_t;

typedef uint32_t ccsds_cfdp_UT_address_t;
typedef uint32_t ccsds_cfdp_FSS_t;


//===================================================================
//                      Return Codes
//===================================================================

enum
{
    CCSDS_CFDP_OK = 0,
    CCSDS_CFDP_RC_NullPtr,
    CCSDS_CFDP_RC_TrStateInval,

    CCSDS_CFDP_RC_NumberOfTypes,
};


//===================================================================
//                      General PDU types
//===================================================================

typedef struct
{
    uint8_t large_file_flag : 1;
    uint8_t CRC_flag : 1;
    uint8_t transmission_mode : 1;
    uint8_t direction : 1;
    uint8_t PDU_type : 1;
    uint8_t version : 3;

    uint16_t PDU_data_len;

    uint8_t len_of_TSN : 3;
    uint8_t segment_metadata_flag : 1;
    uint8_t len_of_entity_IDs : 3;
    uint8_t segmentation_control : 1;

    ccsds_cfdp_EID_t  source_entity_ID;
    ccsds_cfdp_TSN_t  transaction_sequence_number;
    ccsds_cfdp_EID_t  destination_entity_ID;
} ccsds_cfdp_pdu_header_t;

typedef struct
{
    uint8_t lenght;
    uint8_t value[];
} ccsds_cfdp_LV_object_t;

typedef struct
{
    ccsds_cfdp_directives_t type;
    uint8_t lenght;
    uint8_t value[];
} ccsds_cfdp_TLV_object_t;

//===================================================================
//                      File Directive PDU
//===================================================================

typedef enum : uint8_t
{
    CCSDS_CFDP_DIRECTIVES_EOF_PDU = 0x04,
    CCSDS_CFDP_DIRECTIVES_FINISHED_PDU = 0x05,
    CCSDS_CFDP_DIRECTIVES_ACK_PDU = 0x06,
    CCSDS_CFDP_DIRECTIVES_METADATA_PDU = 0x07,
    CCSDS_CFDP_DIRECTIVES_NAK_PDU = 0x08,
    CCSDS_CFDP_DIRECTIVES_PROMPT_PDU = 0x09,
    CCSDS_CFDP_DIRECTIVES_KEEP_ALIVE_PDU = 0x0C,
} ccsds_cfdp_directives_t;

typedef enum : uint8_t
{
    CCSDS_CFDP_CCODE_NO_ERROR = 0,
    CCSDS_CFDP_CCODE_POSITIVE_ACK_LIMIT_REACHED,
    CCSDS_CFDP_CCODE_KEEP_ALIVE_LIMIT_REACHED,
    CCSDS_CFDP_CCODE_INVALID_TRANSMISSION_MODE,
    CCSDS_CFDP_CCODE_FILESTORE_REJECTION,
    CCSDS_CFDP_CCODE_FILE_CHECKSUM_FAILURE,
    CCSDS_CFDP_CCODE_FILE_SIZE_ERROR,
    CCSDS_CFDP_CCODE_NAK_LIMIT_REACHED,
    CCSDS_CFDP_CCODE_INACTIVITY_DETECTED,
    CCSDS_CFDP_CCODE_INVALID_FILE_STRUCTURE,
    CCSDS_CFDP_CCODE_CHECK_LIMIT_REACHED,
    CCSDS_CFDP_CCODE_UNSUPPORTED_CHECKSUM_TYPE,
    CCSDS_CFDP_CCODE_RES0,
    CCSDS_CFDP_CCODE_RES1,
    CCSDS_CFDP_CCODE_SUSPEND_REQUEST_RECEIVED,
    CCSDS_CFDP_CCODE_CANCEL_REQUEST_RECEIVED,
} ccsds_cfdp_ccode_t;

// EOF PDU
typedef struct
{
    uint8_t spare : 4;
    uint8_t ccode : 4;

    uint32_t file_checksum;
    ccsds_cfdp_FSS_t file_size;
} ccsds_cfdp_EOF_PDU_t;

// FINISHED PDU
typedef struct
{
    uint8_t file_status : 2;
    uint8_t delivery_code : 1;
    uint8_t spare : 1;
    uint8_t ccode : 4;
} ccsds_cfdp_FINISHED_PDU_t;

// ACK PDU
typedef struct
{
    uint8_t directive_subtype_code : 4;
    uint8_t directive_code : 4;

    uint8_t transaction_status : 2;
    uint8_t spare : 2;
    uint8_t ccode : 4;
} ccsds_cfdp_ACK_PDU_t;

// METADATA PDU
typedef struct
{
    uint8_t checksum_type : 4;
    uint8_t reserved1 : 2;
    uint8_t closure_requested : 1;
    uint8_t reserved0 : 1;

    ccsds_cfdp_FSS_t file_size;
} ccsds_cfdp_METADATA_PDU_t;


// NAK PDU
typedef struct 
{
    ccsds_cfdp_FSS_t start_offset;
    ccsds_cfdp_FSS_t end_offset;
} ccsds_cfdp_NAK_Segment_PDU_t;
typedef struct
{
    ccsds_cfdp_FSS_t start_of_scope;
    ccsds_cfdp_FSS_t end_of_scope;
} ccsds_cfdp_NAK_PDU_t;

// PROMPT PDU
typedef struct 
{
    uint8_t response_required : 1;
    uint8_t spare : 7;
} ccsds_cfdp_PROMPT_PDU_t;

// KEEP ALIVE PDU
typedef struct 
{
    ccsds_cfdp_FSS_t progress;
} ccsds_cfdp_KEEP_ALIVE_PDU_t;


//===================================================================
//                      TLV PARAMETERS
//===================================================================

// FILESTORE REQUEST TLV
typedef struct 
{
    uint8_t spare : 4;
    uint8_t action_code : 4;
} ccsds_cfdp_FILESTORE_REQUEST_TLV_t;

// FILESTORE RESPONSE TLV
typedef struct 
{
    uint8_t status_code : 4;
    uint8_t action_code : 4;
} ccsds_cfdp_FILESTORE_RESPONSE_TLV_t;

//===================================================================
//                      File Data PDU
//===================================================================

typedef struct
{
    uint8_t segment_metadata_len : 6;
    uint8_t record_continuation_state : 2;
    uint8_t segment_metadata[];
} ccsds_cfdp_data_PDU_half0_t;

typedef struct
{
    ccsds_cfdp_FSS_t offset;
    uint8_t file_data[];
} ccsds_cfdp_data_PDU_half1_t;

//===================================================================
//                      MIB
//===================================================================

typedef struct
{
    ccsds_cfdp_EID_t id;
    bool EOF_sent;
    bool EOF_recv;
    bool file_segment_recv;
    bool transaction_finished;
    bool suspended;
    bool resumed;
} ccsds_cfdp_local_entity_cfg_info_t;

typedef struct
{
    ccsds_cfdp_EID_t id;
    uint8_t PVN;
    ccsds_cfdp_UT_address_t UT_addr;
    uint32_t positive_ACK_timer_interval;
    uint32_t NAK_timer_interval;
    uint32_t keep_alive_interval;
    bool immediate_NAK_mode_enabled;
    bool default_transmission_mode;
    bool transaction_closure_requested;
    uint32_t check_limit;
    uint8_t default_type_of_checksum;
    bool retain_disposition_of_incomplete_rcv_file;
    bool CRCs_required_on_transmission;
    uint16_t maximum_file_segment_length;
    uint32_t keep_alive_discrepancy_limit;
    uint32_t positive_ACK_timer_expiration_limit;
    uint32_t NAK_timer_expiration_limit;
    uint32_t transaction_inactivity_limit;
} ccsds_cfdp_remote_entity_cfg_info_t;