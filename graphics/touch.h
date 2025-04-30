/*!
 * \file touch.h
 * \brief GUI Touch Buttons interface for FT800 interaction
 */

 #ifndef TOUCH_BUTTONS_H
 #define TOUCH_BUTTONS_H
 
 #include "ft800.h"
 #include <cstddef>

 /*!
  * \class Touch_buttons
  * \brief Manages touch button input using FT800 touch tag register
  */
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
 
     /*!
      * \brief Constructor
      * \param ft800 Reference to initialized Ft800 display instance
      */
     Touch_buttons(Ft800& ft800);
 
     ~Touch_buttons();
 
     /*!
      * \brief Poll for any pressed or released button tag
      * \return Tag of detected button or 254 for release, 0 for none
      */
     uint8_t poll_touch_buttons();
 
     /*!
      * \brief Check if a specific tag is currently pressed
      * \param tag Tag value to query
      * \return true if that tag is actively pressed
      */
     bool button_pressed(uint8_t tag);
 
     /*!
      * \brief Get the tag of the current active button
      * \return Tag or no_tag if none is active
      */
     uint8_t active_button();
 
     /*!
      * \brief Discard any current active button without reporting it
      */
     void discard_active_button();
 
 private:
     /*!
      * \brief Finite state machine for debouncing and holding touch buttons
      */
     enum button_state_t
     {
         NO_BUTTON,
         DEBOUNCE,
         BUTTON_DETECTED,
         DISCARD_BUTTON
     };
 
     Ft800& ft800;
     button_state_t button_state;
     uint8_t last_touch_tag;
 };
 
 #endif // TOUCH_BUTTONS_H
 