/* test_rect.cpp – minimal FT800 cmd-FIFO push in C++
 *
 * Build :  g++ -std=c++17 -Wall -O2 test_rect.cpp -o test_rect
 * Run   :  ./test_rect
 * SPDX-License-Identifier: MIT
 */
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "ft800_regs.h"
#include "ft800_uapi.h"

/* ── small helper ───────────────────────────────────────────────────── */
static inline void sleep_ms(unsigned ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static int fifo_drain(int fd, int timeout_ms)
{
    ft800_status st{};
    int waited = 0;

    while (true)
    {
        if (ioctl(fd, FT800_IOCTL_GET_STATUS, &st) < 0)
            return -errno;

        if (st.cmd_read == st.cmd_write)
            return 0;

        sleep_ms(10);
        waited += 10;
        if (waited >= timeout_ms)
            return -ETIMEDOUT;
    }
}

/* main ──────────────────────────────────────────────────────────────── */
int main()
{
    int fd = open("/dev/ft800", O_RDWR | O_CLOEXEC);
    if (fd < 0) { perror("open /dev/ft800"); return EXIT_FAILURE; }

    /* 1. reset co-processor + DL pointers */
    if (ioctl(fd, FT800_IOCTL_CLEAR_DL, 0) < 0)
    {
        perror("CLEAR_DL");
        close(fd);
        return EXIT_FAILURE;
    }

    /* 2. five 32-bit words (little-endian) */
    constexpr std::uint32_t cmd[] = {
        CMD_DLSTART,
        CLEAR_COLOR_RGB(0, 0, 60),   // dark blue background
        CLEAR(1, 1, 1),
        CMD_SWAP,                    // finish list (no text for brevity)
        0                            // keep FIFO slot #5 empty (optional)
    };

    /* 3. package into a CMD_WRITE request                              */
    struct CmdWrite
    {
        std::uint32_t off;
        std::uint32_t len;
        std::uint8_t  payload[sizeof(cmd)];
    } req{ 4, sizeof(cmd) };

    std::memcpy(req.payload, cmd, sizeof(cmd));

    if (ioctl(fd, FT800_IOCTL_CMD_WRITE, &req) < 0)
    {
        if (errno == EAGAIN)
            std::fprintf(stderr, "FIFO full – retry later\n");
        else
            perror("CMD_WRITE");
        close(fd);
        return EXIT_FAILURE;
    }

    /* 4. wait until executed                                           */
    int rc = fifo_drain(fd, 500);
    if (rc)
        std::fprintf(stderr, "timeout (%d)\n", rc);

    close(fd);
    return rc ? EXIT_FAILURE : EXIT_SUCCESS;
}
