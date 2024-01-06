#include "ant-lib/ebyte-e22.h"

int EbyteE22::init(IUart* uart, IGpio* m0, IGpio* m1)
{
    _uart = uart;
    _m0 = m0;
    _m1 = m1;
    _set_mode(EBYTE_E22_MODE_CONFIG);

    get_pid(&pid);
    _set_addr(_addr);
    _set_netid(_netid);
    
    _set_mode(EBYTE_E22_MODE_WOR);
    return 0;
}

int EbyteE22::send(uint8_t* data, uint32_t size)
{
    return _uart.write(data, size);
}

int EbyteE22::recv(uint8_t* buffer, uint32_t lenght)
{
    return _uart.read(buffer, lenght);
}

int EbyteE22::set_addr(uint16_t addr)
{
    _set_register(EBYTE_E22_ADDRH_REG_ADDR, (addr & 0xFF00) >> 8);
    _set_register(EBYTE_E22_ADDRL_REG_ADDR, (addr & 0x00FF) >> 0);
    _addr = addr;
}

int EbyteE22::set_netid(uint8_t netid)
{
    _set_register(EBYTE_E22_NETID_REG_ADDR, netid);
    _netid = netid;
}

int EbyteE22::get_pid(uint64_t* pid)
{
    _get_registers((uint8_t*)pid, EBYTE_E22_PID_REG_SIZE);
}

int EbyteE22::set_mode(ebyte_e22_mode_t mode)
{
    _m0.set(!!(mode & 0x1));
    _m1.set(!!(mode & 0x2));
    return 0;
}

int EbyteE22::_write_register(uint8_t reg, uint8_t value)
{
    uint8_t buf[] = {EBYTE_E22_CMD_WRITE, reg, 1, value};
    _uart.write(buf, sizeof(buf));
    return 0;
}

int EbyteE22::_read_registers(uint8_t reg, uint8_t* buffer, uint8_t lenght)
{
    uint8_t buf[] = {EBYTE_E22_CMD_READ, reg, lenght};
    _uart.write(buf, sizeof(buf));
    uint8_t read_buf[EBYTE_E22_RW_CMD_LEN] = {0};
    _uart.read(read_buf, EBYTE_E22_RW_CMD_LEN);
    if (memcmp(buf, read_buf)) {
        return -2;
    }
    _uart.read(buffer, lenght);
    return 0;
}