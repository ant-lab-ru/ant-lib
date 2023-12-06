#include "ant-lib/scheduler-WRR.h"
#include <gtest/gtest.h>

constexpr uint32_t N = 50;

TEST(SchedulerWRR, Empty)
{
    SchedulerWRR<N> s;

    uint16_t indexes[N] = {0};

    for (int i = 0; i < N; i++) {
        int idx = s.get_index();
        s.index_empty();

        EXPECT_GE( idx, 0 );
        EXPECT_LE( idx, N );
        indexes[idx]++;
    }

    for (int i = 0; i < N; i++) {
        EXPECT_EQ( indexes[i], 1 );
    }
}

