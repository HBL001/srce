/* draw_rect_fifo.cpp  –  one rectangle via FT800 command FIFO
 *
 * build : g++ -std=c++17 -Wall -O2 draw_rect_fifo.cpp -o DrawRectFifo
 * run   : ./DrawRectFifo
 */
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <endian.h>

#include "ft800_uapi.h"
#include "ft800_regs.h"

/* wait until CMD FIFO is idle (RD == WR and RD != 0x0FFF) */
static bool wait_fifo_idle(int fd, unsigned timeout_ms = 250)
{
    ft800_status st{};
    const unsigned step_us = 1'000;                 // 1 ms

    for (unsigned t = 0; t < timeout_ms; ++t) {
        if (ioctl(fd, FT800_IOCTL_GET_STATUS, &st) == 0) {
            uint16_t rd = le16toh(st.cmd_read);
            uint16_t wr = le16toh(st.cmd_write);
            if (rd != 0x0FFF && rd == wr)
                return true;
        }
        usleep(step_us);
    }
    return false;
}

int main()
{
    int fd = open("/dev/ft800", O_RDWR | O_CLOEXEC);
    if (fd < 0) { perror("open"); return 1; }

    /* reset pointers – driver uploads its own 20-byte blue screen DL */
    ioctl(fd, FT800_IOCTL_CLEAR_DL, 0);

    /* wait until that internal list has been consumed */
    if (!wait_fifo_idle(fd, 2000)) {
        fprintf(stderr, "FIFO never became idle!\n");
        return 1;
    }

    /* complete command list we want to execute */
    static const uint32_t cmds[] = {
        CMD_DLSTART,
        CLEAR_COLOR_RGB(0,0,0),  CLEAR(1,1,1),

        COLOR_RGB(0,250,0),
        BEGIN(FT8_RECTS),
            VERTEX2F( 80*16,  40*16),
            VERTEX2F(400*16, 200*16),
        END(),

        DISPLAY(),
        CMD_SWAP
    };

    /* package for SUBMIT_CMDS – the driver handles everything */
    ft800_uapi_cmdlist cl {
        .addr     = FT800_RAM_CMD,
        .len      = static_cast<__u32>(sizeof(cmds)),
        .user_ptr = reinterpret_cast<__u64>(cmds)
    };

    if (ioctl(fd, FT800_IOCTL_SUBMIT_CMDS, &cl) < 0) {
        perror("FT800_IOCTL_SUBMIT_CMDS");
        return 1;
    }

    /* wait for the co-processor to run the list & swap DLs */
    wait_fifo_idle(fd, 100);

    /* give the LCD one frame to refresh */
    usleep(20'000);
    close(fd);
    return 0;
}
