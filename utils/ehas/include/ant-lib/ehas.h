#pragma once

#include <stdint.h>
#include <string.h>

#include "ehas-defs.h"

#ifndef EHAS_MAX_DEVICES
#define EHAS_MAX_DEVICES 50
#endif

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
    ehas_report_t reports[];
} ehas_reports_t;

typedef struct {
    uint16_t    counter;
    uint8_t     type;
    uint8_t     mask;
    const char* name;
} ehas_pack_t;

class EhasPackBase
{
    public:
        EhasPackBase(const uint16_t pack_len, ehas_pack_t* pack):
            pack_len(pack_len),
            pack(pack)
            {};

        const char* device_name;
        const uint16_t pack_len;
        ehas_pack_t* pack;
};

template<int L>
class EhasPack : public EhasPackBase
{
    public:

    EhasPack(): EhasPackBase(L, this->pack_array){};

    ehas_pack_t pack_array[L] = {0};
};

typedef struct {
    bool active;
    EhasPackBase* pack;
} ehas_device_t;

class Ehas
{
    public:
        bool init();
        bool add_device(EhasPackBase* device);
        bool get_error_report(ehas_reports_t* reports);
        bool get_warn_report(ehas_reports_t* reports);
        bool get_info_report(ehas_reports_t* reports);
        bool get_uninit_report(ehas_reports_t* reports);
        bool get_full_report(ehas_reports_t* reports);

    private:
        bool common_report(uint8_t type, ehas_reports_t* reports);
        bool write_to_report(const char* name, uint16_t counter, uint16_t* reports_ptr, ehas_reports_t* reports);
        ehas_device_t devices[EHAS_MAX_DEVICES];
};
