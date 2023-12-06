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
    int _index = 0;
    int _priority = 0;
};

template<int N>
int SchedulerWRR<N>::get_index()
{
    for(int i = 0; i < N; i++)
    {
        _index++;

        if (_index > N) {
            _index = 0;
            _priority++;
        }

        if (_priority <= priorities[_index]) {
            return _index;
        }
    }

    _priority = 0;
    return _index;
}

template<int N>
void SchedulerWRR<N>::index_empty()
{
    return;
}
