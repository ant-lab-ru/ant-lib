#pragma once

#include <stdint.h>
#include <string.h>

#include "ehas-defs.h"

enum {
    EHAS_ERROR = 0,
    EHAS_WARN,
    EHAS_INFO,
    EHAS_FF = 0xFF,
};

typedef struct {
    uint16_t* counter;
    char* name;
} ehas_report_t;

typedef struct {
    uint16_t name_max_len;
    uint16_t reports_len;
    ehas_report_t* reports;
} ehas_reports_t;

typedef struct {
    uint8_t     type;
    uint8_t     mask;
    const char* name;
} ehas_pack_t;

class EhasPackBase
{
    public:
        EhasPackBase(const char* name_i, ehas_pack_t* pack_i, const uint16_t pack_len_i, uint16_t* counters_i):
            name(name_i),
            pack(pack_i),
            pack_len(pack_len_i),
            counters(counters_i)
            {};

        const char*        const name;
        const ehas_pack_t* const pack;
        uint16_t*          const counters;
        const uint16_t pack_len;
};

template<int L>
class EhasPack : public EhasPackBase
{
    public:

    EhasPack(const char* name_i, ehas_pack_t* pack_i):
        EhasPackBase(name_i, pack_i, L, this->counters){};

    uint16_t counters[L] = {0};
};

typedef struct {
    bool active;
    EhasPackBase* device;
} ehas_device_t;

template<int N>
class Ehas
{
    public:
        bool add_device(EhasPackBase* device);

        bool get_error_report(ehas_reports_t* reports);
        bool get_warn_report(ehas_reports_t* reports);
        bool get_info_report(ehas_reports_t* reports);
        bool get_full_report(ehas_reports_t* reports);

    private:
        bool common_report(uint8_t type, ehas_reports_t* reports);
        bool write_to_report(const char* name, uint16_t counter, uint16_t* reports_ptr, ehas_reports_t* reports);
        ehas_device_t devices[N] = {0};
};

//============================================================================================
//  Public
//============================================================================================

template<int N>
bool Ehas<N>::add_device(EhasPackBase* device) {
    if (!device) {
        return false;
    }

    for(int i = 0; i < N; i++) {
        if (this->devices[i].active) {
            continue;
        }
        this->devices[i].active = true;
        this->devices[i].device = device;
        return true;
    }

    return false;
}

template<int N>
bool Ehas<N>::get_error_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_ERROR, reports);
}

template<int N>
bool Ehas<N>::get_warn_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_WARN, reports);
}

template<int N>
bool Ehas<N>::get_info_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_INFO, reports);
}

template<int N>
bool Ehas<N>::get_full_report(ehas_reports_t* reports) {
    return this->common_report(EHAS_FF, reports);
}

//============================================================================================
//  Private
//============================================================================================

template<int N>
bool Ehas<N>::write_to_report(const char* name, uint16_t counter, uint16_t* reports_ptr, ehas_reports_t* reports) {
    return false;
}

template<int N>
bool Ehas<N>::common_report(uint8_t type, ehas_reports_t* reports)
{
    uint16_t reports_ptr = 0;

    for (int i = 0; i < N; i++) {
        ehas_device_t* d = &this->devices[i];

        if (!d->active || !d->device) {
            continue;
        }

        for (int j = 0; j < d->device->pack_len; j++) {
            const ehas_pack_t* p = &d->device->pack[j];
            uint16_t c = d->device->counters[j];
            if (c == 0) {
                continue;
            }
            if (p->type != type && type != EHAS_FF) {
                continue;
            }
            if (!this->write_to_report(p->name, c, &reports_ptr, reports)) {
                return false;
            }
        }
    }

    return true;
}

