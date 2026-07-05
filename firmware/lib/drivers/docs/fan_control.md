# Fan control driver

Percent setpoint with **ramped** PWM output for MOSFET low-side fan switching.

## Hardware

- PWM GPIO → MOSFET module TRIG (e.g. XY-MOS)
- Common GND with fan 12 V supply
- Flyback diode across fan if module has none

## API

| Function | Description |
|----------|-------------|
| `fan_control_init(pin, ramp_ms, step_pct, pwm_hz)` | Pin, ramp timing, PWM frequency |
| `fan_control_set_new_setpoint(0–100)` | Target output % |
| `fan_control_loop()` | Advance ramp — **every `loop()`** |
| `fan_control_get_setpoint()` | Last commanded % |
| `fan_control_get_current_value()` | Actual ramp position % |
| `fan_control_get_pwm_raw()` | Duty 0–1023 (cal matrix / debug) |
| `fan_control_is_ramping()` | `true` while slewing |

## Example

```cpp
#include <drivers/fan_control.h>
#include <drivers/potentiometer.h>

void setup() {
    drivers::pot_init(A0, 5, 1024);
    drivers::fan_control_init(D5, 200, 6, 18000);
}

void loop() {
    drivers::fan_control_loop();
    drivers::fan_control_set_new_setpoint(drivers::pot_read_value_percentage());
}
```

## Gotchas

- ESP8266 `analogWriteFreq` typically **0–20 kHz** — Dorinda uses 18 kHz.
- Wait for ramp to finish before measuring fan current during calibration.
- Call `fan_control_loop()` before reading `fan_control_get_pwm_raw()`.

## Bench checklist

1. Pot 0 % → fan off after ramp settles.
2. Pot 100 % → full speed after ~3–4 s ramp (6 % / 200 ms).
3. Sudden pot jump → gradual slew, no inrush spike on ammeter.
