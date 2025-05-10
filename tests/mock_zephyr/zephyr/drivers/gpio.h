#ifndef MOCK_ZEPHYR_GPIO_H
#define MOCK_ZEPHYR_GPIO_H

#include <cstdint>

// ===== Mock Zephyr GPIO types =====

using gpio_flags_t = int;
using gpio_callback_handler_t = void(*)(const struct device*, struct gpio_callback*, uint32_t);
using gpio_port_pins_t = uint32_t;

struct gpio_dt_spec
{
    void* port;
    int pin;
};

struct gpio_callback {};

#endif // MOCK_ZEPHYR_GPIO_H
