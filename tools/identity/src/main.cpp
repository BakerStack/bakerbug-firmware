// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#ifdef ARCH_LINUX
#include <fstream>
#include <string>
#include <iostream>

std::string read_file(const char* path) {
    std::ifstream f(path);
    std::string s;
    std::getline(f, s);
    return s;
}

int main() {
    std::string id = read_file("/etc/machine-id");
    if (id.empty())
        id = read_file("/sys/class/dmi/id/product_uuid");

    std::cout << "DEVICE_IDENTITY=" << id << std::endl;
    return 0;
}
#else
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

#ifdef ARCH_ESP32
    uint64_t id = ESP.getEfuseMac();
    char buf[32];
    sprintf(buf, "%04X%08X",
            (uint16_t)(id >> 32),
            (uint32_t)id);
    Serial.printf("DEVICE_IDENTITY=%s\n", buf);

#elif defined(ARCH_ESP8266)
    uint32_t id = ESP.getChipId();
    char buf[16];
    sprintf(buf, "%08X", id);
    Serial.printf("DEVICE_IDENTITY=%s\n", buf);
#else
    Serial.println("DEVICE_IDENTITY=UNKNOWN");
#endif

    while(true) {
        delay(1000);
    }
}

void loop() {}
#endif
