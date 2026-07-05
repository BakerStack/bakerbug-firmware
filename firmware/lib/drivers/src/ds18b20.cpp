// SPDX-License-Identifier: Apache-2.0

#include <drivers/ds18b20.h>

#include <DallasTemperature.h>
#include <OneWire.h>

namespace drivers {

namespace {

OneWire* g_one_wire = nullptr;
DallasTemperature* g_sensor = nullptr;
uint8_t g_data_pin = 0;

}  // namespace

void ds18b20_init(uint8_t data_pin)
{
    g_data_pin = data_pin;
    static OneWire one_wire(g_data_pin);
    static DallasTemperature sensor(&one_wire);
    g_one_wire = &one_wire;
    g_sensor = &sensor;

    g_sensor->begin();
    g_sensor->setWaitForConversion(true);
}

bool ds18b20_read_celsius(float* out_c)
{
    if (out_c == nullptr || g_sensor == nullptr)
    {
        return false;
    }

    g_sensor->requestTemperatures();
    const float c = g_sensor->getTempCByIndex(0);

    if (c == DEVICE_DISCONNECTED_C)
    {
        return false;
    }

    *out_c = c;
    return true;
}

}  // namespace drivers
