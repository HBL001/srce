/*!
 * \file smart_battery_if.cpp
 * \brief Smart Battery SMBus Interface implementation
 */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "batteryIf.h"

#ifdef SMART_BATTERY_IF_DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x)
#endif

batteryIf::batteryIf (uint8_t bus_number) : bus_number (bus_number)
{
    device_file = invalid_device_file;
}

batteryIf::~batteryIf (void)
{
    terminate ();
}

bool batteryIf::initialise (void)
{
    bool result = false;

    char device_filename[FILENAME_MAX];
    
    snprintf (device_filename, sizeof (device_filename), "/dev/i2c-%d", bus_number);

    device_file = open (device_filename, O_RDWR);

    if (0 > device_file)
    {
        DEBUG_PRINT (("Failed to open I2C-2 Bus"));
    }
    else if (0 > ioctl (device_file, I2C_SLAVE, smart_battery_address))
    {
        DEBUG_PRINT (("ioctl of smart battery I2C_SLAVE address 0x%02X failed...", smart_battery_address));
        terminate ();
    }
    else
    {
        result = true;
    }

    return result;
}

bool batteryIf::write_word (uint8_t command, uint16_t data)
{
    bool result = false;

    if (invalid_device_file != device_file)
    {
        uint8_t retry_count = 0;

        while (!result && (max_retry_count > retry_count))
        {
            int32_t write_result = i2c_smbus_write_word_data (device_file, command, data);
            if (0 > write_result)
            {
                retry_count++;
            }
            else
            {
                result = true;
            }
        }
    }

    return result;
}

bool batteryIf::read_word (uint8_t command, uint16_t* word)
{
    bool result = false;

    if ((invalid_device_file != device_file) &&
        (NULL != word))
    {
        uint8_t retry_count = 0;

        while (!result && (max_retry_count > retry_count))
        {
            int32_t read_result = i2c_smbus_read_word_data (device_file, command);
            if (0 > read_result)
            {
                retry_count++;
            }
            else
            {
                *word = (uint16_t)(read_result & 0xFFFF);

                if (100 < *word)
                {
                    DEBUG_PRINT (("i2c_smbus_read_word_data returned %08X (retry count %hhu)\n", read_result, retry_count));
                }
                else if (0 != retry_count)
                {
                    DEBUG_PRINT (("Retry count %hhu\n", retry_count));
                }
                result = true;
            }
        }
    }

    return result;
}

void batteryIf::terminate (void)
{
    if (invalid_device_file != device_file)
    {
        ::close (device_file);
        device_file = invalid_device_file;
    }
}


#ifdef SMART_BATTERY_IF_TESTS

int main (void)
{
    batteryIf battery (1);

    if (battery.initialise ())
    {
        printf ("Capacity: %humAh\n", battery.read_word (batteryIf::design_capacity_command));
        printf ("Voltage: %04huV\n", battery.read_word (batteryIf::design_voltage_command));
        printf ("Specification: %04X\n", battery.read_word (batteryIf::specification_command));
        printf ("Relative charge: %hu%%\n", battery.read_word (batteryIf::relative_charge_state_command));
        printf ("Absolute charge: %hu%%\n", battery.read_word (batteryIf::absolute_charge_state_command));
        printf ("Current: %humA\n", battery.read_word (batteryIf::current_command));
        printf ("Average current: %humA\n", battery.read_word (batteryIf::average_current_command));
        printf ("Battery State: %04X\n", battery.read_word (batteryIf::battery_status_command));

        printf ("\n\n");

        int16_t discharge_rate = -100;
        battery.write_word (batteryIf::at_rate_command, *((uint16_t*)&discharge_rate));
        sleep (1);

        uint16_t at_rate = battery.read_word (batteryIf::at_rate_command);
        printf ("At rate: %hdmAh\n", *((int16_t*)&at_rate));
        printf ("Time to empty: %hu minutes\n", battery.read_word (batteryIf::time_to_empty_command));

        discharge_rate = -6000;
        battery.write_word (batteryIf::at_rate_command, *((uint16_t*)&discharge_rate));
        sleep (1);
        at_rate = battery.read_word (batteryIf::at_rate_command);
        printf ("At rate: %hdmAh\n", *((int16_t*)&at_rate));
        printf ("Time to empty: %hu minutes\n", battery.read_word (batteryIf::time_to_empty_command));
    }
    else
    {
        printf ("Error initilising smart battery interface\n");
    }

    battery.terminate ();
}

#endif
