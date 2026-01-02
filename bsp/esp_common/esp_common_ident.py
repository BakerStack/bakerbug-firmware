#!/usr/bin/env python3
import subprocess
import json
import sys
import re

def run(cmd):
    try:
        return subprocess.check_output(cmd, stderr=subprocess.STDOUT, text=True)
    except subprocess.CalledProcessError as e:
        return e.output

def extract(regex, text):
    m = re.search(regex, text)
    return m.group(1).strip() if m else None

def main():
    data = {}

    # Chip ID
    out = run(["esptool", "chip-id"])
    data["chip_id"] = extract(r"Chip ID:\s*(0x[0-9a-fA-F]+)", out)

    # MAC
    out = run(["esptool", "read-mac"])
    data["mac"] = extract(r"MAC:\s*([0-9a-fA-F:]+)", out)

    # Flash JEDEC
    out = run(["esptool", "flash-id"])
    data["flash_jedec"] = extract(r"Flash ID:\s*(0x[0-9a-fA-F]+)", out)
    data["flash_vendor"] = extract(r"Manufacturer:\s*(.+)", out)
    data["flash_model"] = extract(r"Device:\s*(.+)", out)

    # Security (ESP32 only)
    out = run(["esptool", "get-security-info"])
    if "Command not implemented" in out or "FF00" in out:
        data["secure_boot"] = "not supported"
        data["flash_encryption"] = "not supported"
        data["soc_security"] = "none (ESP8266-class)"
    else:
        data["secure_boot"] = extract(r"Secure boot:\s*(.+)", out)
        data["flash_encryption"] = extract(r"Flash encryption:\s*(.+)", out)
        data["soc_security"] = "esp32-class"

    # esptool version
    out = run(["esptool", "version"])
    data["esptool_version"] = extract(r"esptool v([0-9\.]+)", out)

    if not data.get("mac"):
        print("ERROR: Could not read MAC. Is device in bootloader mode?", file=sys.stderr)
        sys.exit(2)

    data["device_id"] = data["mac"].replace(":", "").lower()

    print(json.dumps(data, indent=2))

if __name__ == "__main__":
    main()
