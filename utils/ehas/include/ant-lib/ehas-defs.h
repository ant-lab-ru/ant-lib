#pragma once

#define _to_str_IMPL_(x) #x      //stringify argument
#define _to_STR(x) _to_str_IMPL_(x)  //indirection to expand argument macros

#define EHAS_INIT_DEVICE(dv_nm) {                                                                   \
    static_assert(sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t) >= 1, "Less than one pack");  \
    /* memset(&(this->ehas), 0, sizeof(this->ehas));  */                                            \
    this->ehas.pack[0].name = "InitOk";                                                             \
    this->ehas.pack[0].counter = 0;                                                                 \
    this->ehas.pack[0].type = EHAS_INFO;                                                            \
    this->ehas.pack[0].mask = 0;                                                                    \
    this->ehas.device_name = dv_nm;                                                                 \
}

#define EHAS_INIT_NOINIT_DEVICE(dv_nm) {                                                    \
    static_assert(sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t) >= 1, "Less than one pack");  \
    /* memset(&(this->ehas), 0, sizeof(this->ehas));  */                                            \
    this->ehas.pack[0].name = "InitOk";                                                             \
    this->ehas.pack[0].counter = 1;                                                                 \
    this->ehas.pack[0].type = EHAS_INFO;                                                            \
    this->ehas.pack[0].mask = 0;                                                                    \
    this->ehas.device_name = dv_nm;                                                         \
}

#define EHAS_INIT_PACK(prefix, nm, tp)  {                                                                \
    static_assert((prefix##_##nm) < (sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t)), "Big index"); \
    static_assert((prefix##_##nm) >= 0, "Small index");                                                  \
    this->ehas.pack[(prefix##_##nm)].name = #nm;                                                         \
    this->ehas.pack[(prefix##_##nm)].counter = 0;                                                        \
    this->ehas.pack[(prefix##_##nm)].type = tp;                                                          \
    this->ehas.pack[(prefix##_##nm)].mask = 0;                                                           \
};

#define EHAS_RETURN_INIT_OK() {                                                                     \
    if (this->ehas.pack[0].counter < UINT16_MAX) {                                                  \
        this->ehas.pack[0].counter++;                                                               \
    }                                                                                               \
    return 0;                                                                                       \
}

#define EHAS_RETURN(idx) {                                                                          \
    static_assert(idx < (sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t)), "Big index");        \
    static_assert(idx >= 0, "Small index");                                                         \
    if (this->ehas.pack[idx].counter < UINT16_MAX) {                                                \
        this->ehas.pack[idx].counter++;                                                             \
    }                                                                                               \
    return -idx;                                                                                    \
}

#define EHAS_INCREMENT(idx) {                                                                       \
    static_assert(idx < (sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t)), "Big index");        \
    static_assert(idx >= 0, "Small index");                                                         \
    if (this->ehas.pack[idx].counter < UINT16_MAX) {                                                \
        this->ehas.pack[idx].counter++;                                                             \
    }                                                                                               \
}

#define EHAS_RISE(idx) {}

#define EHAS_UP(idx) {}

#define EHAS_INIT_RC(prefix, nm, tp)  {                                                                \
    static_assert((prefix##_##nm) < (sizeof(this->ehas.pack_array) / sizeof(ehas_pack_t)), "Big index"); \
    static_assert((prefix##_##nm) >= 0, "Small index");                                                  \
    this->ehas.pack[(prefix##_##nm)].name = #nm;                                                         \
    this->ehas.pack[(prefix##_##nm)].counter = 0;                                                        \
    this->ehas.pack[(prefix##_##nm)].type = tp;                                                          \
    this->ehas.pack[(prefix##_##nm)].mask = 0;                                                           \
};        

#define EHAS_INIT(name) {}

#define EHAS_PACK(n) \
