#pragma once
#include <stdint.h>

uint64_t bsp_time_unix();     // seconds since 1970
uint64_t bsp_time_ms();       // milliseconds since epoch
bool     bsp_time_is_valid(); // NTP or RTC synced?
void     bsp_time_set_unix(uint64_t unix_secs);
void     bsp_time_set_ms(uint64_t ms_since_epoch);  