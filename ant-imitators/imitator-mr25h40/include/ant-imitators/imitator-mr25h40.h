#pragma once

#include <stdint.h>
#include "ant-interfaces/spi-if.h"

class ImitatorMr25h40 : public ISpi
{
    public:
        ImitatorMr25h40() {};

        int read(uint8_t* buffer, uint32_t lenght);
        int write(const uint8_t* data, uint32_t size);
        int select();
        int deselect();

    private:
        uint8_t memory[512 * 1024];
        bool chip_select = false;

        bool read_data_request = false;
        uint32_t read_data_addr = 0;
        bool write_data_request = false;
        uint32_t write_data_addr = 0;
        bool read_status_request = false;
        bool write_status_request = false;

        bool WEL = false;
        bool BP0 = false;
        bool BP1 = false;
        bool SRWD = false;

        void reset_all_requests();
};