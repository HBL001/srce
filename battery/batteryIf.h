/*!
 * \file smart_battery_if.h
 * \brief Smart Battery SMBus Interface header file
 */

#include <cstdint>

 #ifndef SMART_BATTERY_IF_H
 #define SMART_BATTERY_IF_H
 

 class batteryIf
 {
 public:
     batteryIf (uint8_t bus_number);
     ~batteryIf (void);
 
     bool initialise (void);
 
     bool write_word (uint8_t command, uint16_t data);
     bool read_word (uint8_t command, uint16_t* word);
 
     void terminate (void);
 
     static const uint8_t at_rate_command = 0x04;
     static const uint8_t time_to_empty_command = 0x06;
     static const uint8_t current_command = 0x0A;
     static const uint8_t average_current_command = 0x0B;
     static const uint8_t relative_charge_state_command = 0x0D;
     static const uint8_t absolute_charge_state_command = 0x0E;
     static const uint8_t battery_status_command = 0x16;
     static const uint8_t cycle_count_command = 0x17;
     static const uint8_t design_capacity_command = 0x18;
     static const uint8_t design_voltage_command = 0x19;
     static const uint8_t specification_command = 0x1A;
 
 private:
     static const uint8_t smart_battery_address = 0x0B;
 
     static const uint8_t max_retry_count = 5;
 
     const uint8_t bus_number;
 
     static const int invalid_device_file = -1;
     int device_file;
 };
 
 #endif
 
 