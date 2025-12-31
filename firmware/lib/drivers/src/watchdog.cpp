// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#include <drivers/watchdog.h>
#include <bsp/gpio.h>
#include <bsp/sleep.h>
#include <bsp/log.h>
 
static bsp_gpio_t watchdog_port;
static bsp_gpio_t watchdog_interrupt_port;
static int watchdog_reset_msec;
static volatile bool watchdog_warning_issued = false;

static void BSP_ISR_ATTR watchdog_irq_handler(void* arg)
{
    watchdog_warning_issued = true;
}  

void watchdog_init(int watchdog_pin, int reset_msec, int watchdog_interrupt_pin)
{
    bsp_gpio_init(&watchdog_port, watchdog_pin, BSP_GPIO_OUTPUT, BSP_GPIO_NOPULL);
    bsp_gpio_write(&watchdog_port, false); // Start low

    watchdog_reset_msec = reset_msec;
    if (watchdog_interrupt_pin >= 0)
    {
        bsp_gpio_init(&watchdog_interrupt_port, watchdog_interrupt_pin, BSP_GPIO_INPUT, BSP_GPIO_PULLUP); 
        bsp_gpio_attach_interrupt(&watchdog_interrupt_port, BSP_GPIO_IRQ_FALLING, watchdog_irq_handler, nullptr);
    }
}

void watchdog_pat()
{
    watchdog_warning_issued = false;
    bsp_gpio_write(&watchdog_port, true);
    bsp_sleep_ms(watchdog_reset_msec);
    bsp_gpio_write(&watchdog_port, false);
}


bool watchdog_ok()
{ 
    return !watchdog_warning_issued;
}   
