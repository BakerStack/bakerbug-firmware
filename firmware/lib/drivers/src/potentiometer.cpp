// SPDX-License-Identifier: Apache-2.0

#include <drivers/potentiometer.h>

#include <Arduino.h>

namespace drivers {

namespace {

uint8_t g_adc_pin = A0;
int16_t g_adc_min = 0;
int16_t g_adc_max = 1023;

}  // namespace

void pot_init(uint8_t adc_pin, int16_t adc_min, int16_t adc_max)
{
    g_adc_pin = adc_pin;
    g_adc_min = adc_min;
    g_adc_max = adc_max;
}

int pot_read_value_percentage()
{
    const int16_t raw = pot_read_raw();
    const int span = static_cast<int>(g_adc_max) - static_cast<int>(g_adc_min);
    if (span <= 0)
    {
        return 0;
    }

    int pct = ((static_cast<int>(raw) - g_adc_min) * 100) / span;
    if (pct < 0)
    {
        return 0;
    }
    if (pct > 100)
    {
        return 100;
    }
    return pct;
}

int16_t pot_read_raw()
{
    return static_cast<int16_t>(analogRead(g_adc_pin));
}

}  // namespace drivers
