#pragma once

template<int N>
class ShedulerWRR : public IScheduler
{
public:
    uint8_t priorities[N] = {0};

    int get_chan_index();
    void last_chan_empty();

private:
    int last_index;
    uint8_t counters[N] = {0};
};
