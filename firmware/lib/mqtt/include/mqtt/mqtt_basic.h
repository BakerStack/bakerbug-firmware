// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#pragma once
#include <Client.h>
#include <stdint.h>

typedef void (*mqtt_message_cb_t)(const char* topic, const char* payload);

class MQTTBasic
{
public:
    MQTTBasic(const char* host, uint16_t port);

    bool connect(Client& transport);
    bool connected();

    bool publish(const char* topic, const char* payload);
    bool subscribe(const char* topic);

    void set_callback(mqtt_message_cb_t cb);
    void loop();
    int state();
private:
    const char* _host;
    uint16_t _port;
public:
    // Message callback needs to be public for static callback access
    mqtt_message_cb_t _cb = nullptr;
};
