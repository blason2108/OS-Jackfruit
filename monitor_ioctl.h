#ifndef MONITOR_IOCTL_H
#define MONITOR_IOCTL_H

#include <linux/ioctl.h>

#define MONITOR_NAME_LEN 32

struct monitor_request {
    pid_t pid;
    char container_id[MONITOR_NAME_LEN];
    unsigned long soft_limit_bytes;
    unsigned long hard_limit_bytes;
};

#define MONITOR_REGISTER _IOW('m', 1, struct monitor_request)
#define MONITOR_UNREGISTER _IOW('m', 2, struct monitor_request)

#endif
