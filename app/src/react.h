/**
 * @file react.h
 * @brief Header file for LED reaction class that handles GPIO and Zbus interactions
 */

 #pragma once

 #include <zephyr/kernel.h>
 #include <zephyr/zbus/zbus.h>
 #include <zephyr/drivers/gpio.h>
 #include "read.h"
 
 /**
  * @class ReactClass
  * @brief Class for managing LED reactions to button state changes via Zbus
  * 
  * This class handles LED control including blinking patterns and timed on/off states
  * in response to button state changes received through Zbus channels.
  */
 class ReactClass 
 {
 public:
     /**
      * @brief Construct a new ReactClass object
      * @param led_spec GPIO device tree specification for the LED
      */
     ReactClass(const struct gpio_dt_spec &led_spec);
 
     /**
      * @brief Initialize the LED GPIO and work queue
      * @return int 0 on success, negative error code on failure
      */
     int init();
 
     /**
      * @brief Zbus observer callback for button state changes
      * @param chan Pointer to the zbus channel that triggered the callback
      */
     static void observer_cb(const struct zbus_channel *chan);
 
     /**
      * @brief Work queue handler for LED control
      * @param work Pointer to the work structure
      */
     static void led_work_handler(struct k_work *work);
 
 private:
     static struct gpio_dt_spec led_;          ///< GPIO specification for the LED
     static struct k_work_delayable led_work_; ///< Delayed work item for LED control
     static uint8_t blink_remaining_;          ///< Counter for remaining blinks
 
     /**
      * @enum led_action
      * @brief Possible LED actions
      */
     static enum led_action
     {
         NONE,     ///< No current LED action
         BLINK,    ///< LED should blink
         ON_500MS, ///< LED should stay on for 500ms
         OFF       ///< LED should turn off
     } current_action_; ///< Current LED action state
 };