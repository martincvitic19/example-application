/**
 * @file     read.cpp
 * @brief    Button input handler implementation
 * @author   Martin Cvitic
 * @date     2025-05-10
 * @version  1.0
 *
 * @details  Implements button input handling including:
 *           - GPIO interrupt configuration
 *           - Debounce logic
 *           - Zbus message publishing
 */

//=============================================================================
// INCLUDES
//=============================================================================
#include "read.h"
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

//=============================================================================
// MACROS
//=============================================================================
LOG_MODULE_REGISTER(ReadClass, LOG_LEVEL_INF);  ///< ReadClass logger instance

ZBUS_CHAN_DEFINE(
    button_chan,
    struct button_msg,
    NULL, NULL,
    ZBUS_OBSERVERS(led_observer),
    ZBUS_MSG_INIT(.pressed = false, .timestamp = 0)
);

//=============================================================================
// STATIC MEMBER INITIALIZATION
//=============================================================================
struct gpio_callback ReadClass::button_cb_;
struct k_work ReadClass::work_;
int64_t ReadClass::last_interrupt_time_ = 0;
ReadClass* ReadClass::instance_ = nullptr;

//=============================================================================
// CLASS METHOD IMPLEMENTATIONS
//=============================================================================

/**
 * @brief Construct a new ReadClass object
 * @param button_spec GPIO specification for the button
 */
ReadClass::ReadClass(const struct gpio_dt_spec &button_spec)
    : button_(button_spec) {}

/**
 * @brief Initialize button hardware and interrupt
 * @return int 0 on success, negative error code on failure
 */
int ReadClass::init()
{
    instance_ = this;
    k_work_init(&work_, work_handler);

    if (!gpio_is_ready_dt(&button_))
    {
        LOG_ERR("Button device not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&button_, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0)
    {
        LOG_ERR("Failed to configure button: %d", ret);
        return ret;
    }

    ret = gpio_pin_interrupt_configure_dt(&button_, GPIO_INT_EDGE_BOTH);
    if (ret != 0)
    {
        LOG_ERR("Failed to configure interrupt: %d", ret);
        return ret;
    }

    gpio_init_callback(&button_cb_, isr_handler, BIT(button_.pin));
    gpio_add_callback(button_.port, &button_cb_);

    return 0;
}

/**
 * @brief Interrupt service routine for button state changes
 * @param dev Pointer to the device structure (unused)
 * @param cb Pointer to the callback structure (unused)
 * @param pins Bitmask of pins that triggered the interrupt (unused)
 */
void ReadClass::isr_handler(const struct device *dev, 
                           struct gpio_callback *cb, 
                           uint32_t pins)
{
    (void)dev;
    (void)cb;
    (void)pins;

    int64_t now = k_uptime_get();
    if ((now - last_interrupt_time_) > debounce_ms_)
    {
        k_work_submit(&instance_->work_);
        last_interrupt_time_ = now;
    }
}

/**
 * @brief Work queue handler for button state processing
 * @param work Pointer to the work structure (unused)
 */
void ReadClass::work_handler(struct k_work *work)
{
    (void)work;
    ReadClass *self = instance_;

    int val = gpio_pin_get_dt(&self->button_);
    if (val < 0)
    {
        LOG_ERR("Failed to read button");
        return;
    }

    bool pressed = (val == 0);
    button_msg msg = {
        .pressed = pressed,
        .timestamp = k_uptime_get_32()
    };

    zbus_chan_pub(&button_chan, &msg, K_NO_WAIT);
    LOG_INF("Button %s", pressed ? "PRESSED" : "RELEASED");
}