/**
 * @file read.h
 * @brief Header file for button reading class with interrupt and debounce handling
 */

 #pragma once

 #include <zephyr/device.h>
 #include <zephyr/drivers/gpio.h>
 #include <zephyr/kernel.h>
 #include <zephyr/sys/util.h>
 #include <cstdint>
 
 /**
  * @struct button_msg
  * @brief Message structure for button state changes
  */
 struct button_msg
 {
     bool pressed;        ///< Current button state (true = pressed)
     uint32_t timestamp;  ///< Timestamp when state change occurred
 };
 
 /**
  * @class ReadClass
  * @brief Class for managing button input with debouncing and interrupt handling
  *
  * This class handles GPIO button input with hardware interrupt triggering,
  * debouncing, and work queue processing for state change events.
  */
 class ReadClass
 {
 public:
     /**
      * @brief Construct a new ReadClass object
      * @param button_spec GPIO device tree specification for the button
      */
     ReadClass(const struct gpio_dt_spec &button_spec);
 
     /**
      * @brief Initialize the button GPIO and interrupt
      * @return int 0 on success, negative error code on failure
      */
     int init();
 
 #ifdef UNIT_TEST
     /**
      * @brief Interrupt service routine for button state changes
      * @param dev Pointer to the device structure
      * @param cb Pointer to the callback structure
      * @param pins Bitmask of pins that triggered the interrupt
      */
     static void isr_handler(const struct device *dev, 
                           struct gpio_callback *cb, 
                           uint32_t pins);
 
     /**
      * @brief Work queue handler for processing button state changes
      * @param work Pointer to the work structure
      */
     static void work_handler(struct k_work *work);
 #else
 private:
     static void isr_handler(const struct device *dev,
                           struct gpio_callback *cb,
                           uint32_t pins);
     static void work_handler(struct k_work *work);
 #endif
 
     struct gpio_dt_spec button_;  ///< GPIO specification for the button
 
 private:
     static struct gpio_callback button_cb_;  ///< GPIO callback structure
     static struct k_work work_;              ///< Work item for deferred processing
     static constexpr int64_t debounce_ms_ = 50;  ///< Debounce time in milliseconds
     static int64_t last_interrupt_time_;     ///< Timestamp of last interrupt
     static ReadClass* instance_;             ///< Singleton instance pointer
 };