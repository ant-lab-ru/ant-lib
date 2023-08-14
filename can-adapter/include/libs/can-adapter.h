#pragma once

#include "can-interface.h"
#include "include/libs/static-queue.h"

class CanSocket
{
    public:
        bool socket_open; // true если сокет открыт
        uint32_t src_id; // CAN ID нужный для настройки фильтра и приема кадров, пакетов
        uint32_t dst_id; // CAN ID который добавляется в передаваемое сообщение

        StaticQueue queue;
};

class CanAdapter
{
    public:
        int init(ICan* ican);
        int deinit();
        int handle();

        int socket_open();
        int socket_close();
        int socket_read();
        int socket_write();

    private:
        ICan* can;
        CanSocket sockets[10];
};