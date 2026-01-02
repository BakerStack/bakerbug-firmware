#!/usr/bin/env python3
import json
import subprocess
import re
import sys


def run_esptool(args):
    cmd = ["esptool"] + args
    try:
        out = subprocess.check_output(cmd, stderr=subprocess.STDOUT, text=True)
        return out
    except subprocess.CalledProcessError as e:
        return e.output


def parse_key_value(output):
    data = {}
    for line in output.splitlines():
        if ":" in line:
            k, v = line.split(":", 1)
            data[k.strip()] = v.strip()
    return data


def get_chip_info():
    out = run_esptool(["chip-id"])
    return parse_key_value(out)


def get_mac():
    out = run_esptool(["read-mac"])
    m = re.search(r"MAC:\s+([0-9a-f:]+)", out, re.IGNORECASE)
    return m.group(1) if m else None


def get_flash_id():
    out = run_esptool(["flash-id"])
    return parse_key_value(out)


def get_security_info():
    out = run_esptool(["get-security-info"])
    if "Command not implemented" in out:
        return None
    return parse_key_value(out)


def detect_port():
    out = run_esptool(["chip-id"])
    m = re.search(r"on (\S+)", out)
    return m.group(1) if m else None


def main():
    port = detect_port()

    chip = get_chip_info()
    mac = get_mac()
    flash = get_flash_id()
    security = get_security_info()

    result = {
        "transport": {
            "type": "serial",
            "port": port,
        },
        "chip": chip,
        "mac": mac,
        "flash": flash,
        "security": security,
    }

    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
