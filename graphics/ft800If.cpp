/*!
 * \file ft800If.cpp
 * \brief FT800 SPI Interface Implementation
 */

 #include <cstring>
 #include <cstddef>
 #include <cstdint>
 #include <new>
 #include "spiIf.h"
 #include "ft800If.h"
 
 /*!
  * \brief Constructor for FT800 interface using SPI.
  */
 Ft800_interface::Ft800_interface(uint8_t bus_number, uint8_t channel_number)
     : spi_interface(bus_number, channel_number)
 {
     spi_interface.enable();
 }
 
 /*!
  * \brief Destructor for FT800 interface.
  */
 Ft800_interface::~Ft800_interface(void)
 {
     spi_interface.disable();
 }
 
 /*!
  * \brief Send a host command to the FT800 chip.
  */
 int Ft800_interface::host_command(command_t cmd)
 {
     uint8_t buffer[1] = { static_cast<uint8_t>(cmd) };
     return spi_interface.write(buffer, sizeof(buffer)) ? 0 : -1;
 }
 
 /*!
  * \brief Write 8-bit value to FT800 memory.
  */
 int Ft800_interface::write_8_bit(uint32_t address, uint8_t value)
 {
     uint8_t buffer[4] = {
         static_cast<uint8_t>((address >> 16) & 0x3F),
         static_cast<uint8_t>(address >> 8),
         static_cast<uint8_t>(address),
         value
     };
     return spi_interface.write(buffer, sizeof(buffer)) ? 0 : -1;
 }
 
 /*!
  * \brief Write 16-bit value to FT800 memory.
  */
 int Ft800_interface::write_16_bit(uint32_t address, uint16_t value)
 {
     uint8_t buffer[5] = {
         static_cast<uint8_t>((address >> 16) & 0x3F),
         static_cast<uint8_t>(address >> 8),
         static_cast<uint8_t>(address),
         static_cast<uint8_t>(value & 0xFF),
         static_cast<uint8_t>((value >> 8) & 0xFF)
     };
     return spi_interface.write(buffer, sizeof(buffer)) ? 0 : -1;
 }
 
 /*!
  * \brief Write 32-bit value to FT800 memory.
  */
 int Ft800_interface::write_32_bit(uint32_t address, uint32_t value)
 {
     uint8_t buffer[7] = {
         static_cast<uint8_t>((address >> 16) & 0x3F),
         static_cast<uint8_t>(address >> 8),
         static_cast<uint8_t>(address),
         static_cast<uint8_t>(value & 0xFF),
         static_cast<uint8_t>((value >> 8) & 0xFF),
         static_cast<uint8_t>((value >> 16) & 0xFF),
         static_cast<uint8_t>((value >> 24) & 0xFF)
     };
     return spi_interface.write(buffer, sizeof(buffer)) ? 0 : -1;
 }
 
 /*!
 * \brief Reads an 8-bit value from a specified FT800 memory address.
 *
 * Constructs a 4-byte SPI read transaction for the FT800 memory-mapped I/O and
 * retrieves a single byte of data.
 *
 * \param address 32-bit FT800 address to read from.
 * \param [out] data Pointer to variable where the read byte will be stored.
 * \return 0 on success, -1 on failure or if data is null.
 */
int Ft800_interface::read_8_bit(uint32_t address, uint8_t* data)
{
    if (data == nullptr)
    {
        return -1;
    }

    const uint8_t command_byte = static_cast<uint8_t>(
        (static_cast<uint8_t>((address >> 16U) & 0x3FU)) |
        static_cast<uint8_t>(memory_read_transaction_bits));

    uint8_t cmd[4] = {
        command_byte,
        static_cast<uint8_t>((address >> 8U) & 0xFFU),
        static_cast<uint8_t>(address & 0xFFU),
        dummy_byte_value
    };

    uint8_t response[1] = { 0U };

    if (spi_interface.write_then_read(cmd, sizeof(cmd), response, sizeof(response)))
    {
        *data = response[0];
        return 0;
    }

    return -1;
}
 
 /*!
 * \brief Reads a 16-bit value from a specified FT800 memory address.
 *
 * Constructs a 4-byte SPI read transaction and reads two bytes from FT800.
 *
 * \param address 32-bit FT800 address to read from.
 * \param[out] data Pointer to variable where the 16-bit value will be stored.
 * \return 0 on success, -1 on failure or if data is null.
 */
