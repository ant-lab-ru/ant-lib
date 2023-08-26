#include <stdint.h>

#define SDLP_TMTX_VC_MAX 8
#define SDLP_TMTX_MC_MAX 2

class SdlpTmtxVc
{

};

class SdlpTmtxMc
{
    public:
        int init(uint16_t MCID, uint16_t frame_size, uint16_t secondary_header_data_size);
    private:
        uint16_t mcid;
        bool initialized;
        uint32_t mc_frame_count;

        SdlpTmtxVc vc[SDLP_TMTX_VC_MAX];
};

class SdlpTmTx
{
    SdlpTmtxMc mc[SDLP_TMTX_MC_MAX];
};