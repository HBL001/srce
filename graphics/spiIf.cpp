#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spiIf.h"

Spi_interface::Spi_interface (uint8_t bus_number, uint8_t channel_number, uint8_t mode, uint32_t speed, uint8_t bits_per_word)
{
    initialise (bus_number, channel_number, mode, speed, bits_per_word);
}

Spi_interface::Spi_interface (uint8_t bus_number, uint8_t channel_number)
{
    initialise (bus_number, channel_number, default_mode, default_speed, default_bits_per_word);
}

Spi_interface::~Spi_interface (void)
{
    disable ();
}

bool Spi_interface::enable (void)
{
    bool result = false;

    disable ();

    device_fd = open (device_name, O_RDWR);

    if (-1 != device_fd)
    {
        if ((-1 != ioctl (device_fd, SPI_IOC_WR_MODE, &spi_mode)) &&
            (-1 != ioctl (device_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word)) &&
            (-1 != ioctl (device_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed)))
        {
            result = true;
        }
        else
        {
            disable ();
        }
    }

    return result;
}

bool Spi_interface::disable (void)
{
    bool result = true;

    if (-1 != device_fd)
    {
        close (device_fd);
        device_fd = -1;
    }

    return result;
}

bool Spi_interface::write (const uint8_t* tx_buffer, uint32_t tx_length)
{
    bool result = false;

    if ((-1 != device_fd) &&
        (NULL != tx_buffer) &&
        (0 != tx_length))
    {
        transfer_structs[0].tx_buf = (unsigned long)tx_buffer;
        transfer_structs[0].rx_buf = (unsigned long)NULL;
        transfer_structs[0].len = tx_length;
        if (-1 != ioctl (device_fd, SPI_IOC_MESSAGE(1), transfer_structs))
        {
            result = true;
        }
    }
    
    return result;
}

bool Spi_interface::transfer (const uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t tx_rx_length)
{
    bool result = false;

    if ((-1 != device_fd) &&
        (NULL != tx_buffer) &&
        (NULL != rx_buffer) &&
        (0 != tx_rx_length))
    {
        transfer_structs[0].tx_buf = (unsigned long)tx_buffer;
        transfer_structs[0].rx_buf = (unsigned long)rx_buffer;
        transfer_structs[0].len = tx_rx_length;
        if (-1 != ioctl (device_fd, SPI_IOC_MESSAGE(1), transfer_structs))
        {
            result = true;
        }
    }
    
    return result;
}

bool Spi_interface::write_then_read (const uint8_t* tx_buffer, uint32_t tx_length, uint8_t* rx_buffer, uint32_t rx_length)
{
    bool result = false;

    if ((-1 != device_fd) &&
        (NULL != tx_buffer) &&
        (0 != tx_length) &&
        (NULL != rx_buffer) &&
        (0 != rx_length))
    {
        transfer_structs[0].tx_buf = (unsigned long)tx_buffer;
        transfer_structs[0].rx_buf = (unsigned long)NULL;
        transfer_structs[0].len = tx_length;
        transfer_structs[1].tx_buf = (unsigned long)NULL;
        transfer_structs[1].rx_buf = (unsigned long)rx_buffer;
        transfer_structs[1].len = rx_length;
        if (-1 != ioctl (device_fd, SPI_IOC_MESSAGE(2), transfer_structs))
        {
            result = true;
        }
    }
    
    return result;
}

void Spi_interface::initialise (uint8_t bus_number, uint8_t channel_number, uint8_t mode, uint32_t speed, uint8_t bits_per_word)
{
    spi_mode = mode;
    spi_speed = speed;
    spi_bits_per_word = bits_per_word;

    // Set default 
    memset (&transfer_structs, 0, sizeof (transfer_structs));
    transfer_structs[0].delay_usecs = 0;
    transfer_structs[0].speed_hz = spi_speed;
    transfer_structs[0].bits_per_word = spi_bits_per_word;
    transfer_structs[1].delay_usecs = 0;
    transfer_structs[1].speed_hz = spi_speed;
    transfer_structs[1].bits_per_word = spi_bits_per_word;

    snprintf (device_name, sizeof (device_name), "/dev/spidev%hhu.%hhu", bus_number, channel_number);
    device_fd = -1;
}

#ifdef PERFORM_SPI_TESTS
int main (void)
{
    int ret = 0;

    Spi_interface spi_interface (1, 0);

    if (spi_interface.enable ())
    {
        uint8_t tx[] = {
            0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
            0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD, 0xF0, 0x0D
        };
        uint8_t rx[sizeof (tx)] = {0, };

        if (spi_interface.transfer (tx, rx, sizeof (rx)))
        {
            for (size_t count = 0; count < sizeof (rx); count++)
            {
                if (7 == (count % 8))
                {
                    printf ("0x%02hhX\n", rx[count]);
                }
                else
                {
                    printf ("0x%02hhX ", rx[count]);
                }
            }
        }
        else
        {
            printf ("SPI transfer failed\n");
        }

        uint8_t wr_buf[] = {'w', 'r', 'i', 't', 'e', ' ', 'b', 'u', 'f'};
        if (spi_interface.write (wr_buf, sizeof (wr_buf)))
        {
            printf ("SPI Write passed\n");
        }
        else
        {
            printf ("SPI Write failed\n");
        }

        uint8_t rd_buf[] = {'r', 'e', 'a', 'd', ' ', 'b', 'u', 'f'};
        if (spi_interface.write_then_read (wr_buf, sizeof (wr_buf), rd_buf, sizeof (rd_buf)))
        {
            for (size_t count = 0; count < sizeof (rd_buf); count++)
            {
                printf ("0x%02hhX ", rd_buf[count]);
            }
            printf ("\n");
        }
        else
        {
            printf ("SPI Write-then-Read failed\n");
        }
    }
    else
    {
        printf ("Failed to enable SPI interface\n");
    }

    return ret;
}
#endif
