#include "ant-lib/platform-gpio.h"

void Gpio::put(bool value)
{
    HAL_GPIO_WritePin(_port, _pin, (GPIO_PinState)value);
}

bool Gpio::get()
{
    return HAL_GPIO_ReadPin(_port, _pin);
}

void Gpio::toggle()
{
    HAL_GPIO_TogglePin(_port, _pin);
}

void Gpio::blink(uint16_t count, uint16_t period_ms)
{
    for (int i = 0; i < 2 * count; i++)
    {
        HAL_GPIO_TogglePin(_port, _pin);
        HAL_Delay(period_ms / 2);
    }
}