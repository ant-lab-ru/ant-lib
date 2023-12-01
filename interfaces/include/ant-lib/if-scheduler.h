#pragma once

#include <stdint.h>

class IScheduler
{
    public:
        virtual int get_index() = 0;
        virtual void index_empty() = 0;
};
