// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdbool.h>
#include <stdint.h>

namespace drivers {

// DS18B20 on any GPIO with external 4.7 kΩ pull-up to 3V3.
void ds18b20_init(uint8_t data_pin);
bool ds18b20_read_celsius(float* out_c);  // false if missing / disconnected

}  // namespace drivers
