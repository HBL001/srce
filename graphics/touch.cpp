/*!
 * \file touch_buttons.cpp
 * \brief GUI Touch Buttons implementation
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "touch.h"

Touch_buttons::Touch_buttons (Ft800 &ft800): ft800 (ft800)
{
    button_state = NO_BUTTON;
    last_touch_tag = no_tag;
}

Touch_buttons::~Touch_buttons (void)
{
}

uint8_t Touch_buttons::poll_touch_buttons (void)
{
    uint8_t result = no_tag;

    uint8_t tag_value = no_tag;

    ft800.get_touch_tag (&tag_value);

    switch (button_state)
    {
        case NO_BUTTON:
        {
            if ((no_tag != tag_value) &&
                (untagged_icon_tag != tag_value))
            {
                // Move on to debounce state
                button_state = DEBOUNCE;
            }
        }
        break;

        case DEBOUNCE:
        {
            if ((no_tag != tag_value) &&
                (untagged_icon_tag != tag_value))
            {
                if (last_touch_tag == tag_value)
                {
                    // A debounced tag - report
                    result = tag_value;
                    ft800.key_click ();

                    // Move on to detected state
                    button_state = BUTTON_DETECTED;
                }
                else
                {
                    // A new button - debounce again
                    button_state = DEBOUNCE;
                }
            }
            else
            {
                // No button
                button_state = NO_BUTTON;
            }
        }
        break;

        case BUTTON_DETECTED:
        {
            if ((no_tag != tag_value) &&
                (untagged_icon_tag != tag_value))
            {
                if (last_touch_tag == tag_value)
                {
                    // Still the same button - do nothing
                }
                else
                {
                    // A button roll-over - report release of last button
                    result = button_released_tag;

                    // Debounce the new button
                    button_state = DEBOUNCE;
                }
            }
            else
            {
                // The button press has ended - report release
                result = button_released_tag;

                // Return to NO_BUTTON state
                button_state = NO_BUTTON;
            }
        }
        break;

        case DISCARD_BUTTON:
        {
            if ((no_tag != tag_value) &&
                (untagged_icon_tag != tag_value))
            {
                // A button is still pressed - ignore
            }
            else
            {
                // No button - return to NO_BUTTON state
                button_state = NO_BUTTON;
            }
        }
        break;

        default:
        {
            button_state = NO_BUTTON;
        }
    }

    // Record the latest value for next poll
    last_touch_tag = tag_value;

    return result;
}

bool Touch_buttons::button_pressed (uint8_t tag)
{
    bool result = false;

    if ((BUTTON_DETECTED == button_state) &&
        (last_touch_tag == tag))
    {
        result = true;
    }

    return result;
}

uint8_t Touch_buttons::active_button (void)
{
    uint8_t result = no_tag;

    if (BUTTON_DETECTED == button_state)
    {
        result = last_touch_tag;
    }

    return result;
}


void Touch_buttons::discard_active_button (void)
{
    if (NO_BUTTON != button_state)
    {
        button_state = DISCARD_BUTTON;
    }
}
