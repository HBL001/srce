
/**
 * @file graph_ft800.cpp
 * @brief FT800 interface using kernel driver IOCTLs.
 *
 * Provides all FT800 operations through a safe user-space wrapper.
 */

 #include <fcntl.h>
 #include <unistd.h>
 #include <sys/ioctl.h>
 #include <cstdio>
 #include <cstdint>
 #include <cstring>
 #include "graph_ft800.h"
 #include "graph_ft800_ioctl.h"  // IOCTL command definitions and structures
 
 GraphFt800::GraphFt800(const char* device_path)
     : fd(-1), display_initialised(false), write_index(0)
 {
     if (device_path != nullptr)
     {
         fd = open(device_path, O_RDWR);
     }
 }
 
 GraphFt800::~GraphFt800()
 {
     if (fd >= 0)
     {
         (void)close(fd);
     }
 }
 
 /** Initializes the FT800 display. */
 bool GraphFt800::initialise()
 {
     int status = ioctl(fd, FT800_IOC_INITIALISE);
     display_initialised = (status == 0);
     return display_initialised;
 }
 
 /** Starts a new display list. */
 void GraphFt800::cmd_dlstart()
 {
     (void)ioctl(fd, FT800_IOC_CMD_DLSTART);
 }
 
 /** Swaps display list. */
 void GraphFt800::cmd_swap()
 {
     (void)ioctl(fd, FT800_IOC_CMD_SWAP);
 }
 
 /** Draws a button. */
 void GraphFt800::cmd_button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* text)
 {
     struct ft800_cmd_button args = {x, y, w, h, font, options};
     (void)strncpy(args.text, text, sizeof(args.text) - 1);
     args.text[sizeof(args.text) - 1] = '\0';
     (void)ioctl(fd, FT800_IOC_CMD_BUTTON, &args);
 }
 
 /** Draws text. */
 void GraphFt800::cmd_text(uint16_t x, uint16_t y, uint16_t font, uint16_t options, const char* text)
 {
     struct ft800_cmd_text args = {x, y, font, options};
     (void)strncpy(args.text, text, sizeof(args.text) - 1);
     args.text[sizeof(args.text) - 1] = '\0';
     (void)ioctl(fd, FT800_IOC_CMD_TEXT, &args);
 }
 
 /** Draws a spinner. */
 void GraphFt800::cmd_spinner(uint16_t x, uint16_t y, uint16_t style, uint16_t scale)
 {
     struct ft800_cmd_spinner args = {x, y, style, scale};
     (void)ioctl(fd, FT800_IOC_CMD_SPINNER, &args);
 }
 
 /** Starts calibration. */
 void GraphFt800::cmd_calibrate()
 {
     (void)ioctl(fd, FT800_IOC_CMD_CALIBRATE);
 }
 
 /** Begins a bitmap drawing context. */
 void GraphFt800::begin_bitmap(uint8_t handle)
 {
     (void)ioctl(fd, FT800_IOC_BEGIN_BITMAP, &handle);
 }
 
 /** Configures bitmap layout. */
 void GraphFt800::bitmap_layout(uint16_t format, uint16_t linestride, uint16_t height)
 {
     struct ft800_bitmap_layout args = {format, linestride, height};
     (void)ioctl(fd, FT800_IOC_BITMAP_LAYOUT, &args);
 }
 
 /** Configures bitmap size. */
 void GraphFt800::bitmap_size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height)
 {
     struct ft800_bitmap_size args = {filter, wrapx, wrapy, width, height};
     (void)ioctl(fd, FT800_IOC_BITMAP_SIZE, &args);
 }
 
 /** Clears screen with parameters. */
 void GraphFt800::clear(bool c, bool s, bool t)
 {
     struct ft800_clear_args args = {c, s, t};
     (void)ioctl(fd, FT800_IOC_CLEAR, &args);
 }
 
 /** Sets clear color using RGB. */
 void GraphFt800::clear_color_rgb(uint8_t r, uint8_t g, uint8_t b)
 {
     struct ft800_rgb args = {r, g, b};
     (void)ioctl(fd, FT800_IOC_CLEAR_COLOR_RGB, &args);
 }
 
 /** Signals end of display list. */
 void GraphFt800::display()
 {
     (void)ioctl(fd, FT800_IOC_DISPLAY);
 }
 
 /** Closes drawing group. */
 void GraphFt800::end()
 {
     (void)ioctl(fd, FT800_IOC_END);
 }
 
 /** Reads raw touch coordinates. */
 bool GraphFt800::get_touch_raw_xy(uint16_t* x, uint16_t* y)
 {
     struct ft800_touch_xy coords;
     int status = ioctl(fd, FT800_IOC_GET_TOUCH_RAW, &coords);
     if (status == 0 && x != nullptr && y != nullptr)
     {
         *x = coords.x;
         *y = coords.y;
         return true;
     }
     return false;
 }
 
 /** Reads screen-calibrated touch coordinates. */
 bool GraphFt800::get_touch_screen_xy(uint16_t* x, uint16_t* y)
 {
     struct ft800_touch_xy coords;
     int status = ioctl(fd, FT800_IOC_GET_TOUCH_SCREEN, &coords);
     if (status == 0 && x != nullptr && y != nullptr)
     {
         *x = coords.x;
         *y = coords.y;
         return true;
     }
     return false;
 }
 
 /** Gets current touch tag. */
 uint8_t GraphFt800::get_touch_tag()
 {
     uint8_t tag = 0U;
     (void)ioctl(fd, FT800_IOC_GET_TOUCH_TAG, &tag);
     return tag;
 }
 
 /** Sets a tag for current context. */
 void GraphFt800::tag(uint8_t tag)
 {
     (void)ioctl(fd, FT800_IOC_SET_TAG, &tag);
 }
 
 /** Checks if display FIFO is empty. */
 bool GraphFt800::fifo_empty()
 {
     int status = 0;
     (void)ioctl(fd, FT800_IOC_FIFO_EMPTY, &status);
     return (status != 0);
 }
 
 /** Updates FIFO write pointer. */
 void GraphFt800::update_fifo_write_pointer(uint32_t ptr)
 {
     (void)ioctl(fd, FT800_IOC_UPDATE_FIFO_PTR, &ptr);
 }
 
 /** Uploads a bitmap to FT800 RAM. */
 void GraphFt800::load_bitmap(uint32_t dst_addr, const void* src, size_t size)
 {
     struct ft800_load_bitmap args = {dst_addr, const_cast<void*>(src), size};
     (void)ioctl(fd, FT800_IOC_LOAD_BITMAP, &args);
 }
 
 /** Manually sets calibration values. */
 void GraphFt800::set_calibration(const struct ft800_cal_data& cal)
 {
     (void)ioctl(fd, FT800_IOC_SET_CALIBRATION, &cal);
 }
 
 /** Returns calibration status. */
 bool GraphFt800::calibration_complete()
 {
     int status = 0;
     (void)ioctl(fd, FT800_IOC_GET_CAL_STATUS, &status);
     return (status != 0);
 }
 