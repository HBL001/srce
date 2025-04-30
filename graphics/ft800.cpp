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
 * \var uint32_t Ft800::cmd_offset
 * \brief Static tracker for the current write offset in the FT800 command buffer.
 *
 * Used to manage the position for appending commands to the FT800's RAM_CMD buffer.
 * This offset wraps automatically using a 4KB mask to stay within valid bounds.
 */
 uint32_t Ft800::cmd_offset = 0U;

 /*!
  * \brief Construct a new Ft800 object
  * \param bus_number SPI bus number
  * \param channel_number SPI channel (chip select) number
  */
 Ft800::Ft800(uint8_t bus_number, uint8_t channel_number)
    : interface(bus_number, channel_number)
{
    // Constructor implementation
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
         interface.host_command(Ft800_interface::ACTIVE);
         usleep(20000); // Wait 20ms
     }
 
 if (interface.read_8_bit(REG_ID, &id) != 0)
 {
     return -1;
 }

 if (id != 0x7C) // Expected ID for FT800
 {
     return -1;
 }

 interface.write_8_bit(REG_PCLK, 0); // Disable display during config

 interface.write_16_bit(REG_HCYCLE, 548);
 interface.write_16_bit(REG_HOFFSET, 43);
 interface.write_16_bit(REG_HSIZE, 480);
 interface.write_16_bit(REG_HSYNC0, 0);
 interface.write_16_bit(REG_HSYNC1, 41);

 interface.write_16_bit(REG_VCYCLE, 292);
 interface.write_16_bit(REG_VOFFSET, 12);
 interface.write_16_bit(REG_VSIZE, 272);
 interface.write_16_bit(REG_VSYNC0, 0);
 interface.write_16_bit(REG_VSYNC1, 10);

 interface.write_8_bit(REG_SWIZZLE, 0);
 interface.write_8_bit(REG_PCLK_POL, 1);
 interface.write_8_bit(REG_CSPREAD, 1);
 
     clear_color_rgb(red, green, blue);
     clear(1, 1, 1);
     display();
 
     interface.write_8_bit(REG_PCLK, 5); // Enable display, 60 Hz


     return 0;
 }

 /*!
 * \brief Check if touchscreen calibration is complete
 */
int Ft800::calibration_complete(calibration_stc_t &calibration)
{
    uint16_t cmd_read, cmd_write;

    if ((interface.read_16_bit(REG_CMD_READ, &cmd_read) != 0) ||
        (interface.read_16_bit(REG_CMD_WRITE, &cmd_write) != 0))
    {
        return -1;
    }

    if (cmd_read != cmd_write)
    {
        return -1; // Calibration still running
    }

    interface.read_32_bit(REG_TOUCH_TRANSFORM_A, &calibration.transform_a);
    interface.read_32_bit(REG_TOUCH_TRANSFORM_B, &calibration.transform_b);
    interface.read_32_bit(REG_TOUCH_TRANSFORM_C, &calibration.transform_c);
    interface.read_32_bit(REG_TOUCH_TRANSFORM_D, &calibration.transform_d);
    interface.read_32_bit(REG_TOUCH_TRANSFORM_E, &calibration.transform_e);
    interface.read_32_bit(REG_TOUCH_TRANSFORM_F, &calibration.transform_f);

    return 0;
}

/*!
 * \brief Apply touchscreen calibration parameters
 */
void Ft800::set_calibration(calibration_stc_t &calibration)
{
    interface.write_32_bit(REG_TOUCH_TRANSFORM_A, calibration.transform_a);
    interface.write_32_bit(REG_TOUCH_TRANSFORM_B, calibration.transform_b);
    interface.write_32_bit(REG_TOUCH_TRANSFORM_C, calibration.transform_c);
    interface.write_32_bit(REG_TOUCH_TRANSFORM_D, calibration.transform_d);
    interface.write_32_bit(REG_TOUCH_TRANSFORM_E, calibration.transform_e);
    interface.write_32_bit(REG_TOUCH_TRANSFORM_F, calibration.transform_f);
}

/*!
 * \brief Set the clear color for the background
 */
void Ft800::clear_color_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    interface.write_32_bit(RAM_DL + 8, interface.clear_color_rgb(red, green, blue));
}

/*!
 * \brief Clear color buffer, stencil buffer, and tag buffer
 */
void Ft800::clear(uint8_t color, uint8_t stencil, uint8_t tag)
{
    interface.write_32_bit(RAM_DL + 12, interface.clear(color, stencil, tag));
}

/*!
 * \brief Display the rendered frame
 */
