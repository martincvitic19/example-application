/**
 * @file     main.cpp
 * @brief    Main application for button/LED interaction system
 * @author   Martin Cvitic
 * @date     2025-05-10
 * @version  1.0
 *
 * @details  Implements main application logic for:
 *           - Zbus message passing between components
 *           - LED output control
 *           - Button input with debouncing
 */

//=============================================================================
// INCLUDES
//=============================================================================
#include "read.h"
#include "react.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//=============================================================================
// MACROS
//=============================================================================
#define LED_NODE        DT_ALIAS(led1)    ///< Device tree node for LED
#define BUTTON_NODE     DT_ALIAS(sw0)     ///< Device tree node for button

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF); ///< Main application logger

#if !DT_NODE_HAS_STATUS_OKAY(BUTTON_NODE)
#error "Unsupported board: button devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS_OKAY(LED_NODE)
#error "Unsupported board: LED devicetree alias is not defined"
#endif

//============================================================================= 
// STATIC VARIABLES
//=============================================================================
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

//=============================================================================
// FUPUBLIC FUNCTIONS
//=============================================================================

/**
 * @brief Main application entry point
 *
 * Initializes system components and enters main processing loop.
 * 
 * @return int Application exit status (0 for success)
 */
int main(void)
{
    //---------------------------------------------------------------------
    // INITIALIZATION
    //---------------------------------------------------------------------
    ReadClass reader(button);
    ReactClass reactor(led);

    if (reader.init() != 0 || reactor.init() != 0)
    {
        LOG_ERR("Initialization failed");
        return -ENODEV;
    }

    LOG_INF("System ready. Press the button.");

    while (true)
    {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}