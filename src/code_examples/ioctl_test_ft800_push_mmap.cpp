// SPDX-License-Identifier: MIT
/**
 * @file    ft800-demo.c
 * @brief   Comprehensive FT800 demo
 *
 *          • CLEAR_DL to reset FIFO pointers
 *          • wait_fifo_idle() helper (robust – accounts for –EAGAIN)
 *          • dump a few important registers
 *          • dump first 64 bytes of RAM_CMD
 *
 * build :  gcc -std=c17 -Wall -O2 ft800-demo.c -o FtDemo
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <endian.h>               /* le16toh() */

#include "ft800_uapi.h"           /* IOCTLs, structs */
#include "ft800_regs.h"                /* register-, CMD-macros */

/* ------------------------------------------------------------ */

#ifndef FT800_RAM_CMD_START
#  define FT800_RAM_CMD_START 0x108000UL
#endif

#define RAM_CMD_DUMP_BYTES  64U

/* ──────────────────────────────────────────────────────────── */
/*  Helper: wait until CMD FIFO is idle (RD == WR, RD ≠ 0x0FFF) */
static bool wait_fifo_idle(int fd, unsigned timeout_ms)
{
    ft800_status st{};
    for (unsigned ms = 0; ms < timeout_ms; ++ms) {
        if (ioctl(fd, FT800_IOCTL_GET_STATUS, &st) == 0) {
            uint16_t rd = le16toh(st.cmd_read);
            uint16_t wr = le16toh(st.cmd_write);

            if (rd == wr && rd != 0x0FFF)
                return true;          /* idle & not in “busy-sentinel” */
        } else if (errno != EAGAIN) { /* ignore transient –EAGAIN       */
            perror("GET_STATUS");
            return false;
        }
        usleep(1000);                 /* 1 ms                           */
    }
    errno = ETIMEDOUT;
    return false;
}
/* ──────────────────────────────────────────────────────────── */

static int read_reg32(int fd, uint32_t addr, uint32_t *out)
{
    struct ft800_mem_op op = { .addr = addr, .len = sizeof(*out) };
    if (ioctl(fd, FT800_IOCTL_MEMREAD, &op) < 0)
        return -1;
    memcpy(out, op.data, sizeof(*out));
    return 0;
}

static int read_reg16(int fd, uint32_t addr, uint16_t *out)
{
    uint32_t tmp;
    if (read_reg32(fd, addr, &tmp) < 0) return -1;
    *out = (uint16_t)(tmp & 0xFFFFu);
    return 0;
}

/* ------------------------------------------------------------ */

int main(void)
{
    int fd = open("/dev/ft800", O_RDWR | O_CLOEXEC);
    if (fd < 0) { perror("open /dev/ft800"); return 1; }

    /* 0) reset FIFO pointers (driver may pulse PD_N → EAGAIN) */
    if (ioctl(fd, FT800_IOCTL_CLEAR_DL, 0) < 0) {
        perror("FT800_IOCTL_CLEAR_DL");
        close(fd);
        return 1;
    }

    /* 1) wait until the co-processor is really idle */
    if (!wait_fifo_idle(fd, 500)) {           /* 0.5 s max */
        perror("wait_fifo_idle");
        close(fd);
        return 1;
    }

    /* 2) dump a few registers and the first 64 bytes of RAM_CMD */
    uint32_t id, pclk, dlswap;
    uint16_t cmd_rd, cmd_wr;

    if (read_reg32(fd, FT800_REG_ID, &id) == 0)
        printf("REG_ID:        0x%08X\n", id);

    if (read_reg32(fd, FT800_REG_PCLK, &pclk) == 0)
        printf("REG_PCLK:      0x%08X\n", pclk);

    if (read_reg32(fd, FT800_REG_DLSWAP, &dlswap) == 0)
        printf("REG_DLSWAP:    0x%08X\n", dlswap);

    if (read_reg16(fd, FT800_REG_CMD_READ,  &cmd_rd) == 0)
        printf("REG_CMD_READ:  0x%04X\n", cmd_rd);

    if (read_reg16(fd, FT800_REG_CMD_WRITE, &cmd_wr) == 0)
        printf("REG_CMD_WRITE: 0x%04X\n\n", cmd_wr);

    puts("RAM_CMD (first 64 bytes):");
    if (lseek(fd, FT800_RAM_CMD_START, SEEK_SET) == -1) {
        perror("lseek RAM_CMD");
    } else {
        uint8_t buf[RAM_CMD_DUMP_BYTES];
        ssize_t got = read(fd, buf, sizeof buf);
        if (got < 0) {
            perror("read RAM_CMD");
        } else {
            for (ssize_t i = 0; i < got; i += 16) {
                printf("0x%06lX:", (unsigned long)(FT800_RAM_CMD_START + i));
                for (ssize_t j = 0; j < 16 && i + j < got; ++j)
                    printf(" %02X", buf[i + j]);
                putchar('\n');
            }
        }
    }

    close(fd);
    return 0;
}
