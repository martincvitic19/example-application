# Zephyr Button-LED Interaction Demo Application

![Zephyr RTOS](https://img.shields.io/badge/Zephyr-RTOS-blue)
![MIT License](https://img.shields.io/badge/License-MIT-green)

A non-trivial button-LED application built on Zephyr RTOS featuring:
- Hardware-triggered interrupts with debouncing
- Zbus message passing between components
- Configurable LED blink patterns
- Thread-safe work queue processing

## Overview

This application implements a feature that enables a button to blink 3 times with frequency of 10 Hz if button is in high state. If, however, the button is in low state, the LED turns on for 500 ms and then turns off. 
That means that when button is pressed, the LED will blink 3 times and when the button is pressed and held, it will turn on and stay on for 500 ms and then inevitabely blink 3 times when the button is turned on.

A simple demo can be seen here:
![Button-LED Demo](/non_trivial_button_led_app/doc/demo.gif "Demo of button press triggering LED blink")

This application uses zephyr's [zbus](https://docs.zephyrproject.org/latest/services/zbus/index.html) that essentially enables the button to communicate with LEDs.
Pressing of a button is registered via an interrupt service routine inside of which a [workqueue thread](https://docs.zephyrproject.org/latest/kernel/services/threads/workqueue.html) is registered inside of which button state change is published on zbus.

An observer of the topic (channel) of interest (`button_chan`) reacts to button being pressed in it it's callback function by spawning a thread in which the LED is then manipulated.

### Application type
This application is a workspace application that is based on [Zephyr example project](https://github.com/zephyrproject-rtos/example-application) since using a [reference workspace application](https://docs.zephyrproject.org/latest/develop/application/index.html#using-a-reference-workspace-application) is the recommended way of getting started with application development.

## Building and flashing

To build the app I used the following command to build for my nRF52dk:

```
west build -p always -b nrf52dk/nrf52832
```

I used `nrfjprog` since it seemed like simplest way that didn't require addition hardware. I made sure that the board was plugged in to the PC and I ran the following:

```
west flash --runner nrfjprog
```

Then in a seprate terminal, I opened picocom with:

```
picocom -b 115200 /dev/ACM0
```

and observed the output.