/*!
 * \file ft800.h
 * \brief FT800 Display Control Header File
 */

 #ifndef FT800_H
 #define FT800_H
 
 #include "ft800If.h"
 #include <cstddef>
 
 /*!
  * \class Ft800
  * \brief High-level driver for controlling the FT800 display controller, graphics, and touch interface.
  */
 class Ft800
 {
 public:
     //! Calibration data for touchscreen
     typedef struct
     {
         uint32_t transform_a;
         uint32_t transform_b;
         uint32_t transform_c;
         uint32_t transform_d;
         uint32_t transform_e;
         uint32_t transform_f;
     } calibration_stc_t;
 
     //! Options for text rendering, buttons, and widgets
     typedef enum
     {
         OPT_3D        = 0,
         OPT_MONO      = 1,
         OPT_NODL      = 2,
         OPT_SIGNED    = 256,
         OPT_FLAT      = 256,
         OPT_CENTERX   = 512,
         OPT_CENTERY   = 1024,
         OPT_CENTER    = 1536,
         OPT_RIGHTX    = 2048,
         OPT_NOBACK    = 4096,
         OPT_NOTICKS   = 8192,
         OPT_NOHM      = 16384,
         OPT_NOPOINTER = 16384,
         OPT_NOSECS    = 32768,
         OPT_NOHANDS   = 49152,
         OPT_RIGHTX_CENTERY = (2048 | 1024),
         OPT_LEFTX_TOPY    = OPT_3D
     } option_t;
 
     //! Bitmap formats used in load_bitmap
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
 
     //! Filtering modes for bitmaps
     typedef enum
     {
         NEAREST = 0,
         BILINEAR
     } filter_t;
 
     //! Wrapping modes for bitmaps
     typedef enum
     {
         BORDER = 0,
         REPEAT
     } wrap_t;
 
     //! Spinner styles
     typedef enum
     {
         CIRCLE = 0,
         LINE,
         CLOCK,
         ORBIT
     } spinner_style_t;
 
     static const uint8_t maximum_user_handle = 13;
     static const uint8_t vertical_text_internal_handle = 14;
 
     Ft800(uint8_t bus_number, uint8_t channel_number);
     ~Ft800(void);
 
     int initialise(bool force = true, uint8_t red = 0x00, uint8_t green = 0x00, uint8_t blue = 0x00);
     void cmd_dlstart(void);
     void cmd_swap(void);
     void cmd_calibrate(void);
     int calibration_complete(calibration_stc_t &calibration);
     void set_calibration(calibration_stc_t &calibration);
 
     void clear_color_rgb(uint8_t red, uint8_t green, uint8_t blue);
     void clear(uint8_t color, uint8_t stencil, uint8_t tag);
     void display(void);
 
     void color_rgb(uint8_t red, uint8_t green, uint8_t blue);
     void color_alpha(uint8_t alpha);
 
     void cmd_bgcolor(uint32_t rgb);
     void cmd_fgcolor(uint32_t rgb);
 
     void cmd_text(uint16_t x, uint16_t y, uint16_t font, option_t option, const char* string);
     void cmd_button(uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, option_t option, const char* string);
     void cmd_keys(uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, option_t option, const char* string);
     void cmd_keys(uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, char pressed, const char* string);
     void cmd_spinner(uint16_t x, uint16_t y, spinner_style_t style, uint16_t scale);
 
     void load_bitmap(uint32_t ram_g_offset, const uint8_t* bitmap, size_t length);
     void cmd_inflate(uint32_t ram_g_offset, const uint8_t* bitmap, size_t length);
     uint32_t bitmap_handle(uint8_t handle);
     void bitmap_source(uint32_t ram_g_offset);
     uint32_t bitmap_layout(bitmap_format_t format, uint16_t stride, uint16_t height);
     uint32_t bitmap_size(filter_t filter, wrap_t wrap_x, wrap_t wrap_y, uint16_t width, uint16_t height);
 
     uint32_t begin_bitmap(void);
     void begin_point(void);
     void begin_line(void);
     void begin_line_strip(void);
     void begin_rectangle(void);
     void end(void);
     void vertex2f(int32_t x, int32_t y);
     void vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell);
     void tag(uint8_t value);
 
     void point_size(uint16_t size);
     void line_width(uint16_t width);
 
     void cmd_macro(uint8_t macro_id);
     void set_macro_color_rgb(uint8_t macro_id, uint8_t red, uint8_t green, uint8_t blue);
 
     int get_touch_raw_xy(uint16_t* x, uint16_t* y);
     int get_touch_screen_xy(uint16_t* x, uint16_t* y);
     int get_touch_tag(uint8_t* tag);
 
     void vertical_text(uint16_t x, uint16_t y, uint8_t font, const char* string);
 
     void configure_keyclick(uint16_t sound_code);
     void key_click(void);
     void mute_sound(void);
     void unmute_sound(void);
 
     void set_backlight(uint8_t percent);
 
     private:
     static const uint32_t dlstart_command = 0xffffff00;
     static const uint32_t dlswap_command = 0xffffff01;
     static const uint32_t background_colour_command = 0xffffff09;
     static const uint32_t foreground_colour_command = 0xffffff0A;
     static const uint32_t cmd_calibrate_command = 0xffffff15;
     static const uint32_t cmd_inflate_command = 0xffffff22;
     static const uint32_t cmd_text_command = 0xffffff0C;
     static const uint32_t cmd_button_command = 0xffffff0D;
     static const uint32_t cmd_keys_command = 0xffffff0E;
     static const uint32_t cmd_spinner_command = 0xffffff16;
     static const uint32_t cmd_fgcolor_command = 0xffffff0A;
     static const uint32_t cmd_bgcolor_command = 0xffffff09;

     static uint32_t cmd_offset;  ///< Tracks current command buffer offset
     static const uint32_t ram_cmd = 0x308000UL;  ///< FT800 command buffer base address

     Ft800_interface interface; ///< Interface to communicate with FT800

     uint32_t get_cmd_offset(void) const { return cmd_offset; }
     void increment_cmd_offset(uint32_t amount) { cmd_offset = (cmd_offset + amount) & 0xFFF; }
 
 }; // End of class Ft800
 
 #endif // FT800_H
