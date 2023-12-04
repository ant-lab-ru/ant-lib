#include "ant-lib/ccsds-tm-mc-send.h"

template<uint8_t VCC, uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
int CcsdsTmMcSend<VCC,F,FSH,SDLSH,SDLST>::get_MC_frame(uint8_t* buffer, uint16_t lenght)
{
    return _master_channel_generation(buffer, lenght);
}

template<uint8_t VCC, uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
int CcsdsTmMcSend<VCC,F,FSH,SDLSH,SDLST>::_master_channel_generation(uint8_t* buffer, uint16_t lenght)
{
    if (lenght < F) {
        return -4;
    }

    int rc = _virtual_channel_multiplexing(buffer, lenght);
    if (rc < 0 || rc >= VCC) {
        return -5;
    }

    ccsds_tm_primary_header_t* pheader = (ccsds_tm_primary_header_t*)buffer;
    pheader->bf.MC_frame_count = (_MC_frame_counter++) % 0xFF;

    if (MC_FSH_enable && !(this->vc[rc].VC_FSH_enable)) {
        ccsds_tm_secondary_header_head_t* sheader = (ccsds_tm_secondary_header_head_t*)(buffer + CCSDS_TM_PHEADER_SIZE);
        sheader->version = CCSDS_TM_FSH_VERSION;
        sheader->length = FSH;
        memcpy(sheader->data, _MC_FSH_buffer, FSH);
    }

    return F;
}

template<uint8_t VCC, uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
int CcsdsTmMcSend<VCC,F,FSH,SDLSH,SDLST>::_virtual_channel_multiplexing(uint8_t* buffer, uint16_t lenght)
{
    int rc = 0;
    int idx = 0;

    for (int i = 0; i < VCC; i++) {
        idx = _scheduler->get_index();

        rc = vc[idx].get_VC_frame(buffer, lenght);

        if (rc == 0) {
            _scheduler->index_empty();
        }

        if (rc < 0) {
            return -14;
        }
        
        return rc;
    }

    return _generate_OID_frame(buffer, lenght);
}

template<uint8_t VCC, uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
int CcsdsTmMcSend<VCC,F,FSH,SDLSH,SDLST>::_generate_OID_frame(uint8_t* buffer, uint16_t lenght)
{
    return -1;
}
