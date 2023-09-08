#include "mr25h40.h"
#include <string.h>

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

    if (!this->chip_select || !buffer) {
        this->reset_all_requests();
        return -1;
    }

    if (this->read_status_request) {
        this->read_status_request = false;

        if (lenght != sizeof(uint8_t)) {
            this->reset_all_requests();
            return -2;
        }
        buffer[0]  = this->WEL  << 1;
        buffer[0] |= this->BP0  << 2;
        buffer[0] |= this->BP1  << 3;
        buffer[0] |= this->SRWD << 7;
        return sizeof(uint8_t);
    }

    if (this->read_data_request) {
        if (this->read_data_addr + lenght > sizeof(this->memory) || this->read_data_addr + lenght < this->read_data_addr) {
            this->reset_all_requests();
            return -3;
        }
        memcmp(buffer, &this->memory[this->read_data_addr], lenght);
        this->read_data_addr += lenght;
        return lenght;
    }

    return -99;
}

int ImitatorMr25h40::write(const uint8_t* data, uint32_t size) {
    if (size == 0) {
        return 0;
    }

    if (!this->chip_select || !data) {
        this->reset_all_requests();
        return -1;
    }

    switch (data[0])
    {
    // Write enable
    case 0x06:
        if (size != sizeof(uint8_t)) {
            this->reset_all_requests();
            return -2;
        }
        this->WEL = true;
        return sizeof(uint8_t);
    
    // Write disable
    case 0x04:
        if (size != sizeof(uint8_t)) {
            this->reset_all_requests();
            return -3;
        }
        this->WEL = false;
        return sizeof(uint8_t);

    // Read status register
    case 0x05:
        if (size != sizeof(uint8_t)) {
            this->reset_all_requests();
            return -4;
        }
        this->reset_all_requests();
        this->read_status_request = true;
        return sizeof(uint8_t);

    // Write status register
    case 0x01:
        if (!this->WEL) {
            this->reset_all_requests();
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
            return size;
        }
        this->reset_all_requests();
        return -6;

    // Read data
    case 0x03:
        if (size != 4 * sizeof(uint8_t)) {
            this->reset_all_requests();
            return -7;
        }
        uint32_t addr = (data[1] << 16) + (data[2] << 8) + (data[3] << 0);
        if (addr >= sizeof(this->memory)) {
            this->reset_all_requests();
            return -8;
        }

        this->reset_all_requests();
        this->read_data_addr = addr;
        this->read_data_request = true;
        return size;

    // Write data
    case 0x02:
        if (!this->WEL) {
            this->reset_all_requests();
            return -9;
        }
        if (size < 4) {
            this->reset_all_requests();
            return -10;
        }

        uint32_t addr = (data[1] << 16) + (data[2] << 8) + (data[3] << 0);
        if (addr >= sizeof(this->memory)) {
            this->reset_all_requests();
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
            return -12;
        }
        memcmp(&this->memory[this->write_data_addr], data, size);
        this->write_data_addr += size;
        return size;

    // Sleep
    case 0xB9:
    case 0xAB:
        return size;

    default:
        if (this->write_data_request) {
            if (this->write_data_addr + size > sizeof(this->memory) || this->write_data_addr + size < this->write_data_addr) {
                this->reset_all_requests();
                return -14;
            }
            memcmp(&this->memory[this->write_data_addr], data, size);
            this->write_data_addr += size;
            return size;
        }

        if (this->write_status_request) {
            this->write_status_request = false;
            if (size != sizeof(uint8_t)) {
                this->reset_all_requests();
                return -15;
            }

            this->BP0  = !!(data[0] >> 2);
            this->BP1  = !!(data[0] >> 3);
            this->SRWD = !!(data[0] >> 7);
            return size;
        }

        this->reset_all_requests();
        return -16;
    }
} 

void ImitatorMr25h40::reset_all_requests() {
    this->read_data_request = false;
    this->write_data_request = false;
    this->read_status_request = false;
    this->write_status_request = false;
}