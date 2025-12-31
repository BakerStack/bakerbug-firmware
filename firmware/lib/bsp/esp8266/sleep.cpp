// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#ifdef CONFIG_ARCH_ESP8266

#include <Arduino.h>
#include <bsp/sleep.h>

void bsp_sleep_ms(uint32_t ms)
{
    delay(ms);
}

void bsp_yield()
{
    yield();
}

uint32_t bsp_millis()
{
    return millis();
}

#endif
