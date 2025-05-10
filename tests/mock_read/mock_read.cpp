#include "mock_read.h"
#include <iostream>


ReadClass::ReadClass(gpio_dt_spec button_spec)
    : button(button_spec) {}

void ReadClass::init()
{
    std::cout << "[mock_read] init() called" << std::endl;
}

void ReadClass::isr_handler(const struct device*, struct gpio_callback*, uint32_t)
{
    std::cout << "[mock_read] isr_handler() called" << std::endl;
    mock_led_state = 0;
}
