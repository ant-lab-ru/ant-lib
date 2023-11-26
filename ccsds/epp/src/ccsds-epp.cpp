#include "ant-lib/ccsds-epp.h"

int CcsdsEpp::encapsulate(uint8_t* data, uint32_t size, uint8_t* buffer, uint32_t lenght)
{
    if (!data || !buffer) {
        EHAS_RETURN(CCSDS_EPP_ERROR_NullPtr);
    }

    if (size > ccsds_epp_max_data_len[this->_lenoflen]) {
        EHAS_RETURN(CCSDS_EPP_ERROR_SizeTooBig);
    }

    uint32_t packet_size = size + ccsds_epp_header_len[this->_lenoflen];
    if (lenght < packet_size) {
        EHAS_RETURN(CCSDS_EPP_ERROR_LenghtTooSmall);
    }

    ccsds_epp_common_header_t common_header = {0};
    common_header.bf.pvn =  CCSDS_EPP_PVN;
    common_header.bf.epid = CCSDS_EPP_EPID;
    common_header.bf.lenoflen = this->_lenoflen;
    memcpy(buffer, &(common_header.byte), sizeof(uint8_t));

    uint8_t* header_ptr = buffer + sizeof(uint8_t);
    switch (this->_lenoflen)
    {
        case CCSDS_EPP_LENOFLEN_01: {
            ccsds_epp_header01_t header = {0};
            header.size = packet_size;
            memcpy(header_ptr, &header, sizeof(header));
        } break;
    
        case CCSDS_EPP_LENOFLEN_10: {
            ccsds_epp_header10_t header = {0};
            header.size = packet_size;
            header.udf_epidext = 0;
            memcpy(header_ptr, &header, sizeof(header));
        } break;

        case CCSDS_EPP_LENOFLEN_11: {
            ccsds_epp_header11_t header = {0};
            header.size = packet_size;
            header.udf_epidext = 0;
            header.cdf = 0;
            memcpy(header_ptr, &header, sizeof(header));
        } break;

        default:
            break;
    }

    uint8_t* data_field_ptr = buffer + ccsds_epp_header_len[this->_lenoflen];
    memcpy(data_field_ptr, data, size);
    return packet_size;
}

int CcsdsEpp::deencapsulate(uint8_t* data, uint32_t size, uint8_t* buffer, uint32_t lenght)
{
    if (!data || !buffer) {
        EHAS_RETURN(CCSDS_EPP_ERROR_NullPtr);
    }

    if (size < sizeof(uint8_t)) {
        EHAS_RETURN(CCSDS_EPP_ERROR_SizeTooSmall);
    }

    ccsds_epp_common_header_t common_header = {0};
    memcpy(&(common_header.byte), data, sizeof(uint8_t));
    if (common_header.bf.pvn !=  CCSDS_EPP_PVN) {
        EHAS_RETURN(CCSDS_EPP_ERROR_InvalPVN);
    }
    if (common_header.bf.epid != CCSDS_EPP_EPID) {
        EHAS_RETURN(CCSDS_EPP_ERROR_InvalEPID);
    }
    ccsds_epp_lenoflen_t lenoflen = (ccsds_epp_lenoflen_t)common_header.bf.lenoflen;
    
    if (size < ccsds_epp_header_len[lenoflen]) {
        EHAS_RETURN(CCSDS_EPP_ERROR_SizeTooSmall);
    }

    uint32_t deenc_size = 0;
    uint8_t* header_ptr = data + sizeof(uint8_t);
    switch (lenoflen)
    {
    case CCSDS_EPP_LENOFLEN_00: {
        deenc_size = sizeof(uint8_t);
    } break;

    case CCSDS_EPP_LENOFLEN_01: {
        ccsds_epp_header01_t header = {0};
        memcpy(&header, header_ptr, sizeof(header));
        deenc_size = header.size;
    } break;

    case CCSDS_EPP_LENOFLEN_10: {
        ccsds_epp_header10_t header = {0};
        memcpy(&header, header_ptr, sizeof(header));
        deenc_size = header.size;
    } break;

    case CCSDS_EPP_LENOFLEN_11: {
        ccsds_epp_header11_t header = {0};
        memcpy(&header, header_ptr, sizeof(header));
        deenc_size = header.size;
    } break;
    
    default:
        EHAS_RETURN(CCSDS_EPP_ERROR_InvalLenOfLen);
        break;
    }

    uint32_t expected_packet_size = deenc_size + ccsds_epp_header_len[lenoflen];
    if (size != expected_packet_size) {
        EHAS_RETURN(CCSDS_EPP_ERROR_SizeInvalid);
    }

    uint8_t* data_ptr = data + ccsds_epp_header_len[lenoflen];
    memcpy(buffer, data_ptr, deenc_size);
    return deenc_size;
}
