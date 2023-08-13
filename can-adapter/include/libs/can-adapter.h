#pragma once

#include "if-can.h"
#include "if-canpacket.h"
#include "libs/static-queue.h"

#ifndef CAN_ADAPTER_SOCKETS_MAX
#define CAN_ADAPTER_SOCKETS_MAX 20
#endif

class CanSocket
{
    public:
        bool socketOpen; // true если сокет открыт
        uint32_t myCanId; // CAN ID нужный для настройки фильтра и приема кадров, пакетов
        uint32_t dstCanId; // CAN ID который добавляется в передаваемое сообщение
        uint32_t packetDataMaxSize;

        ICanPacket* packet;

        bool queueEnable;
        StaticQueue queue;
};

typedef struct
{
    uint32_t myCanId;
    uint32_t dstCanId;
    uint8_t* packetBuffer;
    uint32_t packetBufferLenght;
    ICanPacket* iCanPacket;

    uint8_t* queueBuffer;
    uint32_t queueBufferLenght;
} SocketOpenInitStruct;

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
        ICan* can;
		CanSocket socket[CAN_ADAPTER_SOCKETS_MAX];
		bool initialized;
		int findFreeSocket(uint32_t myId);
		CanSocket* getSocketById(int socketId);
		CanSocket* routeFrame(uint32_t myId);
};