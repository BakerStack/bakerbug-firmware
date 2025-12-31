// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#pragma once
#include <bsp/gpio.h>
// Initialize watchdog on specified GPIO pins
// reset_msec = time to hold watchdog pin high to avoid reset, this should be very short
// since it blocks everything else! Max recommentded time is 5-10ms.
 void watchdog_init(int watchdog_pin, int reset_msec, int watchdog_interrupt_pin = -1);
    // Must be called periodically to reset the external watchdog timer
 void watchdog_pat();
 // Check if watchdog has triggered since last pat
 // NOTE! this means that half of the HARD RESET cycle has elapsed!
 // So if this is true, you must pat the watchdog IMMEDIATELY to avoid reset.
 bool watchdog_ok();