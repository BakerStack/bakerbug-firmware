// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#include <Arduino.h>
#include <time.h>
#include <bsp/time.h>
#include <bsp/log.h>

static bool time_valid = false;
static uint64_t boot_epoch = 0;
static uint32_t boot_millis = 0;

static void ntp_sync()
{
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    log_printf(LOG_LEVEL_INFO, "NTP syncing...");

    time_t now;
    for (int i = 0; i < 20; i++)
    {
        time(&now);
        if (now > 1600000000)   // ~2020
        {
            time_valid = true;
            boot_epoch = (uint64_t)now;
            boot_millis = millis();
            log_printf(LOG_LEVEL_INFO, "NTP sync OK: %lu", (unsigned long)now);
            return;
        }
        delay(500);
    }

    log_printf(LOG_LEVEL_WARN, "NTP sync failed");
}

bool bsp_time_is_valid()
{
    if (!time_valid)
        ntp_sync();

    return time_valid;
}

uint64_t bsp_time_unix()
{
    if (!time_valid)
        ntp_sync();

    time_t now;
    time(&now);
    return (uint64_t)now;
}

uint64_t bsp_time_ms()
{
    if (!time_valid)
        ntp_sync();

    uint32_t delta = millis() - boot_millis;
    return (boot_epoch * 1000ULL) + delta;
}
