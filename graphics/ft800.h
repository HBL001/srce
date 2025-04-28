/*!
 * \file ft800.h
 * \brief FT800 Main header file
 */

 #ifndef FT800_H
 #define FT800_H
 
 #include <cstdint>

 #include "ft800If.h"
 

 class Ft800
 {
 public:
 
     typedef struct
     {
         uint32_t transform_a;
         uint32_t transform_b;
         uint32_t transform_c;
         uint32_t transform_d;
         uint32_t transform_e;
         uint32_t transform_f;
     } calibration_stc_t;
 
     typedef enum
     {
         OPT_3D = 0,
         OPT_MONO = 1,
         OPT_NODL = 2,
         OPT_SIGNED = 256,
         OPT_FLAT = 256,
         OPT_CENTERX = 512,
         OPT_CENTERY = 1024,
         OPT_CENTER = 1536,
         OPT_RIGHTX = 2048,
         OPT_NOBACK = 4096,
         OPT_NOTICKS = 8192,
         OPT_NOHM = 16384,
         OPT_NOPOINTER = 16384,
         OPT_NOSECS = 32768,
         OPT_NOHANDS = 49152
     } option_t;
 
     typedef enum
     {
         ARGB1555 = 0,
         L1,
         L4,
         L8,
         RGB332,
         ARGB2,
         ARGB4,
         RGB565,
         PALETTED,
         TEXT8X8,
         TEXTVGA,
         BARGRAPH
     } bitmap_format_t;
 
     typedef enum
     {
         NEAREST= 0,
         BILINEAR
     } filter_t;
 
     typedef enum
     {
         BORDER = 0,
         REPEAT
     } wrap_t;
 
     typedef enum
     {
         CIRCLE = 0,
         LINE,
         CLOCK,
         ORBIT
     } spinner_style_t;
 
     Ft800 (uint8_t bus_number, uint8_t channel_number);
     ~Ft800 (void);
 
     bool initialise (void);
 
     void cmd_dlstart (void);
     void cmd_swap (void);
 
     void cmd_calibrate (void);
     bool calibration_complete (calibration_stc_t &calibration);
 
     void set_calibration (calibration_stc_t &calibration);
 
     void clear_color_rgb (uint8_t red, uint8_t green, uint8_t blue);
     void clear (uint8_t color, uint8_t stencil, uint8_t tag);
     void display (void);
 
     void color_rgb (uint8_t red, uint8_t green, uint8_t blue);
 
     void cmd_text (uint16_t x, uint16_t y, uint16_t font, option_t option, const char* string);
     void cmd_button (uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, option_t option, const char* string);
     void cmd_spinner (uint16_t x, uint16_t y, spinner_style_t style, uint16_t scale);
 
     void load_bitmap (uint32_t ram_g_offset, const uint8_t* bitmap, size_t length);
     void bitmap_source (uint32_t ram_g_offset);
     void bitmap_layout (bitmap_format_t format, uint16_t stride, uint16_t height);
     void bitmap_size (filter_t filter, wrap_t wrap_x, wrap_t wrap_y, uint16_t width, uint16_t height);
     void begin_bitmap (void);
     void end (void);
     void vertex2ii (uint16_t x, uint16_t y, uint8_t handle, uint8_t cell);
     void tag (uint8_t value);
 
     bool get_touch_raw_xy (uint16_t* x, uint16_t* y);
     bool get_touch_screen_xy (uint16_t* x, uint16_t* y);
     bool get_touch_tag (uint8_t* tag);
 
 private:
     static const uint32_t dlstart_command = 0xffffff00;
     static const uint32_t dlswap_command = 0xffffff01;
     static const uint32_t text_command = 0xffffff0c;
     static const uint32_t button_command = 0xffffff0d;
     static const uint32_t calibrate_command = 0xffffff15;
     static const uint32_t spinner_command = 0xffffff16;
 
     static const uint32_t command_fifo_size = 4096;
 
     Ft800_interface ft800_interface;
 
     bool write_command_32_bit (uint32_t value);
     bool write_command_string (const char *string);
     bool write_command_buffer (const uint8_t* buffer, size_t length);
 
     void update_fifo_write_pointer (size_t size);
 
     bool fifo_empty (void);
 
     bool display_initialised;
     uint16_t write_index;
 };
 
 #endif
 
 