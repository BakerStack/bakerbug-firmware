#pragma once
#include <stdint.h>
#include <stdbool.h>


#if defined(CONFIG_ARCH_ESP8266)
    #include <Arduino.h>         
    #define BSP_ISR_ATTR IRAM_ATTR

#elif defined(CONFIG_ARCH_ESP32)
    #include <Arduino.h>         
    #define BSP_ISR_ATTR IRAM_ATTR

#else
    #define BSP_ISR_ATTR
#endif

typedef struct {
    int pin;
} bsp_gpio_t;

typedef enum {
    BSP_GPIO_INPUT,
    BSP_GPIO_OUTPUT
} bsp_gpio_mode_t;

typedef enum {
    BSP_GPIO_NOPULL,
    BSP_GPIO_PULLUP,
    BSP_GPIO_PULLDOWN
} bsp_gpio_pull_t;

typedef enum {
    BSP_GPIO_IRQ_RISING,
    BSP_GPIO_IRQ_FALLING,
    BSP_GPIO_IRQ_CHANGE
} bsp_gpio_irq_mode_t;

typedef void (*bsp_gpio_irq_handler_t)(void* arg);

bool bsp_gpio_init(bsp_gpio_t* gpio, int pin, bsp_gpio_mode_t mode, bsp_gpio_pull_t pull);
bool bsp_gpio_write(const bsp_gpio_t* gpio, bool level);
bool bsp_gpio_read(const bsp_gpio_t* gpio);

bool bsp_gpio_attach_interrupt(
    const bsp_gpio_t* gpio,
    bsp_gpio_irq_mode_t mode,
    bsp_gpio_irq_handler_t handler,
    void* arg
);

void bsp_gpio_detach_interrupt(const bsp_gpio_t* gpio);
