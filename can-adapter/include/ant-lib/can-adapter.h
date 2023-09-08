#pragma once

#include "ant-interfaces/can-if.h"
#include "ant-interfaces/can-packet-if.h"
#include "ant-lib/static-queue.h"

#ifndef CAN_ADAPTER_SOCKETS_MAX
#define CAN_ADAPTER_SOCKETS_MAX 20
#endif

typedef struct
{
    // My can id. Used for routing received frames
    uint32_t myCanId;
    // destination can id. Inserted into the transmitted frames
    uint32_t dstCanId;
    
    // Buffer for assembling the package for reception.
    // The buffer is used in iCanPacket.
    // packetBufferLenght must be not less than max len of the packet
    uint8_t* packetBuffer;
    uint32_t packetBufferLenght;

    // Interface of the CAN packet assembly library
    ICanPacket* iCanPacket;

    // Optional buffer for saving received packets to the queue
    // nullptr if not used
    // minimum lenght = queueBufferLenght + 2
    // one element size = queueBufferLenght + 2
    uint8_t* queueBuffer;
    uint32_t queueBufferLenght;
} SocketOpenInitStruct;


class CanSocket
{
    public:
        bool socketOpen;
        uint32_t myCanId;
        uint32_t dstCanId;
        uint32_t packetDataMaxSize;

        ICanPacket* packet;

        bool queueEnable;
        StaticQueue queue;
};

class CanAdapter
{
    public:
        int init(ICan* iCan);
        int deinit();
        int handle();

        int socketOpen(SocketOpenInitStruct* initStruct);
        int socketClose(int socketId);
        int socketRead(int socketId, uint8_t* buffer, uint32_t size);
        int socketWrite(int socketId, const uint8_t* data, uint32_t size);

    private:
		bool initialized;

        ICan* can;
		CanSocket socket[CAN_ADAPTER_SOCKETS_MAX];

		int findFreeSocket(uint32_t myId);
		CanSocket* getSocketById(int socketId);
		CanSocket* routeFrame(uint32_t myId);
};