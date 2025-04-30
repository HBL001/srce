/*!
 * \file gui_components.h
 * \brief General-purpose GUI components for FT800 applications
 */

 #ifndef GUI_COMPONENTS_H
 #define GUI_COMPONENTS_H
 
 #include "ft800.h"
 #include "touch.h"
 #include "device_definitions.h"
 #include <cstdint>
 #include <ctime>
 
 /*!
  * \class Gui_components
  * \brief High-level GUI element rendering using FT800.
  */
 class Gui_components
 {
 public:
     static constexpr uint8_t invalid_bitmap_handle = 15;
 
     Gui_components(Ft800& ft800, Touch_buttons& touch_buttons);
     ~Gui_components();
 
     // Core UI
     int start_touch_calibration();
     int load_standard_icons();
     int screen_template(time_t current_time = 0);
     int screen_complete();
     int screen_complete_with_wait_icon();
 
     // Icon Drawing
     int draw_battery_icon(Device_definitions::battery_state_t battery_level, uint16_t x, uint16_t y);
     int draw_memory_icon(bool low_memory, uint16_t x, uint16_t y);
     int display_usb_icon();
     int display_printer_icon();
     int display_warning_triangle();
     int display_heater_icon();
 
     // Informational Overlays
     int display_text_main(const char* text, bool error = false);
     int display_text_secondary(const char* text, bool error = false);
     int display_run_status(const char* status);
     int display_run_time(uint32_t elapsed_seconds, uint32_t scheduled_seconds);
     int display_run_time_bottom(uint32_t elapsed_seconds, uint32_t scheduled_seconds);
     int display_initial_viscosity(double viscosity);
     int display_heater_temperatures(double target_temp, double current_temp);
 
     // Controls
     void display_button(uint8_t tag, const char* label, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font = 28);
     void enable_fullscreen_button(uint8_t tag);
     void set_button_colours(uint8_t tag);
 
     // Color Schemes
     void set_colour_black();
     void set_colour_red();
     void set_colour_blue();
     void set_colour_dark_grey();
     void set_colour_mid_grey();
     void set_colour_pale_grey();
     void set_colour_white();
     void set_colour_green();
     void set_colour_heater();
 
     void foreground_white();
     void foreground_red();
     void foreground_dark_grey();
     void foreground_mid_grey();
     void background_red();
     void bitmap_background_white();
     void bitmap_background_mid_grey();
 
 private:
     Ft800& ft800;
     Touch_buttons& touch_buttons;
 
     static constexpr uint8_t battery_icon_handle = 0;
     static constexpr uint8_t memory_icon_handle = 1;
     static constexpr uint8_t heater_icon_handle = 2;
 };
 
 #endif // GUI_COMPONENTS_H
 