#ifndef MOCK_READ_H
#define MOCK_READ_H

#include "../mock_zephyr/zephyr/device.h"
#include "../mock_zephyr/zephyr/drivers/gpio.h"

class ReadClass
{
public:
    ReadClass(gpio_dt_spec button_spec);
    void init();
    static void isr_handler(const struct device* dev, struct gpio_callback* cb, uint32_t pins);

private:
    gpio_dt_spec button;
};

#endif // MOCK_READ_H

extern int mock_led_state;
