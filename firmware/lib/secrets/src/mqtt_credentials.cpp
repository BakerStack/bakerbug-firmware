// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#include <secrets/mqtt_credentials.h>
#include <cstdlib>

const char *MQTT_BROKER_HOST = BB_MQTT_BROKER_HOST;
const int MQTT_BROKER_PORT_NR = atoi(BB_MQTT_BROKER_PORT);
const char *MQTT_USER_STR = BB_MQTT_USER;
const char *MQTT_PASS_STR = BB_MQTT_PASS;

#if BB_MQTT_USE_TLS

#include  BB_MQTT_BROKER_CA_PEM

#endif // BB_MQTT_USE_TLS