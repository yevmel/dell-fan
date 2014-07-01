/* #undef VERBOSE */

typedef struct _config_item {
    // limit before decreasing fan speed
    int temp_min;

    // limit before increasing fan speed
    int temp_max;

    // value representing the fan speed (0, 1 or 2 for my device)
    int value;

    // time spend before checking if we need to change the fan speed
    int sleep;
} config_item;

config_item config[] = {
    // fan disabled
    {0, 55, 0, 5},

    // slow
    {52, 60, 1, 3},

    // full power
    {57, 999, 2, 10}
};

#define READ_TEMP		_IOR ('i', 0x84, size_t)
#define READ_FAN		_IOWR('i', 0x86, size_t)
#define WRITE_FAN		_IOWR('i', 0x87, size_t)
#define SECOND_FAN		1
#define FIRST_FAN		0

#define PROC_FILE		"/proc/i8k"

// index of the config_item in config[] to enable when daemon is disabled (f.e. by SIGTERM)
#define RESET_CONFIG_ITEM_IDX 1
