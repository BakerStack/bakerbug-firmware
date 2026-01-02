// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#pragma once
#ifndef BB_MQTT_BROKER_PORT
#define BB_MQTT_BROKER_PORT 1883
#endif
#ifndef BB_MQTT_BROKER_HOST
#define BB_MQTT_BROKER_HOST "127.0.0.1"
#endif
#ifndef BB_MQTT_USER
#define BB_MQTT_USER "user"
#endif
#ifndef BB_MQTT_PASS
#define BB_MQTT_PASS "pass"
#endif
#ifndef BB_MQTT_BROKER_NAME
#define BB_MQTT_BROKER_NAME "local_broker"
#endif

#ifndef BB_MQTT_USE_TLS
#error "BB_MQTT_USE_TLS must be defined to 0 or 1"
#endif

#if BB_MQTT_USE_TLS != 0 && BB_MQTT_USE_TLS != 1
#error "BB_MQTT_USE_TLS must be 0 or 1"
#endif

#if BB_MQTT_USE_TLS
#include <bsp/tls.h>
extern const tls_trust_bundle MQTT_TRUST;
#endif // BB_MQTT_USE_TLS

extern const char* MQTT_BROKER_HOST;
extern const int   MQTT_BROKER_PORT_NR;
extern const char* MQTT_USER_STR;
extern const char* MQTT_PASS_STR;
