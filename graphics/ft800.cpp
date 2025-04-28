#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ft800If.h"
#include "ft800Reg.h"
#include "ft800.h"

Ft800::Ft800 (uint8_t bus_number, uint8_t channel_number): ft800_interface (bus_number, channel_number)
{
    display_initialised = false;
    write_index = 0;
}

Ft800::~Ft800 (void)
{
}

bool Ft800::initialise (void)
{
    bool result = false;

    if (ft800_interface.host_command (Ft800_interface::CORERST))
    {
        usleep (100000);

        if (ft800_interface.host_command (Ft800_interface::ACTIVE))
        {
            usleep (100000);

            if (ft800_interface.host_command (Ft800_interface::CLKEXT))
            {
                usleep (100000);

                // Configure display registers - WQVGA resolution
                if (ft800_interface.write_16_bit (REG_HCYCLE, 548) &&
                    ft800_interface.write_16_bit (REG_HOFFSET, 43) &&
                    ft800_interface.write_16_bit (REG_HSYNC0, 0) &&
                    ft800_interface.write_16_bit (REG_HSYNC1, 41) &&
                    ft800_interface.write_16_bit (REG_VCYCLE, 292) &&
                    ft800_interface.write_16_bit (REG_VOFFSET, 12) &&
                    ft800_interface.write_16_bit (REG_VSYNC0, 0) &&
                    ft800_interface.write_16_bit (REG_VSYNC1, 10) &&
                    ft800_interface.write_8_bit (REG_SWIZZLE, 0) &&
                    ft800_interface.write_8_bit (REG_PCLK_POL, 1) &&
                    ft800_interface.write_8_bit (REG_CSPREAD, 1) &&
                    ft800_interface.write_16_bit (REG_HSIZE, 480) &&
                    ft800_interface.write_16_bit (REG_VSIZE, 272))
                {
                    // Write initial display to clear the display
                    if (ft800_interface.write_32_bit (RAM_DL+0, ft800_interface.clear_color_rgb (0, 0, 0)) &&
                        ft800_interface.write_32_bit (RAM_DL+4, ft800_interface.clear (1, 1, 1)) &&
                        ft800_interface.write_32_bit (RAM_DL+8, ft800_interface.display ()) &&
                        ft800_interface.write_8_bit (REG_DLSWAP, DLSWAP_FRAME))
                    {
                        // Enable the display bit
                        uint8_t result_8;
                        if (ft800_interface.read_8_bit (REG_GPIO_DIR, &result_8))
                        {
                            if (ft800_interface.write_8_bit (REG_GPIO_DIR, 0x83 | result_8))
                            {
                                if (ft800_interface.read_8_bit (REG_GPIO, &result_8))
                                {
                                    if (ft800_interface.write_8_bit (REG_GPIO, 0x083 | result_8))
                                    {
                                        // Final command to make display visible...
                                        if (ft800_interface.write_8_bit (REG_PCLK, 5))
                                        {
                                            usleep (20000);
                                            display_initialised = true;
                                            write_index = 0;
                                            result = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

void Ft800::cmd_dlstart (void)
{
    if (display_initialised)
    {
        write_command_32_bit (dlstart_command);
    }
}

void Ft800::cmd_swap (void)
{
    if (display_initialised)
    {
        write_command_32_bit (dlswap_command);
    }
}

void Ft800::cmd_calibrate (void)
{
    if (display_initialised)
    {
        write_command_32_bit (calibrate_command);
        write_command_32_bit (0);
    }
}

bool Ft800::calibration_complete (calibration_stc_t &calibration)
{
    bool result = false;

    if (display_initialised)
    {
        if (fifo_empty ())
        {
            if (ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_A, &calibration.transform_a) &&
                ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_B, &calibration.transform_b) &&
                ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_C, &calibration.transform_c) &&
                ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_D, &calibration.transform_d) &&
                ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_E, &calibration.transform_e) &&
                ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_F, &calibration.transform_f))
            {
                result = true;
            }
        }
    }

    return result;
}

void Ft800::set_calibration (calibration_stc_t &calibration)
{
    if (display_initialised)
    {
        ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_A, calibration.transform_a);
        ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_B, calibration.transform_b);
        ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_C, calibration.transform_c);
        ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_D, calibration.transform_d);
        ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_E, calibration.transform_e);
        ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_F, calibration.transform_f);
    }
}

void Ft800::clear_color_rgb (uint8_t red, uint8_t green, uint8_t blue)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.clear_color_rgb  (red, green, blue));
    }
}

void Ft800::clear (uint8_t color, uint8_t stencil, uint8_t tag)
{
    if (display_initialised)
    {
       write_command_32_bit (ft800_interface.clear (color, stencil, tag));
    }
}

void Ft800::display (void)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.display ());
    }
}

void Ft800::color_rgb (uint8_t red, uint8_t green, uint8_t blue)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.color_rgb  (red, green, blue));
    }
}

void Ft800::cmd_text (uint16_t x, uint16_t y, uint16_t font, option_t option, const char* string)
{
    if (display_initialised)
    {
        write_command_32_bit (text_command);
        write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
        write_command_32_bit (((uint32_t)option << 16) | (uint32_t)font);
        write_command_string (string);
    }
}

void Ft800::cmd_button (uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, option_t option, const char* string)
{
    if (display_initialised)
    {
        write_command_32_bit (button_command);
        write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
        write_command_32_bit (((uint32_t)height << 16) | (uint32_t)width);
        write_command_32_bit (((uint32_t)option << 16) | (uint32_t)font);
        write_command_string (string);
    }
}

void Ft800::cmd_spinner (uint16_t x, uint16_t y, spinner_style_t style, uint16_t scale)
{
    if (display_initialised)
    {
        write_command_32_bit (spinner_command);
        write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
        write_command_32_bit (((uint32_t)scale << 16) | (uint32_t)style);
    }
}

void Ft800::load_bitmap (uint32_t ram_g_offset, const uint8_t* bitmap, size_t length)
{
    if (display_initialised)
    {
        for (size_t index = 0; index < length; index++)
        {
            ft800_interface.write_8_bit (RAM_G + ram_g_offset + index, bitmap[index]);
        }
    }
}

void Ft800::bitmap_source (uint32_t ram_g_offset)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.bitmap_source (RAM_G + ram_g_offset));
    }
}

void Ft800::bitmap_layout (bitmap_format_t format, uint16_t stride, uint16_t height)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.bitmap_layout ((uint16_t)format, stride, height));
    }
}

void Ft800::bitmap_size (filter_t filter, wrap_t wrap_x, wrap_t wrap_y, uint16_t width, uint16_t height)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.bitmap_size ((uint8_t)filter, (uint8_t)wrap_x, (uint8_t)wrap_y, width, height));
    }
}

void Ft800::begin_bitmap (void)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.begin (Ft800_interface::BITMAPS));
    }
}

void Ft800::end (void)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.end ());
    }
}

void Ft800::vertex2ii (uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.vertex2ii (x, y, handle, cell));
    }
}

void Ft800::tag (uint8_t value)
{
    if (display_initialised)
    {
        write_command_32_bit (ft800_interface.tag (value));
    }
}

bool Ft800::get_touch_raw_xy (uint16_t* x, uint16_t* y)
{
    bool result = false;

    if ((NULL != x) && (NULL != y))
    {
        uint32_t raw_xy_value;
        if (ft800_interface.read_32_bit (REG_TOUCH_RAW_XY, &raw_xy_value))
        {
            if (0xFFFFFFFF != raw_xy_value)
            {
                *x = (uint16_t)(raw_xy_value >> 16);
                *y = (uint16_t)(raw_xy_value &  0xFFFF);
                result = true;
            }
        }
    }

    return result;
}

