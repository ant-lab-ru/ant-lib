#include "ccsds-tm-mc-send.h"

template<
    uint8_t MCC,
    uint8_t VCC,
    uint16_t F,
    uint16_t FSH,
    uint16_t SDLSH,
    uint16_t SDLST
> class CcsdsTmSend
{
public:
    int init();
    int handle();

    int all_frames_generation(uint8_t* buffer, uint16_t lenght);

    CcsdsTmMcSend<VCC,F,FSH,SDLSH,SDLST> mc[MCC];

private:
    int _master_channel_multiplexing(uint8_t* buffer, uint16_t lenght);
};
