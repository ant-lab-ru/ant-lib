#pragma once

#define _to_str_IMPL_(x) #x      //stringify argument
#define _to_STR(x) _to_str_IMPL_(x)  //indirection to expand argument macros

#define EHAS_UP(idx) { \
    static_assert(idx < (sizeof(this->ehas.counters) / sizeof(uint16_t)), "Big index"); \
    static_assert(idx >= 0, "Small index"); \
    if (this->ehas.counters[idx] < UINT16_MAX) { \
        this->ehas.counters[idx]++; \
    } \
}

#define EHAS_INIT_RC(c, prefix, nm, tp) \
    static_assert((prefix##_##nm) < (sizeof(c::ehas_pack) / sizeof(ehas_pack_t)), "Big index"); \
    static_assert((prefix##_##nm) >= 0, "Small index"); \
	ehas_pack_t c::ehas_pack[(prefix##_##nm)] = { .type = tp, .mask = 0, .name = #nm }; \

#define EHAS_INIT(c, name) ehas(name, &(ehas_pack[0]))

#define EHAS_PACK(n) EhasPack<n> ehas; static ehas_pack_t ehas_pack[n];
