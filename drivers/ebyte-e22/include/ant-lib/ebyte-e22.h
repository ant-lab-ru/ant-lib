#include "ant-lib/if-uart.h"
#include "ebyte-e22-types.h"

class EbyteE22
{
public:
    EbyteE22(uint16_t addr, uint8_t netid): _addr(addr), _netid(netid):{}

    int init(IUart* uart);
    int set_addr(uint16_t addr);
    int set_netid(uint8_t netid);
    int get_pid(uint64_t* pid);
    int send(uint8_t* data, uint32_t size);
    int recv(uint8_t* buffer, uint32_t lenght);

    int set_mode(ebyte_e22_mode_t mode);

    uint64_t pid;

private:
    int _write_register(uint8_t reg, uint8_t value);
    int _read_register(uint8_t reg, uint8_t* value);
    IUart* _uart;
    IGpio* _m0;
    IGpio* _m1;
    uint16_t _addr;
    uint8_t _netid;
};