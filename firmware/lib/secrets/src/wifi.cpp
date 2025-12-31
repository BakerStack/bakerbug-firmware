// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#include <secrets/wifi.h>
#ifndef BBWIFI_SSID
#define BBWIFI_SSID "default_ssid"  // fallback value
#endif

#ifndef BBWIFI_PASSWORD
#define BBWIFI_PASSWORD "default_password"  // fallback value
#endif

// These are set in secrets.sh and injected via build system
const char WIFI_SSID_STR[] = BBWIFI_SSID;
const char WIFI_PASSWORD_STR[] = BBWIFI_PASSWORD;