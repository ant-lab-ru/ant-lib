#include "ant-lib/scheduler-WRR.h"

template<int N>
int SchedulerWRR<N>::get_index()
{
    return 2;
}

template<int N>
void SchedulerWRR<N>::index_empty()
{
    return;
}
