#include "libs/can-adapter.h"

typedef struct {
    uint32_t packet_size;
    uint8_t packet_data[];
} CanAdapterPacket;

int CanAdapter::init(ICan* iCan)
{
    this->can = iCan;
    this->initialized = true;

    return 0;
}

int CanAdapter::deinit()
{
    return 0;
}

int CanAdapter::socketOpen(SocketOpenInitStruct* initStruct)
{
    int newId = this->findFreeSocket(initStruct->myCanId);

    if (newId < 0) {
        return -1;
    }

    CanSocket* socket = &this->socket[newId];

    socket->myCanId = initStruct->myCanId;
    socket->dstCanId = initStruct->dstCanId;
    socket->packet = initStruct->iCanPacket;
    socket->queueEnable = false;

    socket->packet->init(initStruct->packetBuffer, initStruct->packetBufferLenght);
    socket->socketOpen = true;
    socket->packetDataMaxSize = initStruct->packetBufferLenght;

    if (!initStruct->queueBuffer && initStruct->queueBufferLenght >= initStruct->packetBufferLenght) {
        socket->queueEnable = true;

        uint32_t element_size = initStruct->queueBufferLenght / initStruct->packetBufferLenght;

        socket->queue.init(initStruct->packetBufferLenght, element_size, initStruct->queueBuffer);
    }

    return 0;
}

int CanAdapter::socketWrite(int socketId, const uint8_t* data, uint32_t size)
{
    CanSocket* socket = this->getSocketById(socketId);
    if (socket == nullptr) {
        return -1;
    }

    socket->packet->writePacket(data, size);

    can_frame_t frame = {0};

    while(1)
    {
        int rc = socket->packet->popFrame(frame.frame_data, CAN_FRAME_MAX_DATA_LEN);
        if (rc < 0) {
            return -1;
        }
        if (rc == 0) {
            break;
        }

        frame.frame_size = rc;
        frame.can_id = socket->dstCanId;
        this->can->write(&frame);
    }
}

int CanAdapter::handle()
{
    can_frame_t frame = {0};

    // Exit when all data is received
    while(1)
    {
        // Reading low-level CAN
        int rc = this->can->read(&frame);
        // If error
        if (rc < 0) {
            return -1;
        }
        // All data is received
        if (rc == 0) {
            return 0;
        }

        // Frame routing
        CanSocket* socket = this->routeFrame(frame.can_id);
        if (socket == nullptr) {
            break;
        }

        // Building a packet from frames with CanPacket
        rc = socket->packet->pushFrame(frame.frame_data, frame.frame_size);
        if (rc < 0) {
            break;
        }

        if (socket->queueEnable && socket->packet->isPacketReady())
        {
            CanAdapterPacket* entry = (CanAdapterPacket*)socket->queue.reserve();
            if (!entry) {
                break;
            }

            socket->packet->readPacket(entry->packet_data);
        }
    }

    return 0;
}

int CanAdapter::socketRead(int socketId, uint8_t* buffer, uint32_t lenght)
{
    CanSocket* socket = this->getSocketById(socketId);
    if (socket == nullptr) {
        return -1;
    }

    if (socket->queueEnable) {
        if (socket->queue.empty()) {
            return 0;
        }
        void* entry = socket->queue.remove();
        if (!entry) {
            return -1;
        }
    }
    int rc = socket->packet->readPacket(buffer, lenght);
    if (rc < 0) {
        return -1;
    }

    return 0;
}

int CanAdapter::findFreeSocket(uint32_t myId)
{
    if (!this->initialized) {
        return -1;
    }

    int id = -1;

    for (int i = 0; i < CAN_ADAPTER_SOCKETS_MAX; i++)
    {
        if (this->socket[i].myId == myId) {
            return -1;
        }

        if (!this->socket[i].socketOpen && id == -1) {
            id = i;
        }
    }

    return id;
}

CanSocket* CanAdapter::getSocketById(int socketId)
{
    if (!this->initialized) {
        return nullptr;
    }

    if (socketId >= CAN_ADAPTER_SOCKETS_MAX) {
        return nullptr;
    }

    if (!this->socket[socketId].socketOpen) {
        return nullptr;
    }

    return &this->socket[socketId];
}

