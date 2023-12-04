#include "ant-lib/ccsds-tm.h"

template<uint8_t MCC, uint8_t VCC, uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
int CcsdsTmSend<MCC,VCC,F,FSH,SDLSH,SDLST>::all_frames_generation(uint8_t* buffer, uint16_t lenght)
{
    return _master_channel_multiplexing(buffer, lenght);
}

template<uint8_t MCC, uint8_t VCC, uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
int CcsdsTmSend<MCC,VCC,F,FSH,SDLSH,SDLST>::_master_channel_multiplexing(uint8_t* buffer, uint16_t lenght)
{
    if (MCC == 1) {
        return mc[0].get_MC_frame(buffer, lenght);
    }

    return -30;
}