int Ft800_interface::read_16_bit(uint32_t address, uint16_t* data)
{
    if (data == nullptr)
    {
        return -1;
    }

    const uint8_t command_byte = static_cast<uint8_t>(
        (static_cast<uint8_t>((address >> 16U) & 0x3FU)) |
        static_cast<uint8_t>(memory_read_transaction_bits));

    uint8_t cmd[4] = {
        command_byte,
        static_cast<uint8_t>((address >> 8U) & 0xFFU),
        static_cast<uint8_t>(address & 0xFFU),
        dummy_byte_value
    };

    uint8_t response[2] = { 0U, 0U };

    if (spi_interface.write_then_read(cmd, sizeof(cmd), response, sizeof(response)))
    {
        *data = static_cast<uint16_t>(response[0]) |
                (static_cast<uint16_t>(response[1]) << 8U);
        return 0;
    }

    return -1;
}

 /*!
 * \brief Reads a 32-bit value from a specified FT800 memory address.
 *
 * Constructs a 4-byte SPI read transaction and reads four bytes from FT800 memory.
 *
 * \param address 32-bit FT800 address to read from.
 * \param[out] data Pointer to variable where the 32-bit value will be stored.
 * \return 0 on success, -1 on failure or if data is null.
 */
int Ft800_interface::read_32_bit(uint32_t address, uint32_t* data)
{
    if (data == nullptr)
    {
        return -1;
    }

    const uint8_t command_byte = static_cast<uint8_t>(
        (static_cast<uint8_t>((address >> 16U) & 0x3FU)) |
        static_cast<uint8_t>(memory_read_transaction_bits));

    uint8_t cmd[4] = {
        command_byte,
        static_cast<uint8_t>((address >> 8U) & 0xFFU),
        static_cast<uint8_t>(address & 0xFFU),
        dummy_byte_value
    };

    uint8_t response[4] = { 0U, 0U, 0U, 0U };

    if (spi_interface.write_then_read(cmd, sizeof(cmd), response, sizeof(response)))
    {
        *data = static_cast<uint32_t>(response[0]) |
                (static_cast<uint32_t>(response[1]) << 8U) |
                (static_cast<uint32_t>(response[2]) << 16U) |
                (static_cast<uint32_t>(response[3]) << 24U);
        return 0;
    }

    return -1;
}

 
 /*!
  * \brief Write raw buffer to FT800 memory.
  */
 int Ft800_interface::write(uint32_t address, const uint8_t* data, size_t length)
 {
     if ((data == nullptr) || (length == 0U)) return -1;
 
     uint8_t header[3] = {
         static_cast<uint8_t>((address >> 16) & 0x3F),
         static_cast<uint8_t>(address >> 8),
         static_cast<uint8_t>(address)
     };
 
     const size_t total = length + sizeof(header);
     uint8_t* buffer = new (std::nothrow) uint8_t[total];
     if (buffer == nullptr) return -1;
 
     std::memcpy(buffer, header, sizeof(header));
     std::memcpy(buffer + sizeof(header), data, length);
 
     bool result = spi_interface.write(buffer, total);
     delete[] buffer;
     return result ? 0 : -1;
 }
 
 /*!
  * \brief Construct CLEAR_COLOR_RGB command.
  */
 uint32_t Ft800_interface::clear_color_rgb(uint8_t red, uint8_t green, uint8_t blue)
 {
     return clear_color_rgb_bits | ((static_cast<uint32_t>(red) << 16U) |
                                    (static_cast<uint32_t>(green) << 8U) |
                                    static_cast<uint32_t>(blue));
 }
 
 /*!
  * \brief Construct CLEAR command.
  */
 uint32_t Ft800_interface::clear(uint8_t color, uint8_t stencil, uint8_t tag)
 {
     return clear_bits |
            ((color != 0U ? clear_color_buffer_bit : 0U) |
             (stencil != 0U ? clear_stencil_bit : 0U) |
             (tag != 0U ? clear_tag_bit : 0U));
 }
 
 /*!
  * \brief Construct COLOR_RGB command.
  */
 uint32_t Ft800_interface::color_rgb(uint8_t red, uint8_t green, uint8_t blue)
 {
     return color_rgb_bits | ((static_cast<uint32_t>(red) << 16U) |
                              (static_cast<uint32_t>(green) << 8U) |
                              static_cast<uint32_t>(blue));
 }
 
 /*!
  * \brief Construct COLOR_A command.
  */
 uint32_t Ft800_interface::color_alpha(uint8_t alpha)
 {
     return color_alpha_bits | static_cast<uint32_t>(alpha);
 }
 
 /*!
  * \brief Construct BEGIN command.
  */
 uint32_t Ft800_interface::begin(Begin_primitive_t primitive)
 {
     return begin_bits | static_cast<uint32_t>(primitive);
 }
 
 /*!
  * \brief Construct VERTEX2F command.
  */
 uint32_t Ft800_interface::vertex2f(int32_t x, int32_t y)
 {
     return vertex2f_bits | ((static_cast<uint32_t>(x & 0x7FFF) << 15U) |
                             static_cast<uint32_t>(y & 0x7FFF));
 }
 
 /*!
  * \brief Construct VERTEX2II command.
  */
 uint32_t Ft800_interface::vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
 {
     return vertex2ii_bits |
            ((static_cast<uint32_t>(x) << 21U) |
             (static_cast<uint32_t>(y) << 12U) |
             (static_cast<uint32_t>(handle) << 7U) |
             static_cast<uint32_t>(cell));
 }
 
 /*!
  * \brief Construct POINT_SIZE command.
  */
 uint32_t Ft800_interface::point_size(uint16_t size)
 {
     return 0x0D000000U | static_cast<uint32_t>(size & 0x1FFFU);
 }
 
 /*!
  * \brief Construct LINE_WIDTH command.
  */
 uint32_t Ft800_interface::line_width(uint16_t width)
 {
     return 0x0E000000U | static_cast<uint32_t>(width & 0x0FFFU);
 }
 
 /*!
  * \brief Construct MACRO command.
  */
 uint32_t Ft800_interface::macro(uint8_t macro_id)
 {
     return 0x25000000U | static_cast<uint32_t>(macro_id);
 }
 
 /*!
  * \brief Construct BITMAP_SOURCE command.
  */
 uint32_t Ft800_interface::bitmap_source(uint32_t address)
 {
     return bitmap_source_bits | (address & 0x3FFFFFU);
 }
 
 /*!
  * \brief Construct BITMAP_LAYOUT command.
  */
 uint32_t Ft800_interface::bitmap_layout(uint16_t format, uint16_t stride, uint16_t height)
 {
     return bitmap_layout_bits |
            ((static_cast<uint32_t>(format & 0x1FU) << 19U) |
             (static_cast<uint32_t>(stride & 0x3FFU) << 9U) |
             (height & 0x1FFU));
 }
 
 /*!
  * \brief Construct BITMAP_SIZE command.
  */
 uint32_t Ft800_interface::bitmap_size(uint8_t filter, uint8_t wrap_x, uint8_t wrap_y, uint16_t width, uint16_t height)
 {
     return bitmap_size_bits |
            ((static_cast<uint32_t>(filter & 1U) << 20U) |
             (static_cast<uint32_t>(wrap_x & 1U) << 19U) |
             (static_cast<uint32_t>(wrap_y & 1U) << 18U) |
             (static_cast<uint32_t>(width & 0x1FFU) << 9U) |
             (height & 0x1FFU));
 }
 
 /*!
  * \brief Construct BITMAP_HANDLE command.
  */
 uint32_t Ft800_interface::bitmap_handle(uint8_t handle)
 {
     return bitmap_handle_bits | (handle & 0x1FU);
 }
 
 /*!
  * \brief Construct TAG command.
  */
 uint32_t Ft800_interface::tag(uint8_t value)
 {
     return tag_bits | static_cast<uint32_t>(value);
 }
 