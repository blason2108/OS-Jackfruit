#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <string.h>

#define STACK_SIZE (1024 * 1024)

static char stack[STACK_SIZE];

int container_main(void *arg) {
    printf("Inside container!\n");

    // Set hostname for container
    sethostname("mycontainer", strlen("mycontainer"));

    // Change root filesystem
    if (chroot("rootfs-alpha") != 0) {
        perror("chroot failed");
        return 1;
    }

    if (chdir("/") != 0) {
        perror("chdir failed");
        return 1;
    }

    // Mount /proc inside container
    if (mount("proc", "/proc", "proc", 0, NULL) != 0) {
        perror("mount /proc failed");
        return 1;
    }

    // Start shell
    execlp("/bin/bash", "/bin/bash", NULL);

    perror("execlp failed");
    return 1;
}

int main() {
    int flags = CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS;

    pid_t pid = clone(container_main, stack + STACK_SIZE, flags | SIGCHLD, NULL);

    if (pid == -1) {
        perror("clone failed");
        exit(1);
    }

    waitpid(pid, NULL, 0);
    printf("Container exited\n");

    return 0;
}
