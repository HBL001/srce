/* SPDX-License-Identifier: GPL-2.0+
 *
 * ft800.h – *user-space* convenience header for Bridgetek FT800 / FT843
 * --------------------------------------------------------------------
 *  * No kernel headers – only <stdint.h> required.
 *  * Macros, enums and addresses taken from FTDI “FT800 Series
 *    Programmer’s Guide”, v2.1 (BRT_000030, Jul-2022).
 *  * Matches the symbols referenced by the in-tree kernel driver
 *    but strips out struct ft800_dev and other kernel-only stuff.
 *
 * 2025-07-12  (temporary copy until meta-custom exports the real UAPI)
 */

#ifndef _FT800_USER_H_
#define _FT800_USER_H_

#include <stdint.h>           /* uint32_t -- stdint is all we need      */
#include "ft800_uapi.h"       /* ioctl numbers & struct ft800_cmd …     */

/* ------------------------------------------------------------------------ */
/* 1. Generic helpers                                                       */
/* ------------------------------------------------------------------------ */
#define FT800_SHIFT_BYTE0   0U
#define FT800_SHIFT_BYTE1   8U
#define FT800_SHIFT_BYTE2   16U
#define FT800_SHIFT_BYTE3   24U

#define FT800_ADDR_MASK     0x00FFFFFFu     /* 24-bit address bus        */

/* ------------------------------------------------------------------------ */
/* 2. FT800 memory map                                                      */
/* ------------------------------------------------------------------------ */
#define FT800_RAM_G_START        0x000000UL
#define FT800_RAM_G_SIZE         (256UL * 1024UL)

#define FT800_RAM_DL_START       0x100000UL
#define FT800_RAM_DL_SIZE        (8UL   * 1024UL)

#define FT800_REG_START          0x102400UL
#define FT800_REG_SIZE           0x0380UL

#define FT800_RAM_CMD_START      0x108000UL
#define FT800_RAM_CMD_SIZE       (4UL   * 1024UL)

#define FT800_MAX_MEM            (FT800_RAM_CMD_START + FT800_RAM_CMD_SIZE)

/* handy aliases to match the kernel source */
#define FT800_RAM_G              FT800_RAM_G_START
#define FT800_RAM_DL             FT800_RAM_DL_START
#define FT800_RAM_CMD            FT800_RAM_CMD_START
#define FT800_CMD_FIFO_SIZE      FT800_RAM_CMD_SIZE

/* ------------------------------------------------------------------------ */
/* 3. Register addresses (only the ones user-space ever touches)            */
/* ------------------------------------------------------------------------ */
#define FT800_REG_ID                 0x102400UL
#define FT800_REG_DLSWAP             0x102450UL
#define FT800_REG_CMD_READ           0x1024E4UL
#define FT800_REG_CMD_WRITE          0x1024E8UL

#define DLSWAP_FRAME                 0x02u       /* value for REG_DLSWAP   */

/* ------------------------------------------------------------------------ */
/* 4. Display-List (DL) op-codes                                            */
/* ------------------------------------------------------------------------ */
#define FT800_DL_OP_CLEAR_COLOR_RGB  0x02U
#define FT800_DL_OP_CLEAR            0x26U
#define FT800_DL_OP_DISPLAY          0x00U
#define FT800_DL_OP_BEGIN            0x1FU
#define FT800_DL_OP_END              0x21U
#define FT800_DL_OP_COLOR_RGB        0x04U
#define FT800_DL_OP_VERTEX2F         0x40U

/* --- helpers to compose 32-bit DL words --------------------------------- */
#define CLEAR_COLOR_RGB(r,g,b) \
        (((uint32_t)FT800_DL_OP_CLEAR_COLOR_RGB << 24) | \
         ((uint32_t)((r) & 0xFF) << 16) | \
         ((uint32_t)((g) & 0xFF) << 8)  | \
         ((uint32_t)((b) & 0xFF)))

#define CLEAR(c,s,t) \
        (((uint32_t)FT800_DL_OP_CLEAR << 24) | \
         (((c) & 1U) << 2) | (((s) & 1U) << 1) | ((t) & 1U))

#define DISPLAY()                   0x00000000UL

#define BEGIN(prim) \
        (((uint32_t)FT800_DL_OP_BEGIN << 24) | ((uint32_t)(prim) & 0xFFU))

#define END()                       ((uint32_t)FT800_DL_OP_END << 24)

#define COLOR_RGB(r,g,b) \
        (((uint32_t)FT800_DL_OP_COLOR_RGB << 24) | \
         ((uint32_t)((r) & 0xFF) << 16) | \
         ((uint32_t)((g) & 0xFF) << 8)  | \
         ((uint32_t)((b) & 0xFF)))

#define VERTEX2F(x,y) \
        (((uint32_t)FT800_DL_OP_VERTEX2F << 24) | \
         ((((uint32_t)(x) & 0x3FFFU) << 14) | \
          (((uint32_t)(y) & 0x3FFFU) << 2)))

/* Primitive IDs for BEGIN() --------------------------------------------- */
#define FT800_PRIM_POINTS          0x00U
#define FT800_PRIM_LINES           0x01U
#define FT800_PRIM_LINE_STRIP      0x02U
#define FT800_PRIM_RECTS           0x09U
/* (add more if you need them) */

/* ------------------------------------------------------------------------ */
/* 5. Co-processor command tokens (CMD_*)                                   */
/* ------------------------------------------------------------------------ */
#define CMD_DLSTART        0xFFFFFF00UL
#define CMD_SWAP           0xFFFFFF01UL
#define CMD_TEXT           0x0C000000UL
#define CMD_BUTTON         0x0D000000UL
#define CMD_NUMBER         0x0E000000UL
#define CMD_SLIDER         0x10000000UL
#define CMD_TOGGLE         0x11000000UL
#define CMD_CLOCK          0x12000000UL
#define CMD_DIAL           0x13000000UL
#define CMD_KEYS           0x1A000000UL
#define CMD_GRADIENT       0xFFFFFF2DUL
#define CMD_MEMWRITE       0xFFFFFF1AUL
#define CMD_MEMSET         0xFFFFFF1BUL
#define CMD_MEMCPY         0xFFFFFF1DUL

/* (aliases used by some sample code) */
#define FT800_CMD_TEXT         CMD_TEXT
#define FT800_CMD_BUTTON       CMD_BUTTON
#define FT800_CMD_NUMBER       CMD_NUMBER
#define FT800_CMD_SLIDER       CMD_SLIDER
#define FT800_CMD_TOGGLE       CMD_TOGGLE
#define FT800_CMD_CLOCK        CMD_CLOCK
#define FT800_CMD_DIAL         CMD_DIAL
#define FT800_CMD_KEYS         CMD_KEYS
#define FT800_CMD_GRADIENT     CMD_GRADIENT
#define FT800_CMD_MEMWRITE     CMD_MEMWRITE
#define FT800_CMD_MEMSET       CMD_MEMSET
#define FT800_CMD_MEMCPY       CMD_MEMCPY

/* ------------------------------------------------------------------------ */
/* Nothing below this line is needed in user-space                          */
/* ------------------------------------------------------------------------ */

#endif /* _FT800_USER_H_ */
