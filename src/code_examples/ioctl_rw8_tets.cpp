// 8 bit read and write test

// root@beaglebone-yocto:/home/user# ./Test
// ft800 spi0.0: ft800_wr8: tx=[90 24 8C AA] addr=0x10248C v=0xAA
// ft800 spi0.0: ft800_rd16: addr=0x10248C rx=[42 AA 00] val=0x00AA
// ft800 spi0.0: ft800_rd8: addr=0x10248C tmp=0x00AA => v=0xAA
// GPIO_DIR=0xAA

#include <cstdio>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ft800_uapi.h"
#include "ft800_regs.h"

int main() {
    int fd = open("/dev/ft800", O_RDWR|O_CLOEXEC);
    if (fd < 0) return perror("open"), 1;

    ft800_uapi_rw8 w{FT800_REG_GPIO_DIR, 0xAA};
    if (ioctl(fd, FT800_IOCTL_WR8, &w) < 0) perror("WR8");

    ft800_uapi_rw8 r{FT800_REG_GPIO_DIR};
    if (ioctl(fd, FT800_IOCTL_RD8, &r) == 0)
        printf("GPIO_DIR=0x%02X\n", r.value);
    else
        perror("RD8");

    return 0;
}
