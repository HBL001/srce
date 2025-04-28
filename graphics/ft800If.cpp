#include <stdint.h>
#include <stdio.h>

#include "spiIf.h"
#include "ft800If.h"

Ft800_interface::Ft800_interface (uint8_t bus_number, uint8_t channel_number) : spi_interface (bus_number, channel_number)
{
    spi_interface.enable ();
}

Ft800_interface::~Ft800_interface (void)
{
    spi_interface.disable ();
}

bool Ft800_interface::read_8_bit (uint32_t address, uint8_t* data)
{
    bool result = false;

    if (NULL != data)
    {
        uint8_t response_buffer[1];

        uint8_t command_buffer[4];
        command_buffer[0] = (uint8_t)((address >> 16) & 0x3F) | memory_read_transaction_bits;
        command_buffer[1] = (uint8_t)((address >>  8) & 0xFF);
        command_buffer[2] = (uint8_t)((address >>  0) & 0xFF);
        command_buffer[3] = dummy_byte_value;

        if (spi_interface.write_then_read (command_buffer, sizeof (command_buffer),
                                           response_buffer, sizeof (response_buffer)))
        {
            *data = response_buffer[0];
            result = true;
        }
    }

    return result;
}

bool Ft800_interface::read_16_bit (uint32_t address, uint16_t* data)
{
    bool result = false;

    if (NULL != data)
    {
        uint8_t response_buffer[2];

        uint8_t command_buffer[4];
        command_buffer[0] = (uint8_t)((address >> 16) & 0x3F) | memory_read_transaction_bits;
        command_buffer[1] = (uint8_t)((address >>  8) & 0xFF);
        command_buffer[2] = (uint8_t)((address >>  0) & 0xFF);
        command_buffer[3] = dummy_byte_value;

        if (spi_interface.write_then_read (command_buffer, sizeof (command_buffer),
                                           response_buffer, sizeof (response_buffer)))
        {
            *data = (uint16_t)response_buffer[0];
            *data |= (((uint16_t)response_buffer[1]) << 8);
            result = true;
        }
    }
    
    return result;
}

bool Ft800_interface::read_32_bit (uint32_t address, uint32_t* data)
{
    bool result = false;

    if (NULL != data)
    {
        uint8_t response_buffer[4];

        uint8_t command_buffer[4];
        command_buffer[0] = (uint8_t)((address >> 16) & 0x3F) | memory_read_transaction_bits;
        command_buffer[1] = (uint8_t)((address >>  8) & 0xFF);
        command_buffer[2] = (uint8_t)((address >>  0) & 0xFF);
        command_buffer[3] = dummy_byte_value;

        if (spi_interface.write_then_read (command_buffer, sizeof (command_buffer),
                                           response_buffer, sizeof (response_buffer)))
        {
            *data  = response_buffer[0];
            *data |= (((uint32_t)response_buffer[1]) <<  8);
            *data |= (((uint32_t)response_buffer[2]) << 16);
            *data |= (((uint32_t)response_buffer[3]) << 24);
            result = true;
        }
    }
    
    return result;
}

bool Ft800_interface::write_8_bit (uint32_t address, uint8_t data)
{
    bool result = false;

    uint8_t command_buffer[4];
    command_buffer[0] = (uint8_t)((address >> 16) & 0x3F) | memory_write_transaction_bits;
    command_buffer[1] = (uint8_t)((address >>  8) & 0xFF);
    command_buffer[2] = (uint8_t)((address >>  0) & 0xFF);
    command_buffer[3] = data;

    if (spi_interface.write (command_buffer, sizeof (command_buffer)))
    {
        result = true;
    }

    return result;
}

bool Ft800_interface::write_16_bit (uint32_t address, uint16_t data)
{
    bool result = false;

    uint8_t command_buffer[5];
    command_buffer[0] = (uint8_t)((address >> 16) & 0x3F) | memory_write_transaction_bits;
    command_buffer[1] = (uint8_t)((address >>  8) & 0xFF);
    command_buffer[2] = (uint8_t)((address >>  0) & 0xFF);
    command_buffer[3] = (uint8_t)((data >> 0) & 0xFF);
    command_buffer[4] = (uint8_t)((data >> 8) & 0xFF);

    if (spi_interface.write (command_buffer, sizeof (command_buffer)))
    {
        result = true;
    }

    return result;
}

bool Ft800_interface::write_32_bit (uint32_t address, uint32_t data)
{
    bool result = false;

    uint8_t command_buffer[7];
    command_buffer[0] = (uint8_t)((address >> 16) & 0x3F) | memory_write_transaction_bits;
    command_buffer[1] = (uint8_t)((address >>  8) & 0xFF);
    command_buffer[2] = (uint8_t)((address >>  0) & 0xFF);
    command_buffer[3] = (uint8_t)((data >>  0) & 0xFF);
    command_buffer[4] = (uint8_t)((data >>  8) & 0xFF);
    command_buffer[5] = (uint8_t)((data >> 16) & 0xFF);
    command_buffer[6] = (uint8_t)((data >> 24) & 0xFF);

    if (spi_interface.write (command_buffer, sizeof (command_buffer)))
    {
        result = true;
    }

    return result;
}

