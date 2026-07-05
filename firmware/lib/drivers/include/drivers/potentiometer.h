// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

namespace drivers {

// Analog potentiometer: ADC pin + calibrated min/max → 0–100 %.
void pot_init(uint8_t adc_pin, int16_t adc_min, int16_t adc_max);
int pot_read_value_percentage();
int16_t pot_read_raw();

}  // namespace drivers
