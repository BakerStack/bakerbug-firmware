.DEFAULT_GOAL := help

# Shared toolchain from firmware root
VENV = ../../.venv
PIO  = $(VENV)/bin/pio

# Default board = safest (most common lab device)
ENV ?= esp8266

# -------------------------------------------------------
help:
	@echo "--------------------------------------------------"
	@echo " Device Provisioning Firmware"
	@echo "--------------------------------------------------"
	@echo " build ENV=esp8266|esp32|linux   - Compile identity firmware"
	@echo " upload ENV=esp8266|esp32        - Flash identity firmware"
	@echo " read-id                        - Read DEVICE_IDENTITY from serial"
	@echo " clean                          - Remove all build artifacts"
	@echo "--------------------------------------------------"
	@echo " Default ENV = $(ENV)"
	@echo "--------------------------------------------------"

# -------------------------------------------------------
build:
	$(PIO) run -e $(ENV)

upload:
	$(PIO) run -e $(ENV) -t upload

# Reads DEVICE_IDENTITY from provisioning firmware
read-id:
	@echo "Waiting for DEVICE_IDENTITY from device..."
	@$(PIO) device monitor --quiet | grep DEVICE_IDENTITY

# -------------------------------------------------------
clean:
	rm -rf .pio
