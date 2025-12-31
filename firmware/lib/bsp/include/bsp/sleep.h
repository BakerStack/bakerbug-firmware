// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#pragma once
#include <stdint.h>

/* Sleep for N milliseconds (cooperative) */
void bsp_sleep_ms(uint32_t ms);

/* Yield to scheduler / background tasks */
void bsp_yield();

/* Current uptime in milliseconds */
uint32_t bsp_millis();
