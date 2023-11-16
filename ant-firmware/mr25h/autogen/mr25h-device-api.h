
typedef enum {
    MR25H_UNLOCK = 0,
    MR25H_UPPER_QUARTER_LOCKED,
    MR25H_UPPER_HALF_LOCKED,
    MR25H_ALL_LOCKED,
} mr25h_lock_t;

class Mr25hApi
{
    public:
        mr25h_lock_t lock;
        int onLockUpdate();
};