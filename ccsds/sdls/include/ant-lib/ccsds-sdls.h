#pragma once

#include "stdint.h"

class CcsdsSdls
{
public:
    int apply_security(uint8_t* data, uint32_t size, uint8_t* buffer, uint32_t lenght);
    int process_security(uint8_t* data, uint32_t size, uint8_t* buffer, uint32_t lenght);
private:

};
