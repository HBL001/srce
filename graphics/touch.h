/*!
 * \file touch_buttons.h
 * \brief GUI Touch Buttons interface header file
 */

 #include <ft800.h>
 #ifndef TOUCH_BUTTONS_H
 #define TOUCH_BUTTONS_H
 
 class Touch_buttons
 {
 public:
     static const uint8_t no_tag = 0;
     static const uint8_t button_1_tag = 1;
     static const uint8_t button_2_tag = 2;
     static const uint8_t button_3_tag = 3;
     static const uint8_t button_4_tag = 4;
     static const uint8_t button_5_tag = 5;
     static const uint8_t button_pid_tag = 6;
     static const uint8_t button_hcp_tag = 7;
     static const uint8_t button_info_1_tag = 8;
     static const uint8_t button_info_2_tag = 9;
     static const uint8_t button_info_3_tag = 10;
     static const uint8_t button_info_4_tag = 11;
     static const uint8_t button_minus_tag = '-';
     static const uint8_t button_plus_tag = '+';
     static const uint8_t button_released_tag = 254;
     static const uint8_t untagged_icon_tag = 255;
 
     Touch_buttons (Ft800 &ft800);
 
     ~Touch_buttons (void);
 
     uint8_t poll_touch_buttons (void);
 
     bool button_pressed (uint8_t tag);
     uint8_t active_button (void);
 
     void discard_active_button (void);
 
 private:
 
     typedef enum
     {
         NO_BUTTON,
         DEBOUNCE,
         BUTTON_DETECTED,
         DISCARD_BUTTON
     } button_state_t;
 
     Ft800& ft800;
 
     button_state_t button_state;
 
     uint8_t last_touch_tag;
     bool touch_tag_detected;
 };
 
 #endif
 