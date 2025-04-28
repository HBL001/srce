/*!
 * \file spi_if.h
 * \brief SPI Interface header file
 */

 #ifndef SPI_IF_H
 #define SPI_IF_H
 
 #include <linux/types.h>
 #include <linux/spi/spidev.h>
 
 class Spi_interface
 {
 public:
     static const uint8_t default_mode = SPI_MODE_0;
     static const uint32_t default_speed = 11000000;
     static const uint8_t default_bits_per_word = 8;
 
     Spi_interface (uint8_t bus_number, uint8_t channel_number, uint8_t mode, uint32_t speed, uint8_t bits_per_word);
     Spi_interface (uint8_t bus_number, uint8_t channel_number);
     ~Spi_interface (void);
 
     bool enable (void);
     bool disable (void);
 
     bool write (const uint8_t* tx_buffer, uint32_t tx_length);
 
     bool transfer (const uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t tx_rx_length);
 
     bool write_then_read (const uint8_t* tx_buffer, uint32_t tx_length, uint8_t* rx_buffer, uint32_t rx_length);
 
 private:
     void initialise (uint8_t bus_number, uint8_t channel_number, uint8_t mode, uint32_t speed, uint8_t bits_per_word);
 
     char device_name[100];
     uint8_t spi_mode;
     uint32_t spi_speed;
     uint8_t spi_bits_per_word;
     int device_fd;
 
     struct spi_ioc_transfer transfer_structs[2];
 };
 
 #endif
 