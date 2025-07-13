/* SPDX-License-Identifier: GPL-2.0+
 *
 * ft800_regs.hpp – Bridgetek FT800 (EVE1) register & DL definitions
 *                  for user-space applications
 *
 *  ▸ 100 % ABI-compatible with the kernel driver.
 *  ▸ No Linux kernel headers required – builds cleanly with -std=c++17.
 *  ▸ Only the pieces user-space can legitimately use:   constants,
 *    register addresses, DL opcodes & helper macros.
 */

#ifndef _FT800_REGS_HPP_
#define _FT800_REGS_HPP_

/* ------------------------------------------------------------------ */
/*  portable, kernel-free typedefs                                     */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
  #include <cstdint>
  #include <cstddef>
  extern "C" {
    using  u8  = std::uint8_t;
    using  u16 = std::uint16_t;
    using  u32 = std::uint32_t;
#else
  #include <stdint.h>
  typedef uint8_t  u8;
  typedef uint16_t u16;
  typedef uint32_t u32;
#endif

/* ------------------------------------------------------------------ */
/*  generic helpers                                                   */
/* ------------------------------------------------------------------ */

#define FT800_SHIFT_LSB   0U
#define FT800_SHIFT_MSB   8U
#define FT800_SHIFT_BYTE0 0U
#define FT800_SHIFT_BYTE1 8U
#define FT800_SHIFT_BYTE2 16U
#define FT800_SHIFT_BYTE3 24U

#define FT800_ADDR_MASK   0x00FFFFFFu
#define FT800_LE          0x00u
#define FT800_BE          0x01u

/* ------------------------------------------------------------------ */
/*  on-chip memory map                                                */
/* ------------------------------------------------------------------ */
#define FT800_RAM_G_START        0x000000UL
#define FT800_RAM_G_SIZE        (256UL * 1024UL)

#define FT800_ROM_FONT          0x0B0000UL
#define FT800_ROM_CHIPID        0x0C0000UL
#define FT800_ROM_FONT_ADDR     0x0FFFFCUL

#define FT800_RAM_DL_START      0x100000UL
#define FT800_RAM_DL_SIZE       (8UL  * 1024UL)

#define FT800_RAM_PAL_START     0x102000UL
#define FT800_PAL_SIZE          (1UL  * 1024UL)

#define FT800_REG_START         0x102400UL
#define FT800_REG_SIZE          0x0380UL

#define FT800_RAM_CMD_START     0x108000UL
#define FT800_RAM_CMD_SIZE      (4UL  * 1024UL)

#define FT800_RAM_SNAPSHOT_START 0x1C2000UL
#define FT800_RAM_SNAPSHOT_SIZE  (2UL  * 1024UL)

#define FT800_MAX_MEM (FT800_RAM_CMD_START + FT800_RAM_CMD_SIZE)

/* convenience aliases used by sample code -------------------------------- */
#define FT800_RAM_G         FT800_RAM_G_START
#define FT800_RAM_DL        FT800_RAM_DL_START
#define FT800_RAM_CMD       FT800_RAM_CMD_START
#define FT800_RAM_REG       FT800_REG_START
#define FT800_CMD_FIFO_SIZE FT800_RAM_CMD_SIZE

/* ------------------------------------------------------------------ */
/*  address-range helpers                                             */
/* ------------------------------------------------------------------ */
#define FT800_IS_RAM_G(a,l)   (((a) >= FT800_RAM_G_START)   && (((a)+(l)) <= (FT800_RAM_G_START   + FT800_RAM_G_SIZE)))
#define FT800_IS_RAM_DL(a,l)  (((a) >= FT800_RAM_DL_START)  && (((a)+(l)) <= (FT800_RAM_DL_START  + FT800_RAM_DL_SIZE)))
#define FT800_IS_REG(a,l)     (((a) >= FT800_REG_START)     && (((a)+(l)) <= (FT800_REG_START     + FT800_REG_SIZE)))
#define FT800_IS_RAM_CMD(a,l) (((a) >= FT800_RAM_CMD_START) && (((a)+(l)) <= (FT800_RAM_CMD_START+ FT800_RAM_CMD_SIZE)))

/* ------------------------------------------------------------------ */
/*  register file (excerpt – only the ones used by user-space demos)  */
/* ------------------------------------------------------------------ */
#define FT800_REG_ID                 0x102400UL
#define FT800_REG_HCYCLE             0x102428UL
#define FT800_REG_HOFFSET            0x10242CUL
#define FT800_REG_HSIZE              0x102430UL
#define FT800_REG_HSYNC0             0x102434UL
#define FT800_REG_HSYNC1             0x102438UL
#define FT800_REG_VCYCLE             0x10243CUL
#define FT800_REG_VOFFSET            0x102440UL
#define FT800_REG_VSIZE              0x102444UL
#define FT800_REG_VSYNC0             0x102448UL
#define FT800_REG_VSYNC1             0x10244CUL
#define FT800_REG_DLSWAP             0x102450UL
#define FT800_REG_SWIZZLE            0x102460UL
#define FT800_REG_CSPREAD            0x102464UL
#define FT800_REG_PCLK_POL           0x102468UL
#define FT800_REG_PCLK               0x10246CUL
#define FT800_REG_GPIO_DIR           0x10248CUL
#define FT800_REG_GPIO               0x102490UL
#define FT800_REG_PWM_HZ             0x1024C0UL
#define FT800_REG_PWM_DUTY           0x1024C4UL
#define FT800_REG_CMD_READ           0x1024E4UL
#define FT800_REG_CMD_WRITE          0x1024E8UL
#define FT800_REG_CMD_DL             0x1024ECUL

#define DLSWAP_FRAME 0x02u   /* value written to REG_DLSWAP */

/* ------------------------------------------------------------------ */
/*  display-list opcodes & helper macros                              */
/* ------------------------------------------------------------------ */
#define FT800_DL_OP_DISPLAY            0x00U
#define FT800_DL_OP_BITMAP_SOURCE      0x01U
#define FT800_DL_OP_CLEAR_COLOR_RGB    0x02U
#define FT800_DL_OP_TAG                0x03U
#define FT800_DL_OP_COLOR_RGB          0x04U
#define FT800_DL_OP_BITMAP_HANDLE      0x05U
#define FT800_DL_OP_CELL               0x06U
#define FT800_DL_OP_BITMAP_LAYOUT      0x07U
#define FT800_DL_OP_BITMAP_SIZE        0x08U
#define FT800_DL_OP_ALPHA_FUNC         0x09U
#define FT800_DL_OP_STENCIL_FUNC       0x0AU
#define FT800_DL_OP_BLEND_FUNC         0x0BU
#define FT800_DL_OP_STENCIL_OP         0x0CU
#define FT800_DL_OP_POINT_SIZE         0x0DU
#define FT800_DL_OP_LINE_WIDTH         0x0EU
#define FT800_DL_OP_CLEAR_COLOR_A      0x0FU
#define FT800_DL_OP_COLOR_A            0x10U
#define FT800_DL_OP_CLEAR_STENCIL      0x11U
#define FT800_DL_OP_CLEAR_TAG          0x12U
#define FT800_DL_OP_STENCIL_MASK       0x13U
#define FT800_DL_OP_TAG_MASK           0x14U
#define FT800_DL_OP_BITMAP_TRANSFORM_A 0x15U
#define FT800_DL_OP_BITMAP_TRANSFORM_B 0x16U
#define FT800_DL_OP_BITMAP_TRANSFORM_C 0x17U
#define FT800_DL_OP_BITMAP_TRANSFORM_D 0x18U
#define FT800_DL_OP_BITMAP_TRANSFORM_E 0x19U
#define FT800_DL_OP_BITMAP_TRANSFORM_F 0x1AU
#define FT800_DL_OP_SCISSOR_XY         0x1BU
#define FT800_DL_OP_SCISSOR_SIZE       0x1CU
#define FT800_DL_OP_CALL               0x1DU
#define FT800_DL_OP_JUMP               0x1EU
#define FT800_DL_OP_BEGIN              0x1FU
#define FT800_DL_OP_COLOR_MASK         0x20U
#define FT800_DL_OP_END                0x21U
#define FT800_DL_OP_SAVE_CONTEXT       0x22U
#define FT800_DL_OP_RESTORE_CONTEXT    0x23U
#define FT800_DL_OP_RETURN             0x24U
#define FT800_DL_OP_MACRO              0x25U
#define FT800_DL_OP_CLEAR              0x26U
#define FT800_DL_OP_VERTEX2F           0x40U
#define FT800_DL_OP_VERTEX2II          0x80U

/* helpers for hand-crafted DL entries ------------------------------------ */
#define CLEAR_COLOR_RGB(r,g,b) \
        (((FT800_DL_OP_CLEAR_COLOR_RGB) << 24) | (((r)&0xFFU)<<16) | (((g)&0xFFU)<<8) | ((b)&0xFFU))
#define CLEAR_COLOR_A(a) \
        (((FT800_DL_OP_CLEAR_COLOR_A) << 24) | ((a) & 0xFFU))
#define CLEAR(c,s,t) \
        (((FT800_DL_OP_CLEAR) << 24) | (((c)&1U)<<2) | (((s)&1U)<<1) | ((t)&1U))
#define DISPLAY() 0x00000000UL
#define BEGIN(prim) (((FT800_DL_OP_BEGIN) << 24) | ((prim) & 0xFFU))
#define END() ((FT800_DL_OP_END) << 24)
#define COLOR_RGB(r,g,b) \
        (((FT800_DL_OP_COLOR_RGB) << 24) | (((r)&0xFFU)<<16) | (((g)&0xFFU)<<8) | ((b)&0xFFU))
#define COLOR_A(a) \
        (((FT800_DL_OP_COLOR_A) << 24) | ((a) & 0xFFU))
#define TAG(s) (((FT800_DL_OP_TAG) << 24) | ((s) & 0xFFU))
#define TAG_MASK(m) (((FT800_DL_OP_TAG_MASK) << 24) | ((m) & 0x01U))
#define VERTEX2F(x,y) \
        (((FT800_DL_OP_VERTEX2F) << 24) | ((((x)&0x3FFFU)<<14) | (((y)&0x3FFFU)<<2)))
#define VERTEX2II(x,y,h,c) \
        (((FT800_DL_OP_VERTEX2II) << 24) | ((((h)&0x1FU)<<19) | (((c)&0x7FU)<<12) | (((x)&0x1FFU)<<9) | ((y)&0x1FFU)))
#define BITMAP_SOURCE(a) \
        (((FT800_DL_OP_BITMAP_SOURCE) << 24) | ((a) & 0x1FFFFFUL))
#define BITMAP_LAYOUT(f,s,h) \
        (((FT800_DL_OP_BITMAP_LAYOUT) << 24) | (((f)&0x1FU)<<19) | (((s)&0x1FFFUL)<<6) | ((h)&0x3FU))
#define BITMAP_SIZE(fl,wx,wy,w,h) \
        (((FT800_DL_OP_BITMAP_SIZE) << 24) | (((fl)&3U)<<20) | (((wx)&1U)<<19) | (((wy)&1U)<<18) | (((w)&0x1FFU)<<9) | ((h)&0x1FFU))

/* ------------------------------------------------------------------ */
/*  co-processor (CMD_*) tokens                                       */
/* ------------------------------------------------------------------ */
#define CMD_DLSTART   0xFFFFFF00UL
#define CMD_SWAP      0xFFFFFF01UL
#define CMD_TEXT      0x0C000000UL
#define CMD_BUTTON    0x0D000000UL
#define CMD_NUMBER    0x0E000000UL
#define CMD_SLIDER    0x10000000UL
#define CMD_TOGGLE    0x11000000UL
#define CMD_CLOCK     0x12000000UL
#define CMD_DIAL      0x13000000UL
#define CMD_KEYS      0x1A000000UL
#define CMD_GRADCOLOR 0xFFFFFF34UL
#define CMD_GRADIENT  0xFFFFFF2DUL
#define CMD_MEMWRITE  0xFFFFFF1AUL
#define CMD_MEMSET    0xFFFFFF1BUL
#define CMD_MEMCPY    0xFFFFFF1DUL
#define CMD_INTERRUPT 0xFFFFFF02UL
#define CMD_LOGO      0xFFFFFF31UL
#define CMD_COLDSTART 0xFFFFFF32UL

/* legacy aliases kept for older examples --------------------------- */
#define FT800_CMD_TEXT      CMD_TEXT
#define FT800_CMD_BUTTON    CMD_BUTTON
#define FT800_CMD_NUMBER    CMD_NUMBER
#define FT800_CMD_SLIDER    CMD_SLIDER
#define FT800_CMD_TOGGLE    CMD_TOGGLE
#define FT800_CMD_CLOCK     CMD_CLOCK
#define FT800_CMD_DIAL      CMD_DIAL
#define FT800_CMD_KEYS      CMD_KEYS
#define FT800_CMD_GRADCOLOR CMD_GRADCOLOR
#define FT800_CMD_GRADIENT  CMD_GRADIENT
#define FT800_CMD_MEMWRITE  CMD_MEMWRITE
#define FT800_CMD_MEMSET    CMD_MEMSET
#define FT800_CMD_MEMCPY    CMD_MEMCPY

/* ------------------------------------------------------------------ */
/*  host-interface single-byte commands                               */
/* ------------------------------------------------------------------ */
#define FT800_HOST_ACTIVE  0x00U  /* wake-up / ACTIVE */
#define FT800_HOST_CLKEXT  0x44U  /* external crystal */
#define FT800_HOST_CLK48M  0x62U  /* PLL = 48 MHz    */
#define FT800_HOST_CORERST 0x68U  /* soft core reset */

/* ------------------------------------------------------------------ */
/*  primitive types for BEGIN()                                       */
/* ------------------------------------------------------------------ */
enum FT8_Primitive {
    FT8_POINTS = 0x01,
    FT8_LINES  = 0x02,
    FT8_LINE_STRIP = 0x03,
    FT8_RECTS  = 0x09,
    /* … add more if you use them … */
};

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _FT800_REGS_HPP_ */
