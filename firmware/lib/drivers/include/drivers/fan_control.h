// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

namespace drivers {

// PWM fan output: percent setpoint with ramped slew rate.
// Call fan_control_force_off() as the first line of setup() — before Serial/WiFi.
void fan_control_force_off(uint8_t pwm_pin);
void fan_control_init(uint8_t pwm_pin, uint32_t ramp_interval_ms, uint8_t ramp_step_percent,
                      uint32_t pwm_hz);
void fan_control_set_new_setpoint(int percent_0_100);
void fan_control_loop();
int fan_control_get_setpoint();
int fan_control_get_current_value();
uint16_t fan_control_get_pwm_raw();
bool fan_control_is_ramping();

}  // namespace drivers