void Ft800::display(void)
{
    interface.write_32_bit(RAM_DL + 16, interface.display());
}

/*!
 * \brief Set the current drawing color (RGB)
 */
void Ft800::color_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    interface.write_32_bit(RAM_DL + 20, interface.color_rgb(red, green, blue));
}

/*!
 * \brief Set the current drawing alpha (transparency)
 */
void Ft800::color_alpha(uint8_t alpha)
{
    interface.write_32_bit(RAM_DL + 24, interface.color_alpha(alpha));
}

/*!
 * \brief Write a point (x, y) using fractional coordinates
 */
void Ft800::vertex2f(int32_t x, int32_t y)
{
    interface.write_32_bit(RAM_DL + 28, interface.vertex2f(x, y));
}

/*!
 * \brief Write a vertex using integer coordinates and a bitmap handle
 */
void Ft800::vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
{
    interface.write_32_bit(RAM_DL + 32, interface.vertex2ii(x, y, handle, cell));
}

/*!
 * \brief Set display list object tag
 */
void Ft800::tag(uint8_t value)
{
    interface.write_32_bit(RAM_DL + 36, interface.tag(value));
}

/*!
 * \brief Set point size
 */
void Ft800::point_size(uint16_t size)
{
    interface.write_32_bit(RAM_DL + 40, interface.point_size(size));
}

/*!
 * \brief Set line width
 */
void Ft800::line_width(uint16_t width)
{
    interface.write_32_bit(RAM_DL + 44, interface.line_width(width));
}

/*!
 * \brief Send a macro command
 */
void Ft800::cmd_macro(uint8_t macro_id)
{
    uint16_t cmd_write;
    interface.read_16_bit(REG_CMD_WRITE, &cmd_write);
    interface.write_32_bit(RAM_CMD + cmd_write, 0xffffff2b); // CMD_MACRO
    cmd_write += 4;
    interface.write_32_bit(RAM_CMD + cmd_write, macro_id);
    cmd_write += 4;
    interface.write_16_bit(REG_CMD_WRITE, cmd_write);
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
        interface.write_8_bit(RAM_G + ram_g_offset + i, bitmap[i]);
    }
}

/*!
 * \brief Inflate (decompress) a bitmap into graphics RAM
 */
void Ft800::cmd_inflate(uint32_t ram_g_offset, const uint8_t* bitmap, size_t length)
{
    uint16_t cmd_write;

    interface.read_16_bit(REG_CMD_WRITE, &cmd_write);
    interface.write_32_bit(RAM_CMD + cmd_write, 0xffffff22); // CMD_INFLATE
    cmd_write += 4;
    interface.write_32_bit(RAM_CMD + cmd_write, ram_g_offset);
    cmd_write += 4;

    interface.write(RAM_CMD + cmd_write, bitmap, length);
    cmd_write += (length + 3) & ~3; // 4-byte align

    interface.write_16_bit(REG_CMD_WRITE, cmd_write);
}

/*!
 * \brief Start drawing points
 */
void Ft800::begin_point(void)
{
    interface.write_32_bit(RAM_DL + 68, interface.begin(Ft800_interface::POINTS));
}

/*!
 * \brief Start drawing lines
 */
void Ft800::begin_line(void)
{
    interface.write_32_bit(RAM_DL + 72, interface.begin(Ft800_interface::LINES));
}

/*!
 * \brief Start drawing line strip
 */
void Ft800::begin_line_strip(void)
{
    interface.write_32_bit(RAM_DL + 76, interface.begin(Ft800_interface::LINE_STRIP));
}

/*!
 * \brief Start drawing rectangles
 */
void Ft800::begin_rectangle(void)
{
    interface.write_32_bit(RAM_DL + 80, interface.begin(Ft800_interface::RECTS));
}

/*!
 * \brief End primitive drawing
 */
void Ft800::end(void)
{
    interface.write_32_bit(RAM_DL + 84, interface.end());
}

/*!
 * \brief Get raw touch coordinates
 */
int Ft800::get_touch_raw_xy(uint16_t* x, uint16_t* y)
{
    uint32_t xy;
    if (interface.read_32_bit(REG_TOUCH_RAW_XY, &xy) != 0)
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
    if (interface.read_32_bit(REG_TOUCH_SCREEN_XY, &xy) != 0)
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
    return interface.read_8_bit(REG_TOUCH_TAG, tag);
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
    interface.write_16_bit(REG_SOUND, sound_code);
}

/*!
 * \brief Play the configured key click
 */
void Ft800::key_click(void)
{
    interface.write_8_bit(REG_PLAY, 1);
}

