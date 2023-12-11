#pragma once
#include <stdbool.h>
#include <stdint.h>

class IGpio
{
    public:
        virtual void put(bool value) = 0;
        virtual bool get() = 0;
        virtual void toggle() = 0;
        virtual void blink(uint16_t count, uint16_t period_ms) = 0;
};