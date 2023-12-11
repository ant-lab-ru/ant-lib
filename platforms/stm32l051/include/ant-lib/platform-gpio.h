#pragma once
#include "stdint.h"
#include "ant-lib/ehas.h"
#include "stm32l0xx_hal.h"

// Interfaces
#include "ant-lib/if-gpio.h"

class Gpio : public IGpio
{
public:
    Gpio(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
    : _port(GPIOx)
    , _pin(GPIO_Pin) {};

    void put(bool value);
    bool get();
    void toggle();
    void blink(uint16_t count, uint16_t period_ms);

private:
    GPIO_TypeDef* _port;
    uint16_t      _pin;
};

