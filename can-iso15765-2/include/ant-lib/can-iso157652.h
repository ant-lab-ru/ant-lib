#pragma once
#include <stdint.h>

// FRAME TYPE SINGLE
const uint16_t canIsoSingleHeadLen = 1;
const uint16_t canIsoSingleType = 0;

typedef struct {
    uint8_t size : 4;
    uint8_t type : 4;
} CanIsoSingleHeadBf;

typedef union {
    CanIsoSingleHeadBf  bf;
    uint8_t             word;
} CanIsoSingleHead;

// FRAME TYPE FIRST
const uint16_t canIsoFirstHeadLen = 2;
const uint16_t canIsoFirstType = 1;

typedef struct {
    uint8_t size : 12;
    uint8_t type : 4;
} CanIsoFirstHeadBf;

typedef union {
    CanIsoFirstHeadBf   bf;
    uint16_t            word;
} CanIsoFirstHead;

// FRAME TYPE CONSEC
const uint16_t canIsoConsecHeadLen = 1;
const uint16_t canIsoConsecType = 2;

typedef struct {
    uint8_t indx : 4;
    uint8_t type : 4;
} CanIsoConsecHeadBf;

typedef union {
    CanIsoConsecHeadBf  bf;
    uint8_t             word;
} CanIsoConsecHead;

typedef enum {
  canIsoTxStateIdle = 0,
  canIsoTxStateSingle,
  canIsoTxStateFirst,
  canIsoTxStateConsecutive,  
} CanIsoTxState;

class CanIso157652
{
    public:
        int init(uint32_t frameSize, uint8_t* readBuffer, uint32_t readBufferLenght);
        int deinit();

        int pushFrame(const uint8_t* data, uint32_t size);
        int readPacket(uint8_t* buffer, uint32_t lenght);
        bool isPacketReady();

        int writePacket(const uint8_t* data, uint32_t size);
        int popFrame(uint8_t* buffer, uint32_t lenght);

    private:
        uint32_t frameSize = 0;
        uint32_t packetMaxLengh = 0;
        uint8_t* buffer = nullptr;

        const uint16_t canIsoPacketMaxLen = 4095;
        uint16_t txPacketSize = 0;
        uint16_t txOffset = 0;
        uint8_t* txPacketData = nullptr;
        CanIsoTxState txState = canIsoTxStateIdle;

        bool rxPacketReady;
        uint32_t rxPacketExpectedLengh;
        uint32_t rxPacketCounter;
};
