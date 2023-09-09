#pragma once

#include <stdint.h>
#include <string.h>

#ifndef EHAS_MAX_DEVICES
#define EHAS_MAX_DEVICES 50
#endif

enum {
    EHAS_ERROR = 0,
    EHAS_WARN,
    EHAS_INFO,
};

#define EHAS_INIT_DEVICE(dv_nm) {                                                                   \
    static_assert(sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t) >= 1, "Less than one pack");  \
    memset(&this->ehas, 0, sizeof(this->ehas));                                                     \
    this->ehas.pack[0].name = "InitOk";                                                             \
    this->ehas.pack[0].counter = 0;                                                                 \
    this->ehas.pack[0].type = EHAS_INFO;                                                            \
    this->ehas.pack[0].mask = 0;                                                                    \
    this->ehas.device_name = dv_nm;                                                                 \
}

#define EHAS_INIT_PACK(prefix, nm, tp)  {                                                           \
    static_assert((prefix##_##nm) < (sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t)));         \
    static_assert((prefix##_##nm) >= 0);                                                            \
    this->ehas.pack[(prefix##_##nm)].name = #nm;                                                    \
    this->ehas.pack[(prefix##_##nm)].counter = 0;                                                   \
    this->ehas.pack[(prefix##_##nm)].type = tp;                                                     \
    this->ehas.pack[(prefix##_##nm)].mask = 0;                                                      \
};

#define EHAS_RETURN_INIT_OK() {                                                                     \
    if (this->ehas.pack[0].counter < UINT16_MAX) {                                                  \
        this->ehas.pack[0].counter++;                                                               \
    }                                                                                               \
    return 0;                                                                                       \
}

#define EHAS_RETURN_OK() {                                                                          \
    return 0;                                                                                       \
}

#define EHAS_RETURN(idx) {                                                                          \
    static_assert(idx < (sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t)));                     \
    static_assert(idx >= 0);                                                                        \
    if (this->ehas.pack[idx].counter < UINT16_MAX) {                                                \
        this->ehas.pack[idx].counter++;                                                             \
    }                                                                                               \
    return -idx;                                                                                    \ 
}

#define EHAS_INCREMENT(idx) {                                                                       \
    static_assert(idx < (sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t)));                     \
    static_assert(idx >= 0);                                                                        \ 
    if (this->ehas.pack[idx].counter < UINT16_MAX) {                                                \
        this->ehas.pack[idx].counter++;                                                             \
    }                                                                                               \
}

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

    EhasPack():
        EhasPackBase(L, this->pack_array){};

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
        bool error_report();

    private:
        ehas_device_t devices[EHAS_MAX_DEVICES];
};
