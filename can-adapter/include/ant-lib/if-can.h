#pragma once
#include <stdint.h>

#ifndef CAN_FRAME_MAX_DATA_LEN
#define CAN_FRAME_MAX_DATA_LEN 8
#endif

typedef struct {
    uint32_t can_id; // max 0x1FFF FFFF
    uint32_t frame_size;
    uint8_t  frame_data[CAN_FRAME_MAX_DATA_LEN];
} can_frame_t;

class ICan
{
    public:
        virtual int read(can_frame_t* frame) = 0;
        virtual int write(const can_frame_t* frame) = 0;
        virtual int filter_init(uint32_t can_id) = 0;
        virtual int filter_deinit(uint32_t can_id) = 0;
};

