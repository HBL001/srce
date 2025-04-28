/*!
 * \file battery.cpp
 * \brief Battery Control Interface implementation
 */
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "definitions.h"
#include "battery.h"

#ifdef BATTERY_DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x)
#endif


Battery::Battery (uint8_t bus_number) : battery_interface (bus_number)
{
    thread_id = -1;
    thread_running = false;

    charge_state = unknown_battery_charge;
    discharge_threshold = default_discharge_threshold;
}

Battery::~Battery (void)
{
    if (thread_running)
    {
        thread_running = false;
        pthread_join (thread_id, NULL);
    }
    pthread_mutex_destroy (&charge_state_mutex);
    battery_interface.terminate ();
}

bool Battery::initialise (uint8_t minimum_battery_threshold, Logger& parent_log)
{
    bool result = false;

    discharge_threshold = minimum_battery_threshold;

    if (battery_interface.initialise ())
    {
        // Test communication with the battery
        uint16_t word;
        if (battery_interface.read_word (Smart_battery_interface::specification_command, &word))
        {
            int create_result = pthread_mutex_init (&charge_state_mutex, NULL);

            if (0 == create_result)
            {
                create_result = pthread_create (&thread_id, NULL, entry_routine, this);

                if (0 == create_result)
                {
                    result = true;
                }
                else
                {
                    parent_log.critical ("Failed to create Battery thread - pthread_create returned %d\n", create_result);
                }
            }
            else
            {
                parent_log.critical ("Failed to initialise Battery mutex - pthread_mutex_init returned %d\n", create_result);
            }
        }
        else
        {
            parent_log.critical ("Battery communications failed\n");
        }
    }
    else
    {
        parent_log.critical ("Failed to initialise Battery interface\n");
    }

    return result;
}

uint8_t Battery::charge_level_percent (void)
{
    uint8_t result = unknown_battery_charge;

    if (0 == pthread_mutex_trylock (&charge_state_mutex))
    {
        result = charge_state;
        pthread_mutex_unlock (&charge_state_mutex);
    }

    return result;
}

bool Battery::discharged (void)
{
    bool result = true;

    uint8_t charge = charge_level_percent ();

    if ((unknown_battery_charge == charge) ||
        (discharge_threshold < charge))
    {
        result = false;
    }
    else
    {
        DEBUG_PRINT (("battery discharged (%hhu)\n", charge));
    }

    return result;
}

uint8_t Battery::set_discharge_threshold (uint8_t new_threshold)
{
    uint8_t result = new_threshold;

    if (100 < new_threshold)
    {
        result = 100;
    }

    discharge_threshold = result;
    
    return result;
}

uint8_t Battery::reset_discharge_threshold (void)
{
    uint8_t result = default_discharge_threshold;

    discharge_threshold = result;
    
    return result;
}

void* Battery::entry_routine (void *arg)
{
    if (NULL != arg)
    {
        Battery* this_instance = reinterpret_cast<Battery*>(arg);

        this_instance->main_loop ();
    }
    else
    {
        DEBUG_PRINT (("Battery::entry_routine called with NULL argument\n"));
    }

    return NULL;
}

void Battery::main_loop (void)
{
    thread_running = true;

    DEBUG_PRINT (("Battery thread in main loop\n"));

    while (thread_running)
    {
        uint8_t new_reading = read_charge_level ();
        if (charge_state != new_reading)
        {
            if (0 == pthread_mutex_lock (&charge_state_mutex))
            {
                charge_state = new_reading;
                pthread_mutex_unlock (&charge_state_mutex);
            }
        }
        // Only update every 10 seconds
        sleep (10);
    }
}

uint8_t Battery::read_charge_level (void)
{
    uint8_t result = unknown_battery_charge;

    uint16_t word;
    if (battery_interface.read_word (Smart_battery_interface::relative_charge_state_command, &word))
    {
        if (100 >= word)
        {
            result = (uint8_t)word & 0xFF;
        }
        else
        {
            DEBUG_PRINT (("battery_interface.read_word returned %u\n", word));
        }
    }
    else
    {
        DEBUG_PRINT (("battery_interface.read_word FAILED!\n"));
    }

    return result;
}

