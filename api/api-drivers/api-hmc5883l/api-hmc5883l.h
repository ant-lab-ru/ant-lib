#pragma once

#include "ant-lib/hmc5883l.h"

class ApiHmc5883l
{
public:
    int hmc_config_reference_get(hmc5883l_data_t* data);
    int hmc_config_reference_set(hmc5883l_data_t data);

    int hmc_config_last_test_get(hmc5883l_data_t* data);
    int hmc_config_last_test_set(hmc5883l_data_t data);

    int hmc_config_self_test();

    int hmc_get_xyz(hmc5883l_data_t* data);
    int hmc_get_xyz_calibrated(hmc5883l_data_t* data);

private:
    
};
