#include "ant-lib/ehas.h"

//============================================================================================
//  Public
//============================================================================================

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

bool Ehas::get_error_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_ERROR, reports);
}

bool Ehas::get_warn_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_WARN, reports);
}

bool Ehas::get_info_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_INFO, reports);
}

bool Ehas::get_uninit_report(ehas_reports_t* reports) {
    return false;
}

bool Ehas::get_full_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_FF, reports);
}

//============================================================================================
//  Private
//============================================================================================

bool Ehas::write_to_report(const char* name, uint16_t counter, uint16_t* reports_ptr, ehas_reports_t* reports) {
    return false;
}

bool Ehas::common_report(uint8_t type, ehas_reports_t* reports)
{
    uint16_t reports_ptr = 0;

    for (int i = 0; i < EHAS_MAX_DEVICES; i++) {
        ehas_device_t* d = &this->devices[i];

        if (!d->active || !d->pack) {
            continue;
        }

        for (int j = 0; j < d->pack->pack_len; j++) {
            ehas_pack_t* p = &d->pack->pack[j];
            if (p->counter == 0) {
                continue;
            }
            if (p->type != type && type != EHAS_FF) {
                continue;
            }
            if (!this->write_to_report(p->name, p->counter, &reports_ptr, reports)) {
                return false;
            }
        }
    }

    return true;
}
