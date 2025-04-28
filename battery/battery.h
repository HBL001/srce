/*!
 * \file battery.h
 * \brief Battery Control Interface header file
 */

 #ifndef BATTERY_H
 #define BATTERY_H

 #include <cstdint>
 #include <pthread.h>
 
 #include "batteryIf.h"
 
 class Battery
 {
 public:
     static const uint8_t default_discharge_threshold = 10;
 
     Battery (uint8_t bus_number);
     ~Battery (void);
 
    uint8_t charge_level_percent (void);
 
     bool discharged (void);
 
     uint8_t get_discharge_threshold (void) { return discharge_threshold; };
     uint8_t set_discharge_threshold (uint8_t new_threshold);
     uint8_t reset_discharge_threshold (void);
 
 private:
 
     pthread_t thread_id;
     bool thread_running;
 
     pthread_mutex_t charge_state_mutex;
     uint8_t charge_state;
 
     uint8_t discharge_threshold;
 
     batteryIf battery_interface;
 
     static void* entry_routine (void *arg);
     void main_loop (void);
 
     uint8_t read_charge_level (void);
 };
 
 #endif
 
