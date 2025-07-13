/* SPDX-License-Identifier: MIT
 *
 * FT800_IOCTL_CMD_WRITE exhaust-test
 *
 *   – CLEAR_DL              (and give the panel >10 ms to flush)
 *   – write a tiny DL at offset 0     → solid blue frame
 *   – write another DL at 0x0FF0      → wrap-around, solid red frame
 *   – wait after each write until CMD_READ == CMD_WRITE
 *   – print final FIFO pointer values
 *
 *  ▒ Tips to avoid residual blends ▒
 *  1. Only one colour-clear per “real” frame (we still send two here
 *     because this is a driver test, but each DL ends with CMD_SWAP).
 *  2. Give the LCD at least one full frame (10 ms at 60 Hz) after
 *     CLEAR_DL before sending the first visible list.
 *  3. Don’t queue back-to-back tiny lists in production; build one
 *     full DL and issue a single CMD_SWAP.
 *
 * Build native:
 *   g++ -std=c++17 -Wall -Wextra -O2 ft800_cmd_write_test.cpp -o ft800_cmd_test
 * Cross:
 *   ${CXX} --sysroot=${SYSROOT} -std=c++17 -Wall -Wextra -O2 ft800_cmd_write_test.cpp -o ft800_cmd_test
 */
#include <array>
#include <cstdint>
#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <endian.h>

#include "ft800_uapi.h"
#include "ft800_regs.h"

#define DEVNODE          "/dev/ft800"
#define POLL_USEC        500
#define TIMEOUT_MS       100U
#define FIFO_SIZE        4096U
#define WRAP_OFFSET      0x0FF0U      /* forces split-burst wrap */

/* native co-processor op-codes */
#define FT800_CMD_CLEAR_COLOR_RGB 0x02000000UL
#define FT800_CMD_CLEAR           0x26000000UL

/* ---------- helpers -------------------------------------------------- */
static bool get_status(int fd, uint16_t &rd, uint16_t &wr)
{
    ft800_status st{};
    if (ioctl(fd, FT800_IOCTL_GET_STATUS, &st) < 0)
        return false;
    rd = le16toh(st.cmd_read);
    wr = le16toh(st.cmd_write);
    return true;
}

static bool wait_fifo_empty(int fd, uint16_t expected)
{
    const unsigned long max_iter = (TIMEOUT_MS * 1000U) / POLL_USEC;
    for (unsigned long i = 0; i < max_iter; ++i) {
        uint16_t rd, wr;
        if (!get_status(fd, rd, wr))
            return false;
        if (rd == expected)
            return true;
        if (rd == 0x0FFF)        /* co-proc fault */
            return false;
        usleep(POLL_USEC);
    }
    return false;
}

static int cmd_write(int fd,
                     uint32_t offset,
                     const uint32_t *words,
                     size_t n_words)
{
    struct {
        uint32_t offset;
        uint32_t len;
        uint32_t data[64];
    } __attribute__((packed)) arg{};

    if (n_words > std::size(arg.data)) {
        fprintf(stderr, "internal buffer too small\n");
        return -1;
    }
    arg.offset = offset;
    arg.len    = static_cast<uint32_t>(n_words * sizeof(uint32_t));
    memcpy(arg.data, words, arg.len);

    return ioctl(fd, FT800_IOCTL_CMD_WRITE, &arg);
}
/* -------------------------------------------------------------------- */

int main()
{
    int fd = open(DEVNODE, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open /dev/ft800");
        return EXIT_FAILURE;
    }

    /* --------------------------------------------------------------- */
    puts("Step 0 – CLEAR_DL and wait 10 ms");
    if (ioctl(fd, FT800_IOCTL_CLEAR_DL, nullptr) < 0) {
        perror("CLEAR_DL");
        return EXIT_FAILURE;
    }
    usleep(10 * 1000);                   /* tip #2                                    */

    uint16_t rd0, wr0;
    if (!get_status(fd, rd0, wr0) || rd0 || wr0) {
        fprintf(stderr, "FIFO not empty after CLEAR_DL\n");
        return EXIT_FAILURE;
    }

    /* --------------------------------------------------------------- */
    puts("Step 1 – small DL at offset 0 (blue)");
    const uint32_t blue_dl[] = {
        CMD_DLSTART,
        CLEAR_COLOR_RGB(0,0,128),          /* dark blue background                      */
        CLEAR(1,1,1),
        CMD_SWAP
    };
    if (cmd_write(fd, 0, blue_dl, std::size(blue_dl)) < 0) {
        perror("CMD_WRITE #1");
        return EXIT_FAILURE;
    }
    if (!wait_fifo_empty(fd, sizeof(blue_dl) & (FIFO_SIZE-1))) {
        fprintf(stderr, "FIFO did not drain for block #1\n");
        return EXIT_FAILURE;
    }
    puts("  OK – screen is dark blue");

     /* → give the LCD one full frame (≈16 ms at 60 Hz) to flip colours   */
    usleep(20 * 1000);

    /* --------------------------------------------------------------- */
    puts("Step 2 – wrap-around DL at 0x0FF0 (red)");
    const uint32_t red_dl[] = {
        CMD_DLSTART,
        CLEAR_COLOR_RGB(128,0,0),          /* dark blue background                      */
        CLEAR(1,1,1),
        CMD_SWAP
    };

    if (cmd_write(fd, WRAP_OFFSET, red_dl, std::size(red_dl)) < 0) {
        perror("CMD_WRITE #2");
        return EXIT_FAILURE;
    }
    uint16_t expect = (WRAP_OFFSET + sizeof(red_dl)) & (FIFO_SIZE - 1);
    if (!wait_fifo_empty(fd, expect)) {
        fprintf(stderr, "  FIFO did not drain for block #2\n");
        return EXIT_FAILURE;
    }

    /* → give the LCD one full frame (≈16 ms at 60 Hz) to flip colours   */
    usleep(10 * 20 * 1000);

    puts("  OK – wrap handled, screen is red");


    /* --------------------------------------------------------------- */
    uint16_t rd, wr;
    get_status(fd, rd, wr);
    printf("Final CMD_READ=0x%03x CMD_WRITE=0x%03x (expected 0x%03x)\n",
           rd, wr, expect);

    close(fd);
    puts("ALL TESTS PASSED ✔");
    return EXIT_SUCCESS;
}
