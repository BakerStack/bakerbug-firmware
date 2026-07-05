# Potentiometer driver

Maps an analog potentiometer to **0–100 %** using a calibrated ADC range.

## Hardware

- Pot wiper → MCU ADC pin (e.g. ESP8266 A0)
- Ends across 3V3 and GND (wiper must stay within ADC range)

## API

| Function | Description |
|----------|-------------|
| `pot_init(adc_pin, adc_min, adc_max)` | Pin + calibrated endpoints from bench test |
| `pot_read_value_percentage()` | 0–100 % |
| `pot_read_raw()` | Raw ADC counts (dashboard / debug) |

## Example

```cpp
#include <drivers/potentiometer.h>

void setup() {
    drivers::pot_init(A0, 5, 1024);  // values from TEST 1 bench log
}

void loop() {
    Serial.printf("Pot %d%% (adc=%d)\n",
                  drivers::pot_read_value_percentage(),
                  drivers::pot_read_raw());
    delay(500);
}
```

## Gotchas

- Calibrate `adc_min` / `adc_max` per board — do not assume 0–1023.
- ESP8266 A0 is **0–3.3 V** max.

## Bench checklist

1. Wiper at minimum → ~0 %.
2. Wiper at maximum → ~100 %.
3. Log raw ADC at both ends for `pot_init` args.
