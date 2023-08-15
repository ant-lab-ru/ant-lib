#include "ant-lib/can-adapter.h"

typedef struct {
    uint16_t packetSize;
    uint8_t  packetData[];
} CanAdapterPacket;

int CanAdapter::init(ICan* iCan)
{
    if (!iCan) {
        return -1;
    }

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
    if (!initStruct) {
        return -1;
    }
    if (!initStruct->iCanPacket) {
        return -1;
    }

    int newId = this->findFreeSocket(initStruct->myCanId);
    if (newId < 0) {
        return -1;
    }

    CanSocket* socket = &this->socket[newId];

    // Init main fields
    socket->queueEnable = false;
    socket->myCanId = initStruct->myCanId;
    socket->dstCanId = initStruct->dstCanId;
    socket->packet = initStruct->iCanPacket;
    socket->packetDataMaxSize = initStruct->packetBufferLenght;

    int rc = socket->packet->init(CAN_FRAME_MAX_DATA_LEN, initStruct->packetBuffer, initStruct->packetBufferLenght);
    if (rc < 0) {
        return -1;
    }

    // If iCanPacket init ok -> socket open
    socket->socketOpen = true;

    // If queue is used
    if (!initStruct->queueBuffer)
    {
        if (initStruct->queueBufferLenght < (initStruct->packetBufferLenght + sizeof(uint16_t))) {
            return -1;
        }

        uint32_t element_size = initStruct->queueBufferLenght / initStruct->packetBufferLenght;
        socket->queue.init(initStruct->packetBufferLenght, element_size, initStruct->queueBuffer);

        socket->queueEnable = true;
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
    uint32_t bytes_moved = 0;

    while(1)
    {
        int rc = socket->packet->popFrame(frame.frame_data, CAN_FRAME_MAX_DATA_LEN);
        if (rc < 0 || rc > CAN_FRAME_MAX_DATA_LEN) {
            return -1;
        }
        if (rc == 0) {
            break;
        }

        frame.frame_size = rc;
        frame.can_id = socket->dstCanId;

        rc = this->can->write(&frame);
        if (rc != frame.frame_size) {
            return -1;
        }

        bytes_moved += frame.frame_size;
    }

    return bytes_moved;
}

int CanAdapter::handle()
{
    can_frame_t frame = {0};

    // Exit when all data is received
    while(1)
    {
        // Reading low-level CAN
        int rc = this->can->read(&frame);
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

        // If queue enabled then put packet in it
        if (socket->queueEnable && socket->packet->isPacketReady())
        {
            CanAdapterPacket* entry = (CanAdapterPacket*)socket->queue.reserve();
            if (!entry) {
                break;
            }

            rc = socket->packet->readPacket(entry->packetData, socket->packetDataMaxSize);
            if (rc < 0) {
                return -1;
            }

            entry->packetSize = rc;
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

        CanAdapterPacket* entry = (CanAdapterPacket*)socket->queue.remove();
        if (!entry) {
            return -1;
        }

        if (entry->packetSize > socket->packetDataMaxSize) {
            return -1;
        }
        if (lenght < entry->packetSize){
            return -3;
        }

        memcpy(buffer, entry->packetData, entry->packetSize);
        return entry->packetSize;
    }
    else
    {
        int rc = socket->packet->readPacket(buffer, lenght);
        if (rc < 0) {
            return -1;
        }
        return rc;
    }

    return -1;
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

