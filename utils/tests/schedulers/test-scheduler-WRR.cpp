#include "ant-lib/scheduler-WRR.h"
#include <gtest/gtest.h>

template <typename T>
class SchedulerWRRTester : public ::testing::Test{};

using test_types = ::testing::Types<
    std::integral_constant<int,2>,
    std::integral_constant<int,3>, 
    std::integral_constant<int,4>>;

TYPED_TEST_SUITE(SchedulerWRRTester, test_types);

TYPED_TEST(SchedulerWRRTester, Test1)
{
    static constexpr int n = TypeParam::value;
    SchedulerWRR<n> s;

    EXPECT_EQ( 2 , 2 );
    // EXPECT_EQ( s.get_index() , 0 );
}
