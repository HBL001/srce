/*!
 * \file ft800.cpp
 * \brief FT800 display control implementation
 */

 #include <cstdint>
 #include <cstdio>
 #include <unistd.h>
 #include <cstring>

 #include "ft800If.h"
 #include "ft800Reg.h"
 #include "ft800.h"

 /*!
 * \brief Construct a new Ft800 object
 * \param bus_number SPI bus number
 * \param channel_number SPI channel (chip select) number
 */
Ft800::Ft800(uint8_t bus_number, uint8_t channel_number)
    : ft800_interface(bus_number, channel_number)
{
}

/*!
 * \brief Destructor
 */
Ft800::~Ft800(void)
{
}

/*!
 * \brief Initialise the FT800 device
 * \param force Force re-initialisation even if device is already awake
 * \param red Default background color (red channel)
 * \param green Default background color (green channel)
 * \param blue Default background color (blue channel)
 * \return 0 if successful, -1 if error
 */
int Ft800::initialise(bool force, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t id = 0;

    if (force)
    {
        ft800_interface.host_command(Ft800_interface::ACTIVE);
        usleep(20000); // Wait 20ms
    }

    if (ft800_interface.read_8_bit(REG_ID, &id) != 0)
    {
        return -1;
    }

    if (id != 0x7C) // Expected ID for FT800
    {
        return -1;
    }

    ft800_interface.write_8_bit(REG_PCLK, 0); // Disable display during config

    ft800_interface.write_16_bit(REG_HCYCLE, 548);
    ft800_interface.write_16_bit(REG_HOFFSET, 43);
    ft800_interface.write_16_bit(REG_HSIZE, 480);
    ft800_interface.write_16_bit(REG_HSYNC0, 0);
    ft800_interface.write_16_bit(REG_HSYNC1, 41);

    ft800_interface.write_16_bit(REG_VCYCLE, 292);
    ft800_interface.write_16_bit(REG_VOFFSET, 12);
    ft800_interface.write_16_bit(REG_VSIZE, 272);
    ft800_interface.write_16_bit(REG_VSYNC0, 0);
    ft800_interface.write_16_bit(REG_VSYNC1, 10);

    ft800_interface.write_8_bit(REG_SWIZZLE, 0);
    ft800_interface.write_8_bit(REG_PCLK_POL, 1);
    ft800_interface.write_8_bit(REG_CSPREAD, 1);

    clear_color_rgb(red, green, blue);
    clear(1, 1, 1);
    display();

    ft800_interface.write_8_bit(REG_PCLK, 5); // Enable display, 60 Hz

    return 0;
}

/*!
 * \brief Start a new display list
 */
void Ft800::cmd_dlstart(void)
{
    ft800_interface.write_32_bit(RAM_DL + 0, 0xFFFFFF00); // CMD_DLSTART
}

/*!
 * \brief Swap the display lists
 */
void Ft800::cmd_swap(void)
{
    ft800_interface.write_32_bit(RAM_DL + 4, 0xFFFFFF01); // CMD_SWAP
    ft800_interface.write_8_bit(REG_DLSWAP, 2);           // DLSWAP_FRAME
}

/*!
 * \brief Start touch screen calibration
 */
void Ft800::cmd_calibrate(void)
{
    uint16_t cmd_write;
    ft800_interface.read_16_bit(REG_CMD_WRITE, &cmd_write);
    ft800_interface.write_32_bit(RAM_CMD + cmd_write, 0xFFFFFF15); // CMD_CALIBRATE
    cmd_write += 4;
    ft800_interface.write_16_bit(REG_CMD_WRITE, cmd_write);
}

/*!
 * \brief Check if calibration is complete
 * \param calibration Structure to hold calibration parameters
 * \return 0 if complete, -1 otherwise
 */
int Ft800::calibration_complete(calibration_stc_t &calibration)
{
    uint16_t cmd_read, cmd_write;

    if ((ft800_interface.read_16_bit(REG_CMD_READ, &cmd_read) != 0) ||
        (ft800_interface.read_16_bit(REG_CMD_WRITE, &cmd_write) != 0))
    {
        return -1;
    }

    if (cmd_read != cmd_write)
    {
        return -1; // Calibration still running
    }

    ft800_interface.read_32_bit(REG_TOUCH_TRANSFORM_A, &calibration.transform_a);
    ft800_interface.read_32_bit(REG_TOUCH_TRANSFORM_B, &calibration.transform_b);
    ft800_interface.read_32_bit(REG_TOUCH_TRANSFORM_C, &calibration.transform_c);
    ft800_interface.read_32_bit(REG_TOUCH_TRANSFORM_D, &calibration.transform_d);
    ft800_interface.read_32_bit(REG_TOUCH_TRANSFORM_E, &calibration.transform_e);
    ft800_interface.read_32_bit(REG_TOUCH_TRANSFORM_F, &calibration.transform_f);

    return 0;
}

/*!
 * \brief Apply touchscreen calibration parameters
 */
