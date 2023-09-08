#include "ant-lib/mr25h.h"
#include "ant-lib/mr25h-regs.h"

int Mr25h::init(ISpi* i_spi, IWdt* i_wdt) {
    if (!i_spi) {
        return -1;
    }

    this->spi = i_spi;
    this->wdt = i_wdt;

    EHAS_INIT_DEVICE("Mr25h");
    EHAS_INIT_PACK(MR25H_ERR, NullPtr,      EHAS_ERROR);
    EHAS_INIT_PACK(MR25H_ERR, SpiTx,        EHAS_ERROR);
    EHAS_INIT_PACK(MR25H_ERR, SpiRx,        EHAS_ERROR);

    EHAS_RETURN_INIT_OK();
}

