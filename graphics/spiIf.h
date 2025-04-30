/*!
 * \file spiIf.h
 * \brief SPI Interface abstraction for FT800 SDK (Linux spidev-based)
 */

 #ifndef SPI_IF_H
 #define SPI_IF_H
 
 #include <cstdint>
 #include <linux/types.h>
 #include <linux/spi/spidev.h>
 
 /*!
  * \class Spi_interface
  * \brief Manages low-level SPI transfers over spidev.
  *
  * This class provides an abstraction over Linux's SPI interface using ioctl and spidev.
  * It avoids dynamic memory and is safe for embedded environments.
  */
 class Spi_interface
 {
 public:
     static constexpr uint8_t default_mode = SPI_MODE_0;
     static constexpr uint32_t default_speed = 11000000;
     static constexpr uint8_t default_bits_per_word = 8;
 
     /*!
      * \brief Constructor with full configuration
      * \param bus_number SPI bus number (e.g., 1 for /dev/spidev1.x)
      * \param channel_number SPI channel (CS) number
      * \param mode SPI mode (usually SPI_MODE_0)
      * \param speed SPI clock speed in Hz
      * \param bits_per_word Bit length per SPI frame (typically 8)
      */
     Spi_interface(uint8_t bus_number, uint8_t channel_number,
                   uint8_t mode, uint32_t speed, uint8_t bits_per_word);
 
     /*!
      * \brief Constructor using default SPI settings
      * \param bus_number SPI bus number
      * \param channel_number SPI channel number
      */
     Spi_interface(uint8_t bus_number, uint8_t channel_number);
 
     /*!
      * \brief Destructor (closes device if open)
      */
     ~Spi_interface();
 
     /*!
      * \brief Enable (open) the SPI device
      * \return 0 on success, -1 on failure
      */
     int enable();
 
     /*!
      * \brief Disable (close) the SPI device
      * \return 0 on success, -1 on failure
      */
     int disable();
 
     /*!
      * \brief Write data to the SPI bus
      * \param tx_buffer Pointer to transmit buffer
      * \param tx_length Number of bytes to write
      * \return 0 on success, -1 on failure
      */
     int write(const uint8_t* tx_buffer, uint32_t tx_length);
 
     /*!
      * \brief Simultaneously transmit and receive data over SPI
      * \param tx_buffer Transmit buffer
      * \param rx_buffer Receive buffer
      * \param length Number of bytes to transfer
      * \return 0 on success, -1 on failure
      */
     int transfer(const uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t length);
 
     /*!
      * \brief Write then immediately read from SPI in two separate steps
      * \param tx_buffer Data to write
      * \param tx_length Length of data to write
      * \param rx_buffer Buffer to read into
      * \param rx_length Length of read
      * \return 0 on success, -1 on failure
      */
     int write_then_read(const uint8_t* tx_buffer, uint32_t tx_length,
                         uint8_t* rx_buffer, uint32_t rx_length);
 
 private:
     void initialise(uint8_t bus_number, uint8_t channel_number,
                     uint8_t mode, uint32_t speed, uint8_t bits_per_word);
 
     char device_name[64];
     uint8_t spi_mode;
     uint32_t spi_speed;
     uint8_t spi_bits_per_word;
     int device_fd;
 
     struct spi_ioc_transfer transfer_structs[2];
 };
 
 #endif // SPI_IF_H
 