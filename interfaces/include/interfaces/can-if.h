#pragma once
#include <stdint.h>

#ifndef CAN_FRAME_MAX_DATA_LEN
#define CAN_FRAME_MAX_DATA_LEN 8
#endif

typedef struct {
    uint32_t canId; // max 0x1FFF FFFF
    uint32_t frameSize;
    uint8_t  frameData[CAN_FRAME_MAX_DATA_LEN];
} CanFrame;

class ICan
{
    public:
        virtual int read(CanFrame* frame) = 0;
        virtual int write(const CanFrame* frame) = 0;
        virtual int filterInit(uint32_t canId) = 0;
        virtual int filterDeinit(uint32_t canId) = 0;
};
