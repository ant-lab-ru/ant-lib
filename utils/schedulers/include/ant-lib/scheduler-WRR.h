#pragma once

#include "stdint.h"

#include "ant-lib/if-scheduler.h"

template<int N>
class SchedulerWRR : public IScheduler
{
public:
    SchedulerWRR(){};

    int get_index();
    void index_empty();

    uint8_t priorities[N] = {0};

private:
    int last_index;
    uint8_t counters[N] = {0};
};
