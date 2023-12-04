#pragma once

#include "stdint.h"

#include "ccsds-tm-vc-send.h"

#include "ant-lib/if-scheduler.h"

template<uint8_t VCC, uint16_t F, uint16_t FSH, uint16_t SDLSH, uint16_t SDLST>
class CcsdsTmMcSend
{
public:
    CcsdsTmMcSend():
    {};

    CcsdsTmVcSend<F,FSH,SDLSH,SDLST> vc[VCC];
    uint8_t vc_priority[VCC];

    int MC_FSH_request(uint8_t* data, uint16_t size);

    int get_MC_frame(uint8_t* buffer, uint16_t lenght);

    bool MC_FSH_enable = false;

    int handle();
    int init();

private:

    CcsdsEpp _ep;
    CcsdsSdls _sdls;

    IScheduler* _scheduler;

    uint8_t _MC_FSH_buffer[FSH] = {0};

    uint32_t   _MC_frame_counter = 0;

    int _virtual_channel_multiplexing(uint8_t* buffer, uint16_t lenght);
    int _master_channel_generation(uint8_t* buffer, uint16_t lenght);
    int _generate_OID_frame(uint8_t* buffer, uint16_t lenght);
};
