
/*!
 * File graph_touch.h
 * Brief GUI Touch Buttons interface for FT800 interaction
 */

 #ifndef GRAPH_TOUCH_BUTTONS_H
 #define GRAPH_TOUCH_BUTTONS_H
 
 #include "graph_ft800.h"
 #include <cstddef>
 
 /*!
  * \class Touch_buttons
  * Brief Manages touch button input using FT800 touch tag register
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
      * Brief Constructor
      * \param ft800 Reference to initialized GraphFt800 display instance
      */
     Touch_buttons(GraphFt800& ft800);
 
     ~Touch_buttons();
 
     /*!
      * Brief Poll for any pressed or released button tag
      * 
 eturn Tag of detected button or 254 for release, 0 for none
      */
     uint8_t poll_touch_buttons();
 
     /*!
      * Brief Check if a specific tag is currently pressed
      * \param tag Tag value to query
      * 
 eturn true if that tag is actively pressed
      */
     bool button_pressed(uint8_t tag);
 
     /*!
      * Brief Get the tag of the current active button
      * 
 eturn Tag or no_tag if none is active
      */
     uint8_t active_button();
 
     /*!
      * Brief Discard any current active button without reporting it
      */
     void discard_active_button();
 
 private:
     /*!
      * Brief Finite state machine for debouncing and holding touch buttons
      */
     enum button_state_t
     {
         NO_BUTTON,
         DEBOUNCE,
         BUTTON_DETECTED,
         DISCARD_BUTTON
     };
 
     GraphFt800& ft800;
     button_state_t button_state;
     uint8_t last_touch_tag;
 };
 
 #endif // GRAPH_TOUCH_BUTTONS_H
 