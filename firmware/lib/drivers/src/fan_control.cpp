// SPDX-License-Identifier: Apache-2.0

#include <drivers/fan_control.h>

#include <Arduino.h>

namespace drivers {

namespace {

constexpr uint16_t FAN_PWM_MAX = 1023;

uint8_t g_pwm_pin = 0;
uint32_t g_ramp_interval_ms = 200;
uint8_t g_ramp_step_pct = 6;

int g_setpoint_pct = 0;
int g_current_pct = 0;
uint32_t g_last_ramp_ms = 0;

uint16_t percent_to_pwm(int pct)
{
    if (pct < 0)
    {
        pct = 0;
    }
    if (pct > 100)
    {
        pct = 100;
    }
    return static_cast<uint16_t>((pct * FAN_PWM_MAX) / 100);
}

void write_pwm_percent(int pct)
{
    analogWrite(g_pwm_pin, percent_to_pwm(pct));
}

}  // namespace

void fan_control_init(uint8_t pwm_pin, uint32_t ramp_interval_ms, uint8_t ramp_step_percent,
                      uint32_t pwm_hz)
{
    g_pwm_pin = pwm_pin;
    g_ramp_interval_ms = ramp_interval_ms;
    g_ramp_step_pct = ramp_step_percent > 0 ? ramp_step_percent : 1;

    pinMode(g_pwm_pin, OUTPUT);
    analogWriteRange(FAN_PWM_MAX);
    analogWriteFreq(pwm_hz);

    g_setpoint_pct = 0;
    g_current_pct = 0;
    g_last_ramp_ms = millis();
    write_pwm_percent(0);
}

void fan_control_set_new_setpoint(int percent_0_100)
{
    if (percent_0_100 < 0)
    {
        percent_0_100 = 0;
    }
    if (percent_0_100 > 100)
    {
        percent_0_100 = 100;
    }
    g_setpoint_pct = percent_0_100;
}

void fan_control_loop()
{
    if (g_current_pct == g_setpoint_pct)
    {
        return;
    }

    const uint32_t now = millis();
    if (now - g_last_ramp_ms < g_ramp_interval_ms)
    {
        return;
    }
    g_last_ramp_ms = now;

    if (g_current_pct < g_setpoint_pct)
    {
        const int delta = g_setpoint_pct - g_current_pct;
        const int step = delta < g_ramp_step_pct ? delta : g_ramp_step_pct;
        g_current_pct += step;
    }
    else
    {
        const int delta = g_current_pct - g_setpoint_pct;
        const int step = delta < g_ramp_step_pct ? delta : g_ramp_step_pct;
        g_current_pct -= step;
    }

    write_pwm_percent(g_current_pct);
}

int fan_control_get_setpoint()
{
    return g_setpoint_pct;
}

int fan_control_get_current_value()
{
    return g_current_pct;
}

uint16_t fan_control_get_pwm_raw()
{
    return percent_to_pwm(g_current_pct);
}

bool fan_control_is_ramping()
{
    return g_current_pct != g_setpoint_pct;
}

}  // namespace drivers
