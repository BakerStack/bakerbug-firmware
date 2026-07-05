# BakerStack hardware drivers

Reusable sensor and actuator drivers for ESP8266/ESP32 projects. Sits **above BSP**, **below application** code.

## Layering

```
Application  →  drivers/  →  BSP (wifi, gpio, …)  →  Arduino
```

Apps pass **pins and calibration at init** — drivers never hard-code board wiring.

## Drivers

| Driver | Header | README |
|--------|--------|--------|
| DS18B20 temperature | `drivers/ds18b20.h` | [docs/ds18b20.md](docs/ds18b20.md) |
| Potentiometer (ADC %) | `drivers/potentiometer.h` | [docs/potentiometer.md](docs/potentiometer.md) |
| Fan PWM + ramp | `drivers/fan_control.h` | [docs/fan_control.md](docs/fan_control.md) |

Legacy sources in `src/` (`scd41.cpp`, `watchdog.cpp`) are **not** linked by the default `drivers` library — they need product-specific deps (Sensirion I2C, BSP GPIO).

## PlatformIO

```ini
lib_extra_dirs =
    path/to/bakerbug-firmware/firmware/lib

lib_deps =
    drivers
```

OneWire and DallasTemperature are pulled in automatically via `library.json` dependencies.

## API convention

Each driver uses **init / set-or-read / get / loop** ([dorinda ADR-0012](https://github.com/BakerStack/dorinda-fan-control/blob/main/docs/decisions/0012-hw-component-api.md)):

- `_init(...)` — pin + config once in `setup()`
- `_read_*` / `_set_*` — domain units (%, °C)
- `_get_*` / `_raw` — state or debug
- `_loop()` — call every `loop()` for time-based drivers (fan ramp)

Never call blocking sensor reads or `fan_control_loop()` from async HTTP handlers.
