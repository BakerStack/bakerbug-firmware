
# BakerBug Firmware

Embedded firmware for the BakerBug IoT sensor device, supporting multiple microcontroller architectures through a hardware abstraction layer.

## Repository Layout

```
firmware/
├── platformio.ini          # PlatformIO build configuration
├── README.md              # This file
├── secrets-template.env   # Template for environment secrets
├── secrets.env           # Your local secrets (gitignored)
├── certs/                # TLS certificates
├── src/
│   └── main.cpp          # Application entry point
├── lib/
│   ├── bsp/              # Board Support Package (BSP)
│   │   ├── esp8266/      # ESP8266 implementation
│   │   ├── esp32/        # ESP32 implementation
│   │   └── include/bsp/  # BSP header files
│   ├── drivers/          # Hardware drivers (SCD41, etc.)
│   │   ├── include/drivers/
│   │   └── src/
│   ├── mqtt/             # MQTT client module
│   │   ├── include/mqtt/
│   │   └── src/
│   └── secrets/          # Secret injection library
│       ├── include/secrets/
│       └── src/
└── test/                 # Test code
```

## Architectural Principles

### Layered Architecture

The firmware follows a strict layered architecture to ensure portability and maintainability:

```
┌─────────────────────────────────┐
│      Application (main.cpp)     │
├─────────────────────────────────┤
│    Software Modules (mqtt/)     │
├─────────────────────────────────┤
│      Drivers (drivers/)         │
├─────────────────────────────────┤
│  Board Support Package (bsp/)   │
└─────────────────────────────────┘
```

**Dependency Rules:**
- `main.cpp` → Software modules and drivers only
- Software modules → Drivers, secrets, and BSP
- Drivers → BSP only
- **main.cpp must NOT directly reference BSP or secrets**

### Board Support Package (BSP)

The BSP provides a hardware abstraction layer that isolates architecture-specific code from the application and drivers. This enables the same application code to run on different microcontroller families.

**BSP Modules:**
- `crypto.h/.cpp` - Cryptographic operations
- `gpio.h/.cpp` - GPIO control
- `i2c.h/.cpp` - I2C communication
- `sleep.h/.cpp` - Power management
- `time.h/.cpp` - Time and delay functions
- `tls.h/.cpp` - TLS/SSL operations
- `wifi.h/.cpp` - WiFi connectivity

**Supported Architectures:**
- **ESP8266** - Espressif ESP8266 (e.g., D1 Mini)
- **ESP32** - Espressif ESP32 (future support)

Each architecture has its own implementation directory (`lib/bsp/esp8266/`, `lib/bsp/esp32/`) while sharing common header files in `lib/bsp/include/bsp/`.

### Secrets and Certificates

Secrets are defined in environment variables and injected at compile time for security.

- **Environment variables** - WiFi credentials, MQTT broker details, etc.
- **Certificates** - Stored in `certs/` directory for TLS connections
- **Injection point** - `lib/secrets/` handles the compile-time injection

**Setup:**
```bash
# Create your local secrets file (once)
cp secrets-template.env secrets.env
nano secrets.env  # Edit with your credentials

# Source before building (each session)
source secrets.env
```

The `secrets.env` file should be gitignored and never committed to version control.

## Building and Uploading

### Prerequisites

- [PlatformIO Core](https://platformio.org/install) or PlatformIO IDE
- USB drivers for your target board

### Setting the Architecture

The target architecture is configured in [platformio.ini](platformio.ini) using PlatformIO environments.

**For ESP8266 (D1 Mini):**
```ini
[env:d1_mini]
platform = espressif8266
board = d1_mini
framework = arduino
build_flags =
  -DCONFIG_ARCH_ESP8266
```

**For ESP32 (future):**
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
build_flags =
  -DCONFIG_ARCH_ESP32
```

The `CONFIG_ARCH_*` define tells the BSP which implementation to compile.

### Build Commands

**Using PlatformIO CLI:**
```bash
# Build for default environment (d1_mini)
pio run

# Build for specific environment
pio run -e d1_mini

# Clean build
pio run -t clean
```

**Using PlatformIO IDE:**
- Open the PlatformIO sidebar
- Select environment from the bottom toolbar
- Click "Build" button or use the Build task

### Upload/Download to Device

**Using PlatformIO CLI:**
```bash
# Upload to connected device
pio run -t upload

# Upload and open serial monitor
pio run -t upload && pio device monitor

# Specify upload port
pio run -t upload --upload-port /dev/cu.usbserial-*
```

**Using PlatformIO IDE:**
- Connect device via USB
- Click "Upload" button or use Ctrl+Alt+U (Cmd+Shift+U on macOS)
- Click "Serial Monitor" to view device output

### Monitoring

```bash
# Open serial monitor (115200 baud)
pio device monitor

# Or combined upload + monitor
pio run -t upload -t monitor
```

## Dependencies

The firmware uses several external libraries managed through PlatformIO's Library Manager. Key dependencies include:
- Arduino framework
- ESP8266/ESP32 WiFi libraries
- PubSubClient (MQTT)
- Wire (I2C)

All dependencies are automatically resolved during build.

## Architecture
