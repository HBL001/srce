/*!
 * \file ft800If.cpp
 * \brief FT800 SPI Interface Implementation
 */

 #include <stdint.h>
 #include <stdio.h>
 #include "spiIf.h"
 #include "ft800If.h"
 
 Ft800_interface::Ft800_interface(uint8_t bus_number, uint8_t channel_number)
     : spi_interface(bus_number, channel_number)
 {
     spi_interface.enable();
 }
 
 Ft800_interface::~Ft800_interface(void)
 {
     spi_interface.disable();
 }
 
 int Ft800_interface::read_8_bit(uint32_t address, uint8_t* data)
 {
     if (!data) return -1;
 
     uint8_t command_buffer[4] = {
         (uint8_t)((address >> 16) & 0x3F) | memory_read_transaction_bits,
         (uint8_t)(address >> 8),
         (uint8_t)(address),
         dummy_byte_value
     };
 
     uint8_t response[1] = { 0 };
     if (spi_interface.write_then_read(command_buffer, sizeof(command_buffer), response, sizeof(response)))
     {
         *data = response[0];
         return 0;
     }
 
     return -1;
 }
 
 int Ft800_interface::read_16_bit(uint32_t address, uint16_t* data)
 {
     if (!data) return -1;
 
     uint8_t command_buffer[4] = {
         (uint8_t)((address >> 16) & 0x3F) | memory_read_transaction_bits,
         (uint8_t)(address >> 8),
         (uint8_t)(address),
         dummy_byte_value
     };
 
     uint8_t response[2] = { 0 };
     if (spi_interface.write_then_read(command_buffer, sizeof(command_buffer), response, sizeof(response)))
     {
         *data = (response[1] << 8) | response[0];
         return 0;
     }
 
     return -1;
 }
 
 int Ft800_interface::read_32_bit(uint32_t address, uint32_t* data)
 {
     if (!data) return -1;
 
     uint8_t command_buffer[4] = {
         (uint8_t)((address >> 16) & 0x3F) | memory_read_transaction_bits,
         (uint8_t)(address >> 8),
         (uint8_t)(address),
         dummy_byte_value
     };
 
     uint8_t response[4] = { 0 };
     if (spi_interface.write_then_read(command_buffer, sizeof(command_buffer), response, sizeof(response)))
     {
         *data = (response[3] << 24) | (response[2] << 16) | (response[1] << 8) | response[0];
         return 0;
     }
 
     return -1;
 }
 
 int Ft800_interface::write_8_bit(uint32_t address, uint8_t data)
 {
     uint8_t command_buffer[4] = {
         (uint8_t)((address >> 16) & 0x3F) | memory_write_transaction_bits,
         (uint8_t)(address >> 8),
         (uint8_t)(address),
         data
     };
 
     return spi_interface.write(command_buffer, sizeof(command_buffer)) ? 0 : -1;
 }
 
 int Ft800_interface::write_16_bit(uint32_t address, uint16_t data)
 {
     uint8_t command_buffer[5] = {
         (uint8_t)((address >> 16) & 0x3F) | memory_write_transaction_bits,
         (uint8_t)(address >> 8),
         (uint8_t)(address),
         (uint8_t)(data),
         (uint8_t)(data >> 8)
     };
 
     return spi_interface.write(command_buffer, sizeof(command_buffer)) ? 0 : -1;
 }
 
 int Ft800_interface::write_32_bit(uint32_t address, uint32_t data)
 {
     uint8_t command_buffer[7] = {
         (uint8_t)((address >> 16) & 0x3F) | memory_write_transaction_bits,
         (uint8_t)(address >> 8),
         (uint8_t)(address),
         (uint8_t)(data),
         (uint8_t)(data >> 8),
         (uint8_t)(data >> 16),
         (uint8_t)(data >> 24)
     };
 
     return spi_interface.write(command_buffer, sizeof(command_buffer)) ? 0 : -1;
 }
 
 int Ft800_interface::write(uint32_t address, const uint8_t* data, size_t length)
 {
     if (!data || length == 0) return -1;
 
     uint8_t command[3] = {
         (uint8_t)((address >> 16) & 0x3F) | memory_write_transaction_bits,
         (uint8_t)(address >> 8),
         (uint8_t)(address)
     };
 
     if (spi_interface.write(command, sizeof(command)))
     {
         return spi_interface.write(data, length) ? 0 : -1;
     }
 
     return -1;
 }
 
 int Ft800_interface::host_command(command_t command)
 {
     uint8_t command_buffer[3] = { (uint8_t)command, dummy_byte_value, dummy_byte_value };
     return spi_interface.write(command_buffer, sizeof(command_buffer)) ? 0 : -1;
 }
 