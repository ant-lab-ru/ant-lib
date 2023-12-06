#pragma once

#define _to_str_IMPL_(x) #x      //stringify argument
#define _to_STR(x) _to_str_IMPL_(x)  //indirection to expand argument macros

#define EHAS_UP(idx) { \
    static_assert(idx < (sizeof(this->ehas.counters) / sizeof(uint16_t)), "Big index"); \
    static_assert(idx >= 0, "Small index"); \
    if (this->ehas.pack[idx].counter < UINT16_MAX) { \
        this->ehas.pack[idx].counter++; \
    } \
}

#define EHAS_INIT_RC(prefix, nm, tp)  {                                                                \
    static_assert((prefix##_##nm) < (sizeof(this->ehas.counters) / sizeof(uint16_t)), "Big index"); \
    static_assert((prefix##_##nm) >= 0, "Small index");                                                  \
    this->ehas.pack[(prefix##_##nm)].name = #nm;                                                         \
    this->ehas.pack[(prefix##_##nm)].counter = 0;                                                        \
    this->ehas.pack[(prefix##_##nm)].type = tp;                                                          \
    this->ehas.pack[(prefix##_##nm)].mask = 0;                                                           \
};        

#define EHAS_INIT(name) ehas(name, &(this->ehas_pack))

#define EHAS_PACK(n) EhasPack<n> ehas; static ehas_pack_t ehas_pack[n] = {0};
