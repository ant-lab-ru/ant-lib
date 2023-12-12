#pragma once

#include "stdint.h"

typedef struct
{
    
} api_command_t;

typedef struct
{
    
} api_interface_t;

class Api
{
    int call_getter();
    int call_setter();

private:
    api_interface_t* _if;
    uint8_t _if_count; 
};
