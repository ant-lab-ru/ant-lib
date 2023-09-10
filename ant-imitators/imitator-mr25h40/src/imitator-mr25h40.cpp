#include "ant-imitators/imitator-mr25h40.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define MR25H_LOG_ERROR(err) printf(ANSI_COLOR_MAGENTA "[----------] " err ANSI_COLOR_RESET "\n");

#define MR25H_LOG_INFO(info, ...) printf(ANSI_COLOR_CYAN "[----------] " info ANSI_COLOR_RESET "\n", ##__VA_ARGS__);


int ImitatorMr25h40::select() {
    this->chip_select = true;
    this->reset_all_requests();
    return 0;
}

int ImitatorMr25h40::deselect() {
    this->chip_select = false;
    this->reset_all_requests();
    return 0;
}

int ImitatorMr25h40::read(uint8_t* buffer, uint32_t lenght) {
    if (lenght == 0){
        return 0;
    }

    if (!buffer) {
        MR25H_LOG_ERROR("Mr25h40.read; nullptr input");
        return -1;
    }

    if (!this->chip_select) {
        this->reset_all_requests();
        MR25H_LOG_ERROR("Mr25h40.read; chip_select false");
        return -1;
    }

    if (this->read_status_request) {
        this->read_status_request = false;

        if (lenght != sizeof(uint8_t)) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.read; read_status; lenght != 1");
            return -2;
        }
        buffer[0]  = this->WEL  << 1;
        buffer[0] |= this->BP0  << 2;
        buffer[0] |= this->BP1  << 3;
        buffer[0] |= this->SRWD << 7;
        MR25H_LOG_INFO("Mr25h40.read; status register has been read");
        return sizeof(uint8_t);
    }

    if (this->read_data_request) {
        if (this->read_data_addr + lenght > sizeof(this->memory) || this->read_data_addr + lenght < this->read_data_addr) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.read; read_data; address + lenght > memory_size");
            return -3;
        }
        memcpy(buffer, &this->memory[this->read_data_addr], lenght);
        MR25H_LOG_INFO("Mr25h40.read; data has been read; addr: 0x%X, lenght: 0x%X", this->read_data_addr, lenght);
        this->read_data_addr += lenght;
        return lenght;
    }

    MR25H_LOG_ERROR("Mr25h40.read; unknown; writing data without request");
    return -99;
}

int ImitatorMr25h40::write(const uint8_t* data, uint32_t size) {
    if (size == 0) {
        return 0;
    }

    if (!data) {
        MR25H_LOG_ERROR("Mr25h40.write; nullptr input");
        return -1;
    }

    if (!this->chip_select) {
        this->reset_all_requests();
        MR25H_LOG_ERROR("Mr25h40.write; chip_select false");
        return -1;
    }

    switch (data[0])
    {
    // Write enable
    case 0x06:
        if (size != sizeof(uint8_t)) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; write_enable; size != 1");
            return -2;
        }
        this->WEL = true;
        MR25H_LOG_INFO("Mr25h40.write; write has been enabled");
        return sizeof(uint8_t);
    
    // Write disable
    case 0x04:
        if (size != sizeof(uint8_t)) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; write_disable; size != 1");
            return -3;
        }
        this->WEL = false;
        MR25H_LOG_INFO("Mr25h40.write; write has been disabled");
        return sizeof(uint8_t);

    // Read status register
    case 0x05:
        if (size != sizeof(uint8_t)) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; read_status; size != 1");
            return -4;
        }
        this->reset_all_requests();
        this->read_status_request = true;
        return sizeof(uint8_t);

    // Write status register
    case 0x01:
        if (!this->WEL) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; write_status; WEL=0 permition denied");
            return -5;
        }
        if (size == sizeof(uint8_t)) {
            this->reset_all_requests();
            this->write_status_request = true;
            return size;
        }
        if (size == 2 * sizeof(uint8_t)) {
            this->BP0  = !!(data[1] >> 2);
            this->BP1  = !!(data[1] >> 3);
            this->SRWD = !!(data[1] >> 7);
            MR25H_LOG_INFO("Mr25h40.write; status register has been written");
            return size;
        }
        this->reset_all_requests();
        MR25H_LOG_ERROR("Mr25h40.write; write_status; size != 1 and != 2");
        return -6;

    // Read data
    case 0x03: {
        if (size != 4 * sizeof(uint8_t)) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; read_data; size != 4");
            return -7;
        }

        uint32_t addr = (data[1] << 16) + (data[2] << 8) + (data[3] << 0);
        if (addr >= sizeof(this->memory)) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; read_data; address > memory_size");
            return -8;
        }

        this->reset_all_requests();
        this->read_data_addr = addr;
        this->read_data_request = true;
        return size;
    }

    // Write data
    case 0x02:{
        if (!this->WEL) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; write_data; WEL=0 permition denied");
            return -9;
        }
        if (size < 4) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; write_data; size < 4");
            return -10;
        }

        uint32_t addr = (data[1] << 16) + (data[2] << 8) + (data[3] << 0);
        if (addr >= sizeof(this->memory)) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; write_data; address > memory_size");
            return -11;
        }

        this->reset_all_requests();
        this->write_data_addr = addr;
        this->write_data_request = true;

        if (size == 4) {
            return size;
        }

        if (this->write_data_addr + size > sizeof(this->memory) || this->write_data_addr + size < this->write_data_addr) {
            this->reset_all_requests();
            MR25H_LOG_ERROR("Mr25h40.write; write_data; address+size > memory_size");
            return -12;
        }
        memcpy(&this->memory[this->write_data_addr], data, size);
        MR25H_LOG_INFO("Mr25h40.write; data has been written; addr: 0x%X, lenght: 0x%X", this->write_data_addr, size);
        this->write_data_addr += size;
        return size;
    }

    // Sleep
    case 0xB9:
    case 0xAB:
        return size;

    default: {
        if (this->write_data_request) {
            if (this->write_data_addr + size > sizeof(this->memory) || this->write_data_addr + size < this->write_data_addr) {
                this->reset_all_requests();
                MR25H_LOG_ERROR("Mr25h40.write; write_data; address+size > memory_size");
                return -14;
            }
            memcpy(&this->memory[this->write_data_addr], data, size);
            MR25H_LOG_INFO("Mr25h40.write; data has been written; addr: 0x%X, lenght: 0x%X", this->write_data_addr, size);
            this->write_data_addr += size;
            return size;
        }

        if (this->write_status_request) {
            this->write_status_request = false;
            if (size != sizeof(uint8_t)) {
                this->reset_all_requests();
                MR25H_LOG_ERROR("Mr25h40.write; write_status; size != 1");
                return -15;
            }

            this->BP0  = !!(data[0] >> 2);
            this->BP1  = !!(data[0] >> 3);
            this->SRWD = !!(data[0] >> 7);
            MR25H_LOG_INFO("Mr25h40.write; status register has been written");
            return size;
        }

        this->reset_all_requests();
        MR25H_LOG_ERROR("Mr25h40.write; unknown; writing data without request");
        return -16;
    }
    }
} 

void ImitatorMr25h40::reset_all_requests() {
    this->read_data_request = false;
    this->write_data_request = false;
    this->read_status_request = false;
    this->write_status_request = false;
}