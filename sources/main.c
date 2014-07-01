#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "config.h"

int running = 1;
static int i8k_proc_fd;

void catch_termination_signal() {
    fprintf(stdout, "termination signal received.\n");
    running = 0;
}

int set_fan(int fan, int value) {
    int args[2];

    args[0] = fan;
    args[1] = value;

    fprintf(stdout, "setting fan speed to %d.\n", value);
    return ioctl(i8k_proc_fd, WRITE_FAN, &args);
}

int read_fan(int fan) {
    int args[1];
    int return_code;

    args[0] = fan;
    return_code = ioctl(i8k_proc_fd, READ_FAN, &args);

    if (return_code < 0) {
    	return return_code;
    } else {
        return args[0];
    }
}

int read_cpu_temp() {
    int args[1];
    int return_code = ioctl(i8k_proc_fd, READ_TEMP, &args);

    if (return_code < 0) {
    	return return_code;
    }

    return args[0];
}

int main(int argc, char **argv) {
    // we start with fan disabled
    int current_config_item_idx = 0;

    if (signal(SIGINT, catch_termination_signal) == SIG_ERR) {
        fprintf(stderr, "failed to setup signal handler for SIGINT.\n");
    }

    if (signal(SIGTERM, catch_termination_signal) == SIG_ERR) {
        fprintf(stderr, "failed to setup signal handler for SIGTERM.\n");
    }

    i8k_proc_fd = open(PROC_FILE, O_RDONLY);
    if (i8k_proc_fd < 0) {
        fprintf(stderr, "failed to open %s.\n", PROC_FILE);
        exit(EXIT_FAILURE);
    }

    while(running) {
        int current_temp = read_cpu_temp();
        fprintf(stdout, "current cpu temp is %d.\n", current_temp);

        config_item *current_config_item = &config[current_config_item_idx];
        fprintf(stdout, "current config_item index is %d.\n", current_config_item_idx);

        if(current_config_item->temp_max <= current_temp) {
            current_config_item_idx++;
            
        } else if (current_config_item->temp_min >= current_temp) {
            current_config_item_idx--;
        }

        current_config_item = &config[current_config_item_idx];

        set_fan(FIRST_FAN, current_config_item->value);

        fprintf(stdout, "sleeping for %d.\n", current_config_item->sleep);
        sleep(current_config_item->sleep);
    }

    // we do not want to leave the fan in a undefined state when we stop the deamon,
    // so we switch to a configured default state.
    set_fan(FIRST_FAN, config[RESET_CONFIG_ITEM_IDX].value);
    return 0;
}
