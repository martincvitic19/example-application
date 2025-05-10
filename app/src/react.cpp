/**
 * @file     react.cpp
 * @brief    LED reaction handler implementation
 * @author   Martin Cvitic
 * @date     2025-05-10
 * @version  1.0
 *
 * @details  Implements LED control logic including:
 *           - Zbus observer callback
 *           - LED blink patterns
 *           - Timed LED states
 */

//=============================================================================
// INCLUDES
//=============================================================================
#include "react.h"
#include "read.h"  // Include the header where button_msg is defined
#include <zephyr/logging/log.h>

//=============================================================================
// MACROS
//=============================================================================
LOG_MODULE_REGISTER(ReactClass, LOG_LEVEL_INF);  ///< ReactClass logger instance
ZBUS_LISTENER_DEFINE(led_observer, ReactClass::observer_cb);

//=============================================================================
// STATIC MEMBER INITIALIZATION
//=============================================================================
struct gpio_dt_spec ReactClass::led_;
struct k_work_delayable ReactClass::led_work_;
uint8_t ReactClass::blink_remaining_ = 0;
ReactClass::led_action ReactClass::current_action_ = ReactClass::NONE;


//=============================================================================
// CLASS METHOD IMPLEMENTATIONS
//=============================================================================

/**
 * @brief Construct a new ReactClass object
 * @param led_spec GPIO specification for the LED
 */
ReactClass::ReactClass(const struct gpio_dt_spec &led_spec)
{
    led_ = led_spec;
}

/**
 * @brief Initialize LED hardware and work queue
 * @return int 0 on success, negative error code on failure
 */
int ReactClass::init()
{
    k_work_init_delayable(&led_work_, led_work_handler);

    if (!gpio_is_ready_dt(&led_)) 
    {
        LOG_ERR("LED device not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&led_, GPIO_OUTPUT_INACTIVE);
    if (ret != 0)
    {
        LOG_ERR("Failed to configure LED: %d", ret);
        return ret;
    }

    return 0;
}

/**
 * @brief Zbus observer callback for button state changes
 * @param chan Pointer to the zbus channel that triggered the callback
 */
void ReactClass::observer_cb(const struct zbus_channel *chan)
{

    const button_msg *msg = static_cast<const button_msg *>(zbus_chan_const_msg(chan));

    k_work_cancel_delayable(&led_work_);

    if (msg->pressed) 
    {
        current_action_ = BLINK;
        blink_remaining_ = 6;  // 3 blinks (on/off pairs)
    } 
    else 
    {
        current_action_ = ON_500MS;
    }

    k_work_reschedule(&led_work_, K_NO_WAIT);
}

/**
 * @brief Work queue handler for LED state machine
 * @param work Pointer to the work structure (unused)
 */
void ReactClass::led_work_handler(struct k_work *work)
{
    (void)work;  // Explicitly unused parameter

    switch (current_action_)
    {
    case BLINK:
        if (blink_remaining_ > 0)
        {
            gpio_pin_toggle_dt(&led_);
            blink_remaining_--;
            k_work_reschedule(&led_work_, K_MSEC(100));
        } 
        else 
        {
            gpio_pin_set_dt(&led_, 0);
            current_action_ = NONE;
        }
        break;

    case ON_500MS:
        gpio_pin_set_dt(&led_, 1);
        current_action_ = OFF;
        k_work_reschedule(&led_work_, K_MSEC(500));
        break;

    case OFF:
        gpio_pin_set_dt(&led_, 0);
        current_action_ = NONE;
        break;

    case NONE:
    default:
        break;
    }
}