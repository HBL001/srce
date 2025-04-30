/*!
 * \file touch.cpp
 * \brief FT800 GUI Touch Button handler implementation
 */

 #include "touch.h"

 Touch_buttons::Touch_buttons(Ft800& ft800) : ft800(ft800)
 {
     button_state = NO_BUTTON;
     last_touch_tag = no_tag;
 }
 
 Touch_buttons::~Touch_buttons() {}
 
 uint8_t Touch_buttons::poll_touch_buttons()
 {
     uint8_t result = no_tag;
     uint8_t tag_value = no_tag;
 
     ft800.get_touch_tag(&tag_value);
 
     switch (button_state)
     {
     case NO_BUTTON:
         if (tag_value != no_tag && tag_value != untagged_icon_tag)
         {
             button_state = DEBOUNCE;
         }
         break;
 
     case DEBOUNCE:
         if (tag_value != no_tag && tag_value != untagged_icon_tag)
         {
             if (last_touch_tag == tag_value)
             {
                 result = tag_value;
                 ft800.key_click();
                 button_state = BUTTON_DETECTED;
             }
         }
         else
         {
             button_state = NO_BUTTON;
         }
         break;
 
     case BUTTON_DETECTED:
         if (tag_value != no_tag && tag_value != untagged_icon_tag)
         {
             if (last_touch_tag != tag_value)
             {
                 result = button_released_tag;
                 button_state = DEBOUNCE;
             }
         }
         else
         {
             result = button_released_tag;
             button_state = NO_BUTTON;
         }
         break;
 
     case DISCARD_BUTTON:
         if (tag_value == no_tag || tag_value == untagged_icon_tag)
         {
             button_state = NO_BUTTON;
         }
         break;
 
     default:
         button_state = NO_BUTTON;
         break;
     }
 
     last_touch_tag = tag_value;
     return result;
 }
 
 bool Touch_buttons::button_pressed(uint8_t tag)
 {
     return (button_state == BUTTON_DETECTED && last_touch_tag == tag);
 }
 
 uint8_t Touch_buttons::active_button()
 {
     return (button_state == BUTTON_DETECTED) ? last_touch_tag : no_tag;
 }
 
 void Touch_buttons::discard_active_button()
 {
     if (button_state != NO_BUTTON)
     {
         button_state = DISCARD_BUTTON;
     }
 }
 