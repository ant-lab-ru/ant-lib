#include "ant-lib/ehas.h"
#include <gtest/gtest.h>


TEST(EhasTester, Test1)
{
    Ehas<50> ehas;

    const uint16_t len = 10;
    const uint16_t name_len = 30;

    char names[len][name_len] = {0};
    uint16_t counters[len] = {0};
    ehas_report_t reports[len] = {0};

    ehas_reports_t r = {0};
    r.reports_len = len;
    r.name_max_len = name_len;
    r.reports = reports;

    for(int i = 0; i < len; i++) {
        reports[i].counter = &counters[i];
        reports[i].name = names[i];
    }

    EXPECT_EQ( ehas.get_error_report(&r), true );
}