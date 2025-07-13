/* SPDX-License-Identifier: GPL-2.0+ */
#ifndef _UAPI_LINUX_FT800_H
#define _UAPI_LINUX_FT800_H

/*
 * ft800_uapi.hpp  –  Bridgetek FT800 (EVE1) public API
 *
 *  • Exactly the same data-layout and ioctl numbers as the in-kernel copy
 *    shipped with the driver (ft800_uapi.h).
 *  • Thin tweaks so the file is painless to consume from modern C++:
 *        – <cstdint>/<cstddef> instead of <linux/types.h>
 *        – everything wrapped in extern "C" when compiled as C++
 *
 *  Nothing else changed – keep it in sync with the driver!
 */

#ifdef __cplusplus
  #include <cstdint>
  #include <cstddef>
  #include <sys/ioctl.h>
  extern "C" {
    using  __u8  = std::uint8_t;
    using  __u16 = std::uint16_t;
    using  __u32 = std::uint32_t;
    using  __u64 = std::uint64_t;
    using __le16 = std::uint16_t;   /* little-endian helper */
#else               /* pure-C or kernel side → original includes */
  #include <linux/types.h>
  #include <linux/ioctl.h>
#endif

/* -------------------------------------------------------------------- */
/*  attributes                                                           */
/* -------------------------------------------------------------------- */
#ifndef __packed
# define __packed __attribute__((packed))
#endif

/* -------------------------------------------------------------------- */
/*  shared constants                                                     */
/* -------------------------------------------------------------------- */
constexpr __u32 FT800_MAX_CMD_TEXT_LEN = 63U;   /* +1 for NUL */

/* -------------------------------------------------------------------- */
/*  structures (MUST stay in this order/size)                            */
/* -------------------------------------------------------------------- */

struct ft800_touch {
    __le16 x;
    __le16 y;
    __u8   tag;
    __u8   pad;
} __packed;

struct ft800_cmd {
    __u32 cmd;
    __u16 x, y;
    __u16 w, h;
    __u8  font;
    __u8  options;
    __u16 val;
    __u16 range;
    __u32 arg;
    __u32 arg2;
    __u32 count;
    __u32 color0;
    __u32 color1;
    char  text[FT800_MAX_CMD_TEXT_LEN + 1];
} __packed;

/* rarely used by the driver itself, kept for completeness */
struct ft800_cmd_keys {
    __u32 cmd;
    __u16 x, y;
    __u16 w, h;
    __u8  font;
    __u8  options;
    const char *text;
};

/* bulk copy into RAM_CMD */
struct ft800_uapi_cmdlist {
    __u32 addr;
    __u32 len;
    __u64 user_ptr;
} __packed;

/* push mmap’ed staging buffer */
struct ft800_uapi_mmap_push {
    __u32 addr;
    __u32 len;
} __packed;

/* soft reset helper */
struct ft800_uapi_reset {
    __u8 clear_display;
    __u8 r, g, b;
} __packed;

/* generic memory I/O */
struct ft800_mem_op {
    __u32 addr;
    __u32 len;
    __u8  data[4096];
} __packed;

/* short status snapshot */
struct ft800_status {
    __le16 cmd_read;
    __le16 cmd_write;
    __u8   id;
    __u8   pad;
} __packed;

/* batch execute descriptor list */
struct ft800_uapi_exec_cmds {
    __u64 user_ptr;
    __u32 count;
} __packed;

/* fine-grained register helpers */
struct ft800_uapi_rw8  { __u32 addr; __u8  value; };
struct ft800_uapi_rw16 { __u32 addr; __u16 value; };

/* -------------------------------------------------------------------- */
/*  ioctl numbers (unchanged)                                            */
/* -------------------------------------------------------------------- */
#define FT800_IOCTL_BASE   'F'

#define FT800_IOCTL_GET_TOUCH    _IOR (FT800_IOCTL_BASE, 0x01, struct ft800_touch)
#define FT800_IOCTL_SUBMIT_CMDS  _IOW (FT800_IOCTL_BASE, 0x02, struct ft800_uapi_cmdlist)
#define FT800_IOCTL_PUSH_MMAP    _IOW (FT800_IOCTL_BASE, 0x03, struct ft800_uapi_mmap_push)
#define FT800_IOCTL_RESET        _IOW (FT800_IOCTL_BASE, 0x04, struct ft800_uapi_reset)
#define FT800_IOCTL_CMD_WRITE    _IOW (FT800_IOCTL_BASE, 0x05, struct ft800_cmd [32])
#define FT800_IOCTL_MEMWRITE     _IOW (FT800_IOCTL_BASE, 0x06, struct ft800_mem_op)
#define FT800_IOCTL_MEMREAD      _IOWR(FT800_IOCTL_BASE, 0x07, struct ft800_mem_op)
#define FT800_IOCTL_GET_STATUS   _IOR (FT800_IOCTL_BASE, 0x08, struct ft800_status)
#define FT800_IOCTL_CLEAR_DL     _IO  (FT800_IOCTL_BASE, 0x09)
#define FT800_IOCTL_EXEC_CMDS    _IOW (FT800_IOCTL_BASE, 0x0A, struct ft800_uapi_exec_cmds)
#define FT800_IOCTL_WR8          _IOW (FT800_IOCTL_BASE, 0x0B, struct ft800_uapi_rw8)
#define FT800_IOCTL_RD8          _IOWR(FT800_IOCTL_BASE, 0x0C, struct ft800_uapi_rw8)
#define FT800_IOCTL_WR16         _IOW (FT800_IOCTL_BASE, 0x0D, struct ft800_uapi_rw16)
#define FT800_IOCTL_RD16         _IOWR(FT800_IOCTL_BASE, 0x0E, struct ft800_uapi_rw16)

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _UAPI_LINUX_FT800_H */
