#ifdef CONFIG_ARCH_ESP8266

#include <Arduino.h>
#include <bsp/gpio.h>

static bsp_gpio_irq_handler_t irq_handlers[16];
static void* irq_args[16];

static void IRAM_ATTR gpio_isr_trampoline0() { if (irq_handlers[0]) irq_handlers[0](irq_args[0]); }
static void IRAM_ATTR gpio_isr_trampoline1() { if (irq_handlers[1]) irq_handlers[1](irq_args[1]); }
static void IRAM_ATTR gpio_isr_trampoline2() { if (irq_handlers[2]) irq_handlers[2](irq_args[2]); }
static void IRAM_ATTR gpio_isr_trampoline3() { if (irq_handlers[3]) irq_handlers[3](irq_args[3]); }
static void IRAM_ATTR gpio_isr_trampoline4() { if (irq_handlers[4]) irq_handlers[4](irq_args[4]); }
static void IRAM_ATTR gpio_isr_trampoline5() { if (irq_handlers[5]) irq_handlers[5](irq_args[5]); }
static void IRAM_ATTR gpio_isr_trampoline12() { if (irq_handlers[12]) irq_handlers[12](irq_args[12]); }
static void IRAM_ATTR gpio_isr_trampoline13() { if (irq_handlers[13]) irq_handlers[13](irq_args[13]); }
static void IRAM_ATTR gpio_isr_trampoline14() { if (irq_handlers[14]) irq_handlers[14](irq_args[14]); }
static void IRAM_ATTR gpio_isr_trampoline15() { if (irq_handlers[15]) irq_handlers[15](irq_args[15]); }

static void (*trampolines[16])() = {
    gpio_isr_trampoline0,
    gpio_isr_trampoline1,
    gpio_isr_trampoline2,
    gpio_isr_trampoline3,
    gpio_isr_trampoline4,
    gpio_isr_trampoline5,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    gpio_isr_trampoline12,
    gpio_isr_trampoline13,
    gpio_isr_trampoline14,
    gpio_isr_trampoline15
};

bool bsp_gpio_init(bsp_gpio_t* gpio, int pin, bsp_gpio_mode_t mode, bsp_gpio_pull_t pull)
{
    gpio->pin = pin;

    uint8_t m = (mode == BSP_GPIO_OUTPUT) ? OUTPUT : INPUT;
    if (pull == BSP_GPIO_PULLUP) m = INPUT_PULLUP;

    pinMode(pin, m);
    return true;
}

bool bsp_gpio_write(const bsp_gpio_t* gpio, bool level)
{
    digitalWrite(gpio->pin, level ? HIGH : LOW);
    return true;
}

bool bsp_gpio_read(const bsp_gpio_t* gpio)
{
    return digitalRead(gpio->pin);
}

bool bsp_gpio_attach_interrupt(
    const bsp_gpio_t* gpio,
    bsp_gpio_irq_mode_t mode,
    bsp_gpio_irq_handler_t handler,
    void* arg
)
{
    int pin = gpio->pin;
    if (!trampolines[pin]) return false;

    irq_handlers[pin] = handler;
    irq_args[pin] = arg;

    int arduino_mode = CHANGE;
    if (mode == BSP_GPIO_IRQ_RISING) arduino_mode = RISING;
    if (mode == BSP_GPIO_IRQ_FALLING) arduino_mode = FALLING;

    attachInterrupt(digitalPinToInterrupt(pin), trampolines[pin], arduino_mode);
    return true;
}

void bsp_gpio_detach_interrupt(const bsp_gpio_t* gpio)
{
    detachInterrupt(digitalPinToInterrupt(gpio->pin));
    irq_handlers[gpio->pin] = nullptr;
    irq_args[gpio->pin] = nullptr;
}

#endif
