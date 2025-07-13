// SPDX-License-Identifier: MIT
// ft800-reset-red-demo.cpp
//   gcc -std=c11  ft800-reset-red-demo.cpp -o ft800-reset-red-demo
//   g++ -std=c++17 … likewise works


// Screen flashes red 

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "ft800_uapi.h"
#include "ft800_regs.h"        // our slim user header

static int wr8(int fd, uint32_t addr, uint8_t value)
{
    ft800_uapi_rw8 w{ addr, value };
    return ioctl(fd, FT800_IOCTL_WR8, &w);
}

int main()
{
    int fd = open("/dev/ft800", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open /dev/ft800");
        return 1;
    }

    /* 1️⃣  Enable pixel clock (PCLK = 48 MHz / 5 = 9.6 MHz) */
    if (wr8(fd, FT800_REG_PCLK, 5)) {
        perror("WR8 PCLK");
        close(fd);
        return 1;
    }
    puts("→ PCLK enabled");

    /* 2️⃣  Soft-reset, keep existing frame */
    ft800_uapi_reset rst{};
    if (ioctl(fd, FT800_IOCTL_RESET, &rst) < 0) {
        perror("FT800_IOCTL_RESET");
        close(fd);
        return 1;
    }
    puts("→ FIFO pointers cleared");

    /* 3️⃣  Build & submit a clear-to-red co-proc list */
    const uint32_t cmds[] = {
        CMD_DLSTART,
        CLEAR_COLOR_RGB(255, 0, 0),
        CLEAR(1,1,1),
        DISPLAY(),
        CMD_SWAP
    };
    ft800_uapi_cmdlist cl{
        .addr     = FT800_RAM_CMD,
        .len      = static_cast<uint32_t>(sizeof(cmds)),
        .user_ptr = reinterpret_cast<uintptr_t>(cmds)
    };
    if (ioctl(fd, FT800_IOCTL_SUBMIT_CMDS, &cl) < 0) {
        perror("FT800_IOCTL_SUBMIT_CMDS");
        
        close(fd);
        return 1;
    }
    printf("→ Submitted %zu commands\n", sizeof(cmds) / sizeof(cmds[0]));

    /* 4️⃣  Small delay for the co-processor */
    usleep(50'000);

    /* 5️⃣  Request DL swap via single-byte register write */
    if (wr8(fd, FT800_REG_DLSWAP, DLSWAP_FRAME)) {
        perror("WR8 DLSWAP");
    
        

        close(fd);
        return 1;
    }
    puts("→ DLSWAP_FRAME issued – screen should now be red");

       
    close(fd);
    return 0;
}