void Ft800::set_calibration(calibration_stc_t &calibration)
{
    ft800_interface.write_32_bit(REG_TOUCH_TRANSFORM_A, calibration.transform_a);
    ft800_interface.write_32_bit(REG_TOUCH_TRANSFORM_B, calibration.transform_b);
    ft800_interface.write_32_bit(REG_TOUCH_TRANSFORM_C, calibration.transform_c);
    ft800_interface.write_32_bit(REG_TOUCH_TRANSFORM_D, calibration.transform_d);
    ft800_interface.write_32_bit(REG_TOUCH_TRANSFORM_E, calibration.transform_e);
    ft800_interface.write_32_bit(REG_TOUCH_TRANSFORM_F, calibration.transform_f);
}

/*!
 * \brief Set the clear color for the background
 */
void Ft800::clear_color_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    ft800_interface.write_32_bit(RAM_DL + 8, ft800_interface.clear_color_rgb(red, green, blue));
}

/*!
 * \brief Clear color buffer, stencil buffer, and tag buffer
 */
void Ft800::clear(uint8_t color, uint8_t stencil, uint8_t tag)
{
    ft800_interface.write_32_bit(RAM_DL + 12, ft800_interface.clear(color, stencil, tag));
}

/*!
 * \brief Display the rendered frame
 */
void Ft800::display(void)
{
    ft800_interface.write_32_bit(RAM_DL + 16, ft800_interface.display());
}

/*!
 * \brief Set the current drawing color (RGB)
 */
void Ft800::color_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    ft800_interface.write_32_bit(RAM_DL + 20, ft800_interface.color_rgb(red, green, blue));
}

/*!
 * \brief Set the current drawing alpha (transparency)
 */
void Ft800::color_alpha(uint8_t alpha)
{
    ft800_interface.write_32_bit(RAM_DL + 24, ft800_interface.color_alpha(alpha));
}

/*!
 * \brief Write a point (x, y) using fractional coordinates
 */
void Ft800::vertex2f(int32_t x, int32_t y)
{
    ft800_interface.write_32_bit(RAM_DL + 28, ft800_interface.vertex2f(x, y));
}

/*!
 * \brief Write a vertex using integer coordinates and a bitmap handle
 */
void Ft800::vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
{
    ft800_interface.write_32_bit(RAM_DL + 32, ft800_interface.vertex2ii(x, y, handle, cell));
}

/*!
 * \brief Set the active tag
 */
void Ft800::tag(uint8_t value)
{
    ft800_interface.write_32_bit(RAM_DL + 36, ft800_interface.tag(value));
}

/*!
 * \brief Set point size
 */
void Ft800::point_size(uint16_t size)
{
    ft800_interface.write_32_bit(RAM_DL + 40, ft800_interface.point_size(size));
}

/*!
 * \brief Set line width
 */
void Ft800::line_width(uint16_t width)
{
    ft800_interface.write_32_bit(RAM_DL + 44, ft800_interface.line_width(width));
}

/*!
 * \brief Send a macro command
 */
void Ft800::cmd_macro(uint8_t macro_id)
{
    uint16_t cmd_write;
    ft800_interface.read_16_bit(REG_CMD_WRITE, &cmd_write);
    ft800_interface.write_32_bit(RAM_CMD + cmd_write, 0xffffff2b); // CMD_MACRO
    cmd_write += 4;
    ft800_interface.write_32_bit(RAM_CMD + cmd_write, macro_id);
    cmd_write += 4;
    ft800_interface.write_16_bit(REG_CMD_WRITE, cmd_write);
}

/*!
 * \brief Set macro color
 */
void Ft800::set_macro_color_rgb(uint8_t macro_id, uint8_t red, uint8_t green, uint8_t blue)
{
    color_rgb(red, green, blue);
    cmd_macro(macro_id);
}

/*!
 * \brief Load a bitmap into graphics RAM
 */
void Ft800::load_bitmap(uint32_t ram_g_offset, const uint8_t* bitmap, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        ft800_interface.write_8_bit(RAM_G + ram_g_offset + i, bitmap[i]);
    }
}

/*!
 * \brief Inflate (decompress) a bitmap into graphics RAM
 */
void Ft800::cmd_inflate(uint32_t ram_g_offset, const uint8_t* bitmap, size_t length)
{
    uint16_t cmd_write;

    ft800_interface.read_16_bit(REG_CMD_WRITE, &cmd_write);

    ft800_interface.write_32_bit(RAM_CMD + cmd_write, 0xffffff22); // CMD_INFLATE
    cmd_write += 4;
    ft800_interface.write_32_bit(RAM_CMD + cmd_write, ram_g_offset);
    cmd_write += 4;

    ft800_interface.write(RAM_CMD + cmd_write, bitmap, length);
    cmd_write += (length + 3) & ~3; // 4-byte align

    ft800_interface.write_16_bit(REG_CMD_WRITE, cmd_write);
}

/*!
 * \brief Set active bitmap handle
 */
