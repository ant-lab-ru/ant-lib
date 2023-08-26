#include <stdint.h>

#define EHAS_MAX_DEVICES 50

typedef struct {
    uint16_t error_counter;
    uint8_t error_type;
    uint8_t reserved;
    char* error_name;
} ehas_error_pack_t;

template<uint32_t L>
class EhasPack
{
    char* device;
    ehas_error_pack_t errors[L];
};

typedef struct {
    EhasPack* pack;
} ehas_device_t;

class Ehas
{
    public:
        int init();
        int add(EhasPack* device);
        int error_report();

    private:
        ehas_device_t devices[EHAS_MAX_DEVICES];
};