bool Ft800_interface::host_command (command_t command)
{
    bool result = false;

    uint8_t command_buffer[3];
    command_buffer[0] = (uint8_t)command;
    command_buffer[1] = dummy_byte_value;
    command_buffer[2] = dummy_byte_value;

    if (spi_interface.write (command_buffer, sizeof (command_buffer)))
    {
        result = true;
    }

    return result;
}

uint32_t Ft800_interface::clear_color_rgb (uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t result = clear_color_rgb_bits;

    result |= ((uint32_t)red << 16);
    result |= ((uint32_t)green << 8);
    result |= ((uint32_t)blue << 0);

    return result;
}

uint32_t Ft800_interface::clear (uint8_t color, uint8_t stencil, uint8_t tag)
{
    uint32_t result = clear_bits;

    if (0 != color)
    {
        result |= clear_color_buffer_bit;
    }
    if (0 != stencil)
    {
        result |= clear_stencil_bit;
    }
    if (0 != tag)
    {
        result |= clear_tag_bit;
    }

    return result;
}

uint32_t Ft800_interface::color_rgb (uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t result = color_rgb_bits;

    result |= ((uint32_t)red << 16);
    result |= ((uint32_t)green << 8);
    result |= ((uint32_t)blue << 0);

    return result;
}

uint32_t Ft800_interface::begin (Begin_primative_t primative)
{
    uint32_t result = begin_bits;

    result |= (uint32_t)primative;

    return result;
}

uint32_t Ft800_interface::vertex2ii (uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
{
    uint32_t result = vertex2ii_bits;

    result |= ((uint32_t)(x & 0x1FF)) << 21;
    result |= ((uint32_t)(y & 0x1FF)) << 12;
    result |= ((uint32_t)(handle & 0x1F)) << 7;
    result |= (uint32_t)(cell & 0x7F);

    return result;
}

uint32_t Ft800_interface::bitmap_source (uint32_t address)
{
    uint32_t result = bitmap_source_bits;

    result |= (address & 0xFFFFF);

    return result;
}

uint32_t Ft800_interface::bitmap_layout (uint16_t format, uint16_t stride, uint16_t height)
{
    uint32_t result = bitmap_layout_bits;

    result |= (format & 0x1F) << 19;
    result |= (stride & 0x3FF) << 9;
    result |= (height & 0x1FF);

    return result;
}

uint32_t Ft800_interface::bitmap_size (uint8_t filter, uint8_t wrap_x, uint8_t wrap_y, uint16_t width, uint16_t height)
{
    uint32_t result = bitmap_size_bits;

    result |= (filter & 1) << 20;
    result |= (wrap_x & 1) << 19;
    result |= (wrap_y & 1) << 18;
    result |= (width & 0x1FF) << 9;
    result |= (height & 0x1FF);

    return result;
}

uint32_t Ft800_interface::tag (uint8_t value)
{
    uint32_t result = tag_bits;

    result |= value;

    return result;
}

#ifdef PERFORM_FT800_INTERFACE_TESTS

int main (void)
{
    Ft800_interface ft800_interface (1, 0);
    uint8_t result_8;
    uint16_t result_16;
    uint32_t result_32;

    if (ft800_interface.read_8_bit (0, &result_8))
    {
        printf ("Read  8: %02X\n", result_8);
    }
    else
    {
        printf ("read_8_bit failed\n");
    }
    if (ft800_interface.read_16_bit (0x100, &result_16))
    {
        printf ("Read 16: %04X\n", result_16);
    }
    else
    {
        printf ("read_16_bit failed\n");
    }
    if (ft800_interface.read_32_bit (0x20000, &result_32))
    {
        printf ("Read 32: %08X\n", result_32);
    }
    else
    {
        printf ("read_32_bit failed\n");
    }

    if (ft800_interface.write_8_bit (0x0, 0xAB))
    {
        printf ("write_8_bit OK\n");
    }
    else
    {
        printf ("write_8_bit failed\n");
    }
    if (ft800_interface.write_16_bit (0x800, 0xABCD))
    {
        printf ("write_16_bit OK\n");
    }
    else
    {
        printf ("write_16_bit failed\n");
    }
    if (ft800_interface.write_32_bit (0x200000, 0xABCDEF55))
    {
        printf ("write_32_bit OK\n");
    }
    else
    {
        printf ("write_32_bit failed\n");
    }

    return 0;
}    
#endif