void Ft800::bitmap_handle(uint8_t handle)
{
    ft800_interface.write_32_bit(RAM_DL + 48, ft800_interface.bitmap_handle(handle));
}

/*!
 * \brief Set bitmap source address
 */
void Ft800::bitmap_source(uint32_t ram_g_offset)
{
    ft800_interface.write_32_bit(RAM_DL + 52, ft800_interface.bitmap_source(ram_g_offset));
}

/*!
 * \brief Set bitmap layout
 */
void Ft800::bitmap_layout(bitmap_format_t format, uint16_t stride, uint16_t height)
{
    ft800_interface.write_32_bit(RAM_DL + 56, ft800_interface.bitmap_layout(format, stride, height));
}

/*!
 * \brief Set bitmap size and attributes
 */
void Ft800::bitmap_size(filter_t filter, wrap_t wrap_x, wrap_t wrap_y, uint16_t width, uint16_t height)
{
    ft800_interface.write_32_bit(RAM_DL + 60, ft800_interface.bitmap_size(filter, wrap_x, wrap_y, width, height));
}

/*!
 * \brief Start drawing bitmaps
 */
void Ft800::begin_bitmap(void)
{
    ft800_interface.write_32_bit(RAM_DL + 64, ft800_interface.begin(Ft800_interface::BITMAPS));
}

/*!
 * \brief Start drawing points
 */
void Ft800::begin_point(void)
{
    ft800_interface.write_32_bit(RAM_DL + 68, ft800_interface.begin(Ft800_interface::POINTS));
}

/*!
 * \brief Start drawing lines
 */
void Ft800::begin_line(void)
{
    ft800_interface.write_32_bit(RAM_DL + 72, ft800_interface.begin(Ft800_interface::LINES));
}

/*!
 * \brief Start drawing line strip
 */
void Ft800::begin_line_strip(void)
{
    ft800_interface.write_32_bit(RAM_DL + 76, ft800_interface.begin(Ft800_interface::LINE_STRIP));
}

/*!
 * \brief Start drawing rectangles
 */
void Ft800::begin_rectangle(void)
{
    ft800_interface.write_32_bit(RAM_DL + 80, ft800_interface.begin(Ft800_interface::RECTS));
}

/*!
 * \brief End primitive drawing
 */
void Ft800::end(void)
{
    ft800_interface.write_32_bit(RAM_DL + 84, ft800_interface.end());
}

/*!
 * \brief Get raw touch coordinates
 */
int Ft800::get_touch_raw_xy(uint16_t* x, uint16_t* y)
{
    uint32_t xy;
    if (ft800_interface.read_32_bit(REG_TOUCH_RAW_XY, &xy) != 0)
        return -1;

    *x = (xy >> 16) & 0x03FF;
    *y = (xy >> 0) & 0x03FF;
    return 0;
}

/*!
 * \brief Get screen touch coordinates
 */
int Ft800::get_touch_screen_xy(uint16_t* x, uint16_t* y)
{
    uint32_t xy;
    if (ft800_interface.read_32_bit(REG_TOUCH_SCREEN_XY, &xy) != 0)
        return -1;

    *x = (xy >> 16) & 0x03FF;
    *y = (xy >> 0) & 0x03FF;
    return 0;
}

/*!
 * \brief Get current touch tag
 */
int Ft800::get_touch_tag(uint8_t* tag)
{
    return ft800_interface.read_8_bit(REG_TOUCH_TAG, tag);
}

/*!
 * \brief Vertical text (rotated 90 degrees)
 */
void Ft800::vertical_text(uint16_t x, uint16_t y, uint8_t font, const char* string)
{
    bitmap_handle(vertical_text_internal_handle);
    bitmap_layout(TEXT8X8, 8, 8);
    bitmap_size(NEAREST, BORDER, BORDER, 8, 8);

    color_rgb(255, 255, 255);

    begin_bitmap();
    for (size_t i = 0; string[i] != 0; ++i)
    {
        vertex2ii(x, y + (8 * i), vertical_text_internal_handle, string[i]);
    }
    end();
}

/*!
 * \brief Configure a sound to play when key clicked
 */
void Ft800::configure_keyclick(uint16_t sound_code)
{
    ft800_interface.write_16_bit(REG_SOUND, sound_code);
}

/*!
 * \brief Play the configured key click
 */
void Ft800::key_click(void)
{
    ft800_interface.write_8_bit(REG_PLAY, 1);
}

/*!
 * \brief Mute sound output
 */
void Ft800::mute_sound(void)
{
    ft800_interface.write_8_bit(REG_VOL_SOUND, 0);
}

/*!
 * \brief Unmute sound output
 */
void Ft800::unmute_sound(void)
{
    ft800_interface.write_8_bit(REG_VOL_SOUND, 255);
}

/*!
 * \brief Set backlight brightness
 */
void Ft800::set_backlight(uint8_t percent)
{
    ft800_interface.write_8_bit(REG_PWM_DUTY, percent);
}
