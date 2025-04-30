/*!
 * \file spiIf.cpp
 * \brief SPI Interface implementation for Linux spidev with safe embedded design
 */

 #include "spiIf.h"

 #include <fcntl.h>
 #include <unistd.h>
 #include <sys/ioctl.h>
 #include <cstring>
 #include <cstdio>
 
 Spi_interface::Spi_interface(uint8_t bus_number, uint8_t channel_number,
                              uint8_t mode, uint32_t speed, uint8_t bits_per_word)
 {
     initialise(bus_number, channel_number, mode, speed, bits_per_word);
 }
 
 Spi_interface::Spi_interface(uint8_t bus_number, uint8_t channel_number)
 {
     initialise(bus_number, channel_number, default_mode, default_speed, default_bits_per_word);
 }
 
 Spi_interface::~Spi_interface()
 {
     disable();
 }
 
 void Spi_interface::initialise(uint8_t bus_number, uint8_t channel_number,
                                uint8_t mode, uint32_t speed, uint8_t bits_per_word)
 {
     spi_mode = mode;
     spi_speed = speed;
     spi_bits_per_word = bits_per_word;
 
     snprintf(device_name, sizeof(device_name), "/dev/spidev%u.%u", bus_number, channel_number);
     device_fd = -1;
 
     std::memset(&transfer_structs, 0, sizeof(transfer_structs));
 }
 
 /*!
  * \brief Opens and configures the SPI device.
  * \return 0 on success, -1 on failure
  */
 int Spi_interface::enable()
 {
     device_fd = open(device_name, O_RDWR);
     if (device_fd < 0) return -1;
 
     if (ioctl(device_fd, SPI_IOC_WR_MODE, &spi_mode) < 0) return -1;
     if (ioctl(device_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word) < 0) return -1;
     if (ioctl(device_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0) return -1;
 
     return 0;
 }
 
 /*!
  * \brief Closes the SPI device if open.
  * \return 0 (always succeeds)
  */
 int Spi_interface::disable()
 {
     if (device_fd >= 0) {
         close(device_fd);
         device_fd = -1;
     }
     return 0;
 }
 
 /*!
  * \brief Writes raw data over SPI.
  * \param tx_buffer Pointer to buffer to send
  * \param tx_length Length of buffer
  * \return 0 on success, -1 on failure
  */
 int Spi_interface::write(const uint8_t* tx_buffer, uint32_t tx_length)
 {
     if (device_fd < 0 || tx_buffer == nullptr || tx_length == 0) return -1;
 
     struct spi_ioc_transfer xfer{};
     xfer.tx_buf = reinterpret_cast<__u64>(tx_buffer);
     xfer.rx_buf = 0;
     xfer.len = tx_length;
     xfer.speed_hz = spi_speed;
     xfer.bits_per_word = spi_bits_per_word;
 
     return (ioctl(device_fd, SPI_IOC_MESSAGE(1), &xfer) >= 0) ? 0 : -1;
 }
 
 /*!
  * \brief Simultaneously send and receive data over SPI.
  * \param tx_buffer Pointer to transmit buffer
  * \param rx_buffer Pointer to receive buffer
  * \param length Number of bytes to transfer
  * \return 0 on success, -1 on failure
  */
 int Spi_interface::transfer(const uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t length)
 {
     if (device_fd < 0 || tx_buffer == nullptr || rx_buffer == nullptr || length == 0) return -1;
 
     struct spi_ioc_transfer xfer{};
     xfer.tx_buf = reinterpret_cast<__u64>(tx_buffer);
     xfer.rx_buf = reinterpret_cast<__u64>(rx_buffer);
     xfer.len = length;
     xfer.speed_hz = spi_speed;
     xfer.bits_per_word = spi_bits_per_word;
 
     return (ioctl(device_fd, SPI_IOC_MESSAGE(1), &xfer) >= 0) ? 0 : -1;
 }
 
 /*!
  * \brief Perform a two-step write-then-read SPI transaction.
  * \param tx_buffer Pointer to buffer to write
  * \param tx_length Number of bytes to write
  * \param rx_buffer Pointer to buffer to read into
  * \param rx_length Number of bytes to read
  * \return 0 on success, -1 on failure
  */
 int Spi_interface::write_then_read(const uint8_t* tx_buffer, uint32_t tx_length,
                                    uint8_t* rx_buffer, uint32_t rx_length)
 {
     if (device_fd < 0 || tx_buffer == nullptr || rx_buffer == nullptr ||
         tx_length == 0 || rx_length == 0) return -1;
 
     struct spi_ioc_transfer xfers[2]{};
 
     xfers[0].tx_buf = reinterpret_cast<__u64>(tx_buffer);
     xfers[0].rx_buf = 0;
     xfers[0].len = tx_length;
     xfers[0].speed_hz = spi_speed;
     xfers[0].bits_per_word = spi_bits_per_word;
 
     xfers[1].tx_buf = 0;
     xfers[1].rx_buf = reinterpret_cast<__u64>(rx_buffer);
     xfers[1].len = rx_length;
     xfers[1].speed_hz = spi_speed;
     xfers[1].bits_per_word = spi_bits_per_word;
 
     return (ioctl(device_fd, SPI_IOC_MESSAGE(2), xfers) >= 0) ? 0 : -1;
 }
 