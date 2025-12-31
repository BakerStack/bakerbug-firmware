// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#pragma once
#include <Arduino.h>

void co2_init(void);

/*
 * Returnerar true endast när ett NYTT giltigt värde finns
 * (warm-up filtreras bort internt).
 */
bool co2_measure(void);

/* Senaste giltiga värden */
uint16_t co2_ppm(void);
float co2_temperature(void);
float co2_humidity(void);