bool Ft800::get_touch_screen_xy (uint16_t* x, uint16_t* y)
{
    bool result = false;

    if ((NULL != x) && (NULL != y))
    {
        uint32_t screen_xy_value;
        if (ft800_interface.read_32_bit (REG_TOUCH_SCREEN_XY, &screen_xy_value))
        {
            if (0x80008000 != screen_xy_value)
            {
                *x = (uint16_t)(screen_xy_value >> 16);
                *y = (uint16_t)(screen_xy_value &  0xFFFF);
                result = true;
            }
        }
    }

    return result;
}

bool Ft800::get_touch_tag (uint8_t* tag)
{
    bool result = false;

    if (NULL != tag)
    {
        uint8_t tag_value;
        if (ft800_interface.read_8_bit (REG_TOUCH_TAG, &tag_value))
        {
            *tag = tag_value;
            result = true;
        }
    }

    return result;
}

bool Ft800::write_command_32_bit (uint32_t value)
{
    bool result = false;

    if (ft800_interface.write_32_bit (RAM_CMD + write_index, value))
    {
        update_fifo_write_pointer (sizeof (uint32_t));
        result = true;
    }

    return result;
}

bool Ft800::write_command_string (const char *string)
{
    // Write the (null terminated) string
    return write_command_buffer ((uint8_t*)string, strlen (string) + 1);
}

bool Ft800::write_command_buffer (const uint8_t* buffer, size_t length)
{
    bool result = true;

    size_t index = 0;

    while ((index < length) && result)
    {
        // Handle fifo wrap-round
        uint32_t fifo_address = RAM_CMD + ((write_index + index) % command_fifo_size);
        result = ft800_interface.write_8_bit (fifo_address, buffer[index]);
        index++;
    }

    if (result)
    {
        update_fifo_write_pointer (length);
    }

    return result;
}

void Ft800::update_fifo_write_pointer (size_t size)
{
    // Ensure increment of 4 byte multiples
    if (0 != (size % 4))
    {
        size += 4 - (size % 4);
    }
    write_index += (uint16_t)size;

    // Wrap round the circular buffer
    write_index %= command_fifo_size;

    // Update the FT800
    ft800_interface.write_32_bit (REG_CMD_WRITE, write_index);
}

bool Ft800::fifo_empty (void)
{
    bool result = false;

    uint32_t cmd_read;
    if (ft800_interface.read_32_bit (REG_CMD_READ, &cmd_read))
    {
        if (cmd_read == write_index)
        {
            result = true;
        }
    }

    return result;
}


#ifdef FT800_TESTS
#include "spanner_icon.h"
#include "battery_icon.h"

int main (void)
{
    Ft800 ft800 (1, 0);
    
    if (ft800.initialise ())
    {
#define CALIBRATION_REQUIRED
#ifdef CALIBRATION_REQUIRED
        ft800.cmd_dlstart ();
        ft800.clear (1, 1, 1);
        ft800.cmd_text (250, 110, 30, Ft800::OPT_CENTER, "Calibration required");
        ft800.cmd_text (250, 138, 28, Ft800::OPT_CENTER, "Please tap on the dot");
        ft800.cmd_calibrate();

        Ft800::calibration_stc_t calibration;
        while (!ft800.calibration_complete (calibration))
        {
            usleep (1000);
        }
        printf ("Calibration complete\n");
        printf ("\tCalibration transform A: 0x%08X\n", calibration.transform_a);
        printf ("\tCalibration transform B: 0x%08X\n", calibration.transform_b);
        printf ("\tCalibration transform C: 0x%08X\n", calibration.transform_c);
        printf ("\tCalibration transform D: 0x%08X\n", calibration.transform_d);
        printf ("\tCalibration transform E: 0x%08X\n", calibration.transform_e);
        printf ("\tCalibration transform F: 0x%08X\n", calibration.transform_f);
#endif

        static const uint32_t small_spanner = 0;
        ft800.load_bitmap (small_spanner, small_spanner_icon, sizeof (small_spanner_icon));
        static const uint32_t battery_75_percent = small_spanner + sizeof (small_spanner_icon);
        ft800.load_bitmap (battery_75_percent, battery_75_percent_icon, sizeof (battery_75_percent_icon));

        ft800.cmd_dlstart ();
        ft800.clear_color_rgb (0, 0, 0);
        ft800.clear (1, 1, 1);

        ft800.color_rgb (0xbb, 0xf7, 0xf9);
        ft800.cmd_text (400, 14, 28, Ft800::OPT_CENTER, "37.5C");

        ft800.color_rgb (0xbb, 0xf7, 0xf9);
        ft800.cmd_text (240, 90, 30, Ft800::OPT_CENTER, "LiquidIC Oil Condition Tester");

        ft800.bitmap_source (battery_75_percent);
        ft800.bitmap_layout (Ft800::ARGB1555, 82, 20);
        ft800.bitmap_size (Ft800::NEAREST, Ft800::BORDER, Ft800::BORDER, 41, 20);
        ft800.begin_bitmap ();
        ft800.vertex2ii (435, 5, 0, 0);
        ft800.end ();

        ft800.tag (0x80);
        ft800.bitmap_source (small_spanner);
        ft800.bitmap_layout (Ft800::ARGB1555, 60, 30);
        ft800.bitmap_size (Ft800::NEAREST, Ft800::BORDER, Ft800::BORDER, 30, 30);
        ft800.begin_bitmap ();
        ft800.vertex2ii (445, 235, 0, 0);
        ft800.end ();

        ft800.tag ('S');
        ft800.color_rgb (255, 255, 255);
        ft800.cmd_button (190, 170, 100, 40, 24, Ft800::OPT_3D, "Start");

        ft800.display ();
        ft800.cmd_swap ();

        while (true)
        {
            uint16_t raw_x_value;
            uint16_t raw_y_value;
            if (ft800.get_touch_raw_xy (&raw_x_value, &raw_y_value))
            {
                uint16_t screen_x_value = 0;
                uint16_t screen_y_value = 0;
                uint8_t tag_value = 0;
                ft800.get_touch_screen_xy (&screen_x_value, &screen_y_value);
                ft800.get_touch_tag (&tag_value);
                printf ("%d:%d - %d:%d - %02X\n",
                        raw_x_value, raw_y_value,
                        screen_x_value, screen_y_value,
                        tag_value);
                usleep (100000);
                if ('S' == tag_value)
                {
                    ft800.cmd_dlstart ();
                    ft800.clear (1, 1, 1);

                    ft800.color_rgb (0xbb, 0xf7, 0xf9);
                    ft800.cmd_text (400, 14, 28, Ft800::OPT_CENTER, "40.0C");

                    ft800.bitmap_source (battery_75_percent);
                    ft800.bitmap_layout (Ft800::ARGB1555, 82, 20);
                    ft800.bitmap_size (Ft800::NEAREST, Ft800::BORDER, Ft800::BORDER, 41, 20);
                    ft800.begin_bitmap ();
                    ft800.vertex2ii (435, 5, 0, 0);
                    ft800.end ();

                    ft800.color_rgb (0xff, 0xff, 0xff);
                    ft800.cmd_text (240, 90, 29, Ft800::OPT_CENTER, "Measurement in progress");
                    ft800.cmd_text (240, 140, 28, Ft800::OPT_CENTER, "Please wait...");
                    ft800.cmd_spinner (240, 180, Ft800::ORBIT, 0);
                }
            }
            else
            {
                usleep (1000);
            }
        }
    }
    else
    {
        printf ("Failed to initialise FT800\n");
    }

    return 0;
}

#endif    