/*!
 * \brief Mute sound output
 */
void Ft800::mute_sound(void)
{
    interface.write_8_bit(REG_VOL_SOUND, 0);
}

/*!
 * \brief Unmute sound output
 */
void Ft800::unmute_sound(void)
{
    interface.write_8_bit(REG_VOL_SOUND, 255);
}

/*!
 * \brief Set backlight brightness
 */
void Ft800::set_backlight(uint8_t percent)
{
    interface.write_8_bit(REG_PWM_DUTY, percent);
}

/*!
 * \brief Starts a new display list.
 *
 * Issues the CMD_DLSTART command to reset the co-processor command buffer.
 */
void Ft800::cmd_dlstart()
{
    interface.write_32_bit(ram_cmd + get_cmd_offset(), 0xFFFFFF00UL); // CMD_DLSTART
    increment_cmd_offset(4U);
}

/*!
 * \brief Draws a text string on screen using the FT800 co-processor.
 *
 * \param[in] x X coordinate
 * \param[in] y Y coordinate
 * \param[in] font Font handle (0–31)
 * \param[in] options Rendering options (e.g., FT800::OPT_CENTER)
 * \param[in] text Null-terminated C-string
 */
void Ft800::cmd_text(uint16_t x, uint16_t y, uint16_t font, option_t options, const char* text)
{
    const uint32_t offset = get_cmd_offset();
    const uint32_t text_length = static_cast<uint32_t>(strlen(text)) + 1U;

    interface.write_32_bit(ram_cmd + offset, 0x0C000000UL); // CMD_TEXT
    interface.write_32_bit(ram_cmd + offset + 4U, (static_cast<uint32_t>(y) << 16U) | x);
    interface.write_32_bit(ram_cmd + offset + 8U, (static_cast<uint32_t>(options) << 16U) | font);
    interface.write(ram_cmd + offset + 12U, reinterpret_cast<const uint8_t*>(text), text_length);
    increment_cmd_offset(12U + text_length);
}

/*!
 * \brief Swaps the display list to visible screen.
 *
 * Sends CMD_SWAP to signal the end of rendering operations.
 */
void Ft800::cmd_swap()
{
    interface.write_32_bit(ram_cmd + get_cmd_offset(), 0xFFFFFF01UL); // CMD_SWAP
    increment_cmd_offset(4U);
}

/*!
 * \brief Constructs the bitmap handle command.
 *
 * \param[in] handle Bitmap handle number (0–31)
 * \return The command word to set the handle
 */
uint32_t Ft800::bitmap_handle(uint8_t handle)
{
    return 0x05000000UL | (static_cast<uint32_t>(handle) & 0x1FU);
}

/*!
 * \brief Constructs the bitmap layout command.
 *
 * \param[in] format Bitmap format enum
 * \param[in] stride Bytes per line
 * \param[in] height Bitmap height in pixels
 * \return Command word
 */
uint32_t Ft800::bitmap_layout(bitmap_format_t format, uint16_t stride, uint16_t height)
{
    return 0x07000000UL |
           ((static_cast<uint32_t>(format) & 0x1FU) << 19U) |
           ((static_cast<uint32_t>(stride) & 0x3FFU) << 9U) |
           (static_cast<uint32_t>(height) & 0x1FFU);
}

/*!
 * \brief Constructs the bitmap size command.
 *
 * \param[in] filter Filtering mode
 * \param[in] wrap_x Horizontal wrap mode
 * \param[in] wrap_y Vertical wrap mode
 * \param[in] width Width of bitmap in pixels
 * \param[in] height Height of bitmap in pixels
 * \return Command word
 */
uint32_t Ft800::bitmap_size(filter_t filter, wrap_t wrap_x, wrap_t wrap_y, uint16_t width, uint16_t height)
{
    return 0x08000000UL |
           ((static_cast<uint32_t>(filter) & 0x01U) << 20U) |
           ((static_cast<uint32_t>(wrap_x) & 0x01U) << 19U) |
           ((static_cast<uint32_t>(wrap_y) & 0x01U) << 18U) |
           ((static_cast<uint32_t>(width) & 0x1FFU) << 9U) |
           (static_cast<uint32_t>(height) & 0x1FFU);
}

/*!
 * \brief Begins drawing a bitmap primitive.
 *
 * Equivalent to `BEGIN(BITMAPS)` in FT800 display list.
 *
 * \return Command word
 */
uint32_t Ft800::begin_bitmap()
{
    return 0x1F000001UL; // BEGIN BITMAP
}

