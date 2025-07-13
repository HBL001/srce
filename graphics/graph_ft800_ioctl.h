
/**
 * @file graph_ft800_ioctl.h
 * @brief User-space IOCTL definitions for FT800 kernel interface
 */

#ifndef GRAPH_FT800_IOCTL_H
#define GRAPH_FT800_IOCTL_H

#include <stdint.h>
#include <stddef.h>
#include <linux/ioctl.h>

#define FT800_IOC_MAGIC        'f'

// Display commands
#define FT800_IOC_INITIALISE         _IO(FT800_IOC_MAGIC, 0x01)
#define FT800_IOC_CMD_DLSTART        _IO(FT800_IOC_MAGIC, 0x02)
#define FT800_IOC_CMD_SWAP           _IO(FT800_IOC_MAGIC, 0x03)
#define FT800_IOC_CMD_BUTTON         _IOW(FT800_IOC_MAGIC, 0x04, struct ft800_cmd_button)
#define FT800_IOC_CMD_TEXT           _IOW(FT800_IOC_MAGIC, 0x05, struct ft800_cmd_text)
#define FT800_IOC_CMD_SPINNER        _IOW(FT800_IOC_MAGIC, 0x06, struct ft800_cmd_spinner)
#define FT800_IOC_CMD_CALIBRATE      _IO(FT800_IOC_MAGIC, 0x07)
#define FT800_IOC_BEGIN_BITMAP       _IOW(FT800_IOC_MAGIC, 0x08, uint8_t)
#define FT800_IOC_BITMAP_LAYOUT      _IOW(FT800_IOC_MAGIC, 0x09, struct ft800_bitmap_layout)
#define FT800_IOC_BITMAP_SIZE        _IOW(FT800_IOC_MAGIC, 0x0A, struct ft800_bitmap_size)
#define FT800_IOC_CLEAR              _IOW(FT800_IOC_MAGIC, 0x0B, struct ft800_clear_args)
#define FT800_IOC_CLEAR_COLOR_RGB    _IOW(FT800_IOC_MAGIC, 0x0C, struct ft800_rgb)
#define FT800_IOC_DISPLAY            _IO(FT800_IOC_MAGIC, 0x0D)
#define FT800_IOC_END                _IO(FT800_IOC_MAGIC, 0x0E)

// Touch input and control
#define FT800_IOC_GET_TOUCH_RAW      _IOR(FT800_IOC_MAGIC, 0x0F, struct ft800_touch_xy)
#define FT800_IOC_GET_TOUCH_SCREEN   _IOR(FT800_IOC_MAGIC, 0x10, struct ft800_touch_xy)
#define FT800_IOC_GET_TOUCH_TAG      _IOR(FT800_IOC_MAGIC, 0x11, uint8_t)
#define FT800_IOC_SET_TAG            _IOW(FT800_IOC_MAGIC, 0x12, uint8_t)

// FIFO operations
#define FT800_IOC_FIFO_EMPTY         _IOR(FT800_IOC_MAGIC, 0x13, int)
#define FT800_IOC_UPDATE_FIFO_PTR    _IOW(FT800_IOC_MAGIC, 0x14, uint32_t)

// Asset & Calibration
#define FT800_IOC_LOAD_BITMAP        _IOW(FT800_IOC_MAGIC, 0x15, struct ft800_load_bitmap)
#define FT800_IOC_SET_CALIBRATION    _IOW(FT800_IOC_MAGIC, 0x16, struct ft800_cal_data)
#define FT800_IOC_GET_CAL_STATUS     _IOR(FT800_IOC_MAGIC, 0x17, int)

struct ft800_cmd_button {
    uint16_t x, y, w, h, font, options;
    char text[64];
};

struct ft800_cmd_text {
    uint16_t x, y, font, options;
    char text[64];
};

struct ft800_cmd_spinner {
    uint16_t x, y, style, scale;
};

struct ft800_bitmap_layout {
    uint16_t format, linestride, height;
};

struct ft800_bitmap_size {
    uint8_t filter, wrapx, wrapy;
    uint16_t width, height;
};

struct ft800_clear_args {
    bool c, s, t;
};

struct ft800_rgb {
    uint8_t r, g, b;
};

struct ft800_touch_xy {
    uint16_t x, y;
};

struct ft800_load_bitmap {
    uint32_t dst_addr;
    void* src;
    size_t size;
};

struct ft800_cal_data {
    int32_t transform[6];
};

#endif // GRAPH_FT800_IOCTL_H
