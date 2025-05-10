#include "mock_zephyr/zephyr/device.h"
#include "mock_zephyr/zephyr/drivers/gpio.h"
#include <gtest/gtest.h>
#include "mock_zephyr/zephyr/kernel.h"
#include "mock_read/mock_read.h"

// ==== Mock State ====

namespace {
    bool mock_button_state = false;
}

int mock_led_state = -1;

// ==== Mock Functions ====

int gpio_pin_get_dt(const gpio_dt_spec *)
{
    return mock_button_state ? 1 : 0;
}

int gpio_pin_set_dt(const gpio_dt_spec *, int value)
{
    mock_led_state = value;
    return 0;
}

int gpio_pin_configure_dt(const gpio_dt_spec *, gpio_flags_t) { return 0; }
int gpio_pin_interrupt_configure_dt(const gpio_dt_spec *, gpio_flags_t) { return 0; }
bool gpio_is_ready_dt(const gpio_dt_spec *) { return true; }

void gpio_init_callback(gpio_callback *, gpio_callback_handler_t, gpio_port_pins_t) {}
int gpio_add_callback(const device *, gpio_callback *) { return 0; }

void k_work_init(struct k_work *work, void (*handler)(struct k_work *))
{
    work->handler = handler;
}
int k_work_submit(struct k_work *work)
{
    work->handler(work);
    return 0;
}

int64_t fake_time = 0;


// ==== Tests ====

TEST(ReadTest_ButtonHighTriggersBlink, Test)
{
    gpio_dt_spec dummy_button = {nullptr, 0};
    ReadClass reader(dummy_button);
    reader.init();

    mock_button_state = true;

    fake_time = 100;
    ReadClass::isr_handler(nullptr, nullptr, 0);

    EXPECT_EQ(mock_led_state, 0);
}

TEST(ReadTest_ButtonLowTurnsOnThenOff, Test)
{
    gpio_dt_spec dummy_button = {nullptr, 0};
    ReadClass reader(dummy_button);
    reader.init();

    mock_button_state = false;

    fake_time = 200;
    ReadClass::isr_handler(nullptr, nullptr, 0);

    EXPECT_EQ(mock_led_state, 0);
}
