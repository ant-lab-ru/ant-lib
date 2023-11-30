#include "ant-lib/ccsds-epp.h"

int CcsdsEpp::encapsulate(uint8_t* data, uint32_t size, ccsds_epp_packet_type_t type, uint8_t* buffer, uint32_t lenght)
{
    if (!buffer || !data) {
        EHAS_RETURN(CCSDS_EPP_ERROR_NullPtr);
    }

    uint32_t packet_size = 0;
    ccsds_epp_common_header_t common_header = {0};
    common_header.bf.pvn =  CCSDS_EPP_PVN;
    common_header.bf.epid = type;

    if (size <= ccsds_epp_max_data_len[CCSDS_EPP_LENOFLEN_01])
    {
        common_header.bf.lenoflen = CCSDS_EPP_LENOFLEN_01;
        packet_size = size + ccsds_epp_header_len[common_header.bf.lenoflen];
    
        if (lenght < packet_size) {
            EHAS_RETURN(CCSDS_EPP_ERROR_LenghtTooSmall);
        }

        memcpy(&buffer[0], &(common_header.byte), sizeof(uint8_t));
        ccsds_epp_header01_t header = {0};
        header.size = packet_size;
        memcpy(&buffer[1], &header, sizeof(header));
    }
    else if (size <= ccsds_epp_max_data_len[CCSDS_EPP_LENOFLEN_10])
    {
        common_header.bf.lenoflen = CCSDS_EPP_LENOFLEN_10;
        packet_size = size + ccsds_epp_header_len[common_header.bf.lenoflen];

        if (lenght < packet_size) {
            EHAS_RETURN(CCSDS_EPP_ERROR_LenghtTooSmall);
        }
    
        memcpy(&buffer[0], &(common_header.byte), sizeof(uint8_t));
        ccsds_epp_header10_t header = {0};
        header.size = packet_size;
        header.udf_epidext = 0;
        memcpy(&buffer[1], &header, sizeof(header));
    }
    else if (size <= ccsds_epp_max_data_len[CCSDS_EPP_LENOFLEN_11])
    {
        common_header.bf.lenoflen = CCSDS_EPP_LENOFLEN_11;
        packet_size = size + ccsds_epp_header_len[common_header.bf.lenoflen];

        if (lenght < packet_size) {
            EHAS_RETURN(CCSDS_EPP_ERROR_LenghtTooSmall);
        }
    
        memcpy(&buffer[0], &(common_header.byte), sizeof(uint8_t));
        ccsds_epp_header11_t header = {0};
        header.size = packet_size;
        header.udf_epidext = 0;
        header.cdf = 0;
        memcpy(&buffer[1], &header, sizeof(header));
    }
    else {
        EHAS_RETURN(CCSDS_EPP_ERROR_SizeTooBig);
    }

    uint8_t* data_field_ptr = buffer + ccsds_epp_header_len[common_header.bf.lenoflen];
    memcpy(data_field_ptr, data, size);

    return packet_size;
}

int CcsdsEpp::deencapsulate(uint8_t* data, uint32_t size, ccsds_epp_packet_type_t* type, uint8_t* buffer, uint32_t lenght)
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

    *type = (ccsds_epp_packet_type_t)common_header.bf.epid;
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

int CcsdsEpp::generate_idle_packet(uint8_t* buffer, uint32_t size)
{
    if (size == 0) {
        return 0;
    }

    if (!buffer) {
        EHAS_RETURN(CCSDS_EPP_ERROR_NullPtr);
    }

    ccsds_epp_common_header_t common_header = {0};
    common_header.bf.pvn =  CCSDS_EPP_PVN;
    common_header.bf.epid = CCSDS_EPP_IDLE;

    if (size <= ccsds_epp_max_packet_len[CCSDS_EPP_LENOFLEN_00]) {
        common_header.bf.lenoflen = CCSDS_EPP_LENOFLEN_00;
        buffer[0] = common_header.byte;
        return size;
    }

    if (size <= ccsds_epp_max_packet_len[CCSDS_EPP_LENOFLEN_01]) {
        common_header.bf.lenoflen = CCSDS_EPP_LENOFLEN_01;
        buffer[0] = common_header.byte;
        ccsds_epp_header01_t header = {0};
        header.size = size;
        memcpy(&buffer[1], &header, sizeof(header));

        uint32_t data_size = size - ccsds_epp_header_len[CCSDS_EPP_LENOFLEN_01];
        memset(buffer + ccsds_epp_header_len[CCSDS_EPP_LENOFLEN_01], 0, data_size);
        return size;
    }

    if (size <= ccsds_epp_max_packet_len[CCSDS_EPP_LENOFLEN_10]) {
        common_header.bf.lenoflen = CCSDS_EPP_LENOFLEN_10;
        buffer[0] = common_header.byte;
        ccsds_epp_header10_t header = {0};
        header.size = size;
        header.udf_epidext = 0;
        memcpy(&buffer[1], &header, sizeof(header));

        uint32_t data_size = size - ccsds_epp_header_len[CCSDS_EPP_LENOFLEN_10];
        memset(buffer + ccsds_epp_header_len[CCSDS_EPP_LENOFLEN_01], 0, data_size);
        return size;
    }

    if (size <= ccsds_epp_max_packet_len[CCSDS_EPP_LENOFLEN_11]) {
        common_header.bf.lenoflen = CCSDS_EPP_LENOFLEN_11;
        buffer[0] = common_header.byte;
        ccsds_epp_header11_t header = {0};
        header.size = size;
        header.udf_epidext = 0;
        memcpy(&buffer[1], &header, sizeof(header));

        uint32_t data_size = size - ccsds_epp_header_len[CCSDS_EPP_LENOFLEN_11];
        memset(buffer + ccsds_epp_header_len[CCSDS_EPP_LENOFLEN_01], 0, data_size);
        return size;
    }

    EHAS_RETURN(CCSDS_EPP_ERROR_SizeTooBig);
}