#pragma once
#include <stdint.h>

class ICanPacket
{
    public:
        virtual int init(uint8_t* readBuffer, uint32_t readBufferLenght) = 0;
        virtual int deinit() = 0;

        virtual int pushFrame(const uint8_t* data, uint32_t size) = 0;
        virtual int readPacket(uint8_t* buffer, uint32_t lenght) = 0;
        virtual bool isPacketReady() = 0;

        virtual int writePacket(const uint8_t* data, uint32_t size) = 0;
        virtual int popFrame(uint8_t* buffer, uint32_t lenght) = 0;
};