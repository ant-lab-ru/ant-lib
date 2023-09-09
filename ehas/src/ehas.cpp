#include "ant-lib/ehas.h"

bool Ehas::error_report()
{
    for (int i = 0; i < EHAS_MAX_DEVICES; i++) {
        ehas_device_t* d = &this->devices[i];

        if (!d->active || !d->pack) {
            continue;
        }

        for (int j = 0; j < d->pack->pack_len; j++) {
            if (d->pack->pack[j].counter == 0) {
                continue;
            }

            
        }
    }

    return 0;
}

bool Ehas::init() {
    memset(this->devices, 0, sizeof(this->devices));
    return true;
}

bool Ehas::add_device(EhasPackBase* device) {
    if (!device) {
        return false;
    }

    for(int i = 0; i < EHAS_MAX_DEVICES; i++) {
        if (this->devices[i].active) {
            continue;
        }
        this->devices[i].active = true;
        this->devices[i].pack = device;
        return true;
    }

    return false;
}
