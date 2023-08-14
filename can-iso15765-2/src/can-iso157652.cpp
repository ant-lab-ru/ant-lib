#include "ant-lib/can-iso157652.h"

int CanIso157652::init(uint32_t frameSize, uint8_t* readBuffer, uint32_t readBufferLenght)
{
    this->buffer = readBuffer;
    this->packetMaxLengh = readBufferLenght;
    this->frameSize = frameSize;
}

int CanIso157652::deinit()
{

}

int CanIso157652::readPacket(uint8_t* buffer, uint32_t lenght)
{

}

int CanIso157652::pushFrame(const uint8_t* data, uint32_t size)
{

}

int CanIso157652::writePacket(const uint8_t* data, uint32_t size)
{
    if (size > this->canIsoPacketMaxLen) {
        return -1;
    }

    this->txPacketSize = size;
    this->txOffset = 0;
    this->txPacketData = data;

    if (size <= this->frameSize - canIsoSingleHeadLen) {
        this->txStarted = canIsoTxStateSingle;
    }
    else {
        this->txStarted = canIsoTxStateFirst;
    }

    return size;
}

int CanIso157652::popFrame(uint8_t* buffer, uint32_t lenght)
{
    if (!buffer || !lenght) {
        return -1;
    }

    switch (this->txState)
    {
    case canIsoTxStateIdle:
        return 0;
    
    case canIsoTxStateSingle:
        uint16_t frameLen = canIsoSingleHeadLen + this->txPacketSize;
        if (lenght < frameLen) {
            return -1;
        }

        CanIsoSingleHead head = {0};
        head.bf.type = canIsoSingleType;
        head.bf.size = this->txPacketSize;

        buffer[0] = head.word;
        memcpy(&buffer[1], this->txPacketData, this->txPacketSize);
        this->txState = canIsoTxStateIdle;
        return frameLen;

    case canIsoTxStateFirst:
        if (lenght < this->frameSize) {
            return -1;
        }

        uin16t_t dataLen = this->frameSize - canIsoFirstHeadLen;
        CanIsoFirstHead head = {0};
        head.bf.type = canIsoFirstType;
        head.bf.size = this->txPacketSize;
        memcpy(&buffer[0], (void*)&head.word, canIsoFirstHeadLen);
        memcpy(&buffer[canIsoFirstHeadLen], this->txPacketData, dataLen);
        this->txOffset = dataLen;
        this->txState = canIsoTxStateConsecutive;
        return dataLen;

    case canIsoTxStateConsecutive:
        

    default:
        break;
    }
}
