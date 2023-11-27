#include "ant-lib/if-spi.h"

#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal_gpio.h"

class Spi : public ISpi
{
public:

    Spi(SPI_HandleTypeDef* hspi, GPIO_TypeDef* cs_port, uint16_t cs_pin)
        : _hspi(hspi), _cs_port(cs_port), _cs_pin(cs_pin) {
            EHAS_INIT_NOINIT_DEVICE("i2c");
            EHAS_INIT_PACK(I2C_ERROR, NullPtr, EHAS_ERROR);
            EHAS_INIT_PACK(I2C_ERROR, Hal,     EHAS_ERROR);
        };

    EhasPack<SPI_NumberOfTypes> ehas;

    int read(uint8_t* buffer, uint32_t lenght);
    int write(const uint8_t* data, uint32_t size);

private:

    SPI_HandleTypeDef*  _hspi;
    GPIO_TypeDef*       _cs_port;
    const uint16_t      _cs_pin;
};