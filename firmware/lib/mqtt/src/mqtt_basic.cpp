// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#include <mqtt/mqtt_basic.h>
#include <bsp/log.h>
#include <PubSubClient.h>
#include <string.h>
 

static MQTTBasic* _active = nullptr;
static PubSubClient _client;
static bool _mqtt_instance_created = false; // Only one instance of MQTTBasic is allowed

static void _mqtt_cb(char* topic, byte* payload, unsigned int len)
{
    if (!_active) return;

    static char buf[256];
    if (len >= sizeof(buf))
        len = sizeof(buf) - 1;

    memcpy(buf, payload, len);
    buf[len] = 0;

    if (_active->_cb)
        _active->_cb(topic, buf);
}

MQTTBasic::MQTTBasic(const char* host, uint16_t port)
{ 
    if (_mqtt_instance_created)
    {
        log_printf(LOG_LEVEL_ERROR, "Only one instance of MQTTBasic is allowed");
        // fatal
        while (1);
    }
    _mqtt_instance_created = true;
    _host = host;
    _port = port;
}

bool MQTTBasic::connect(Client& transport)
{
    _client.setClient(transport);
    _client.setServer(_host, _port);

    _active = this;
    _client.setCallback(_mqtt_cb);

    return _client.connect("bakerbug");
}

bool MQTTBasic::connected()
{
    return _client.connected();
}

bool MQTTBasic::publish(const char* topic, const char* payload)
{
    return _client.publish(topic, payload);
}

bool MQTTBasic::subscribe(const char* topic)
{
    return _client.subscribe(topic);
}

void MQTTBasic::set_callback(mqtt_message_cb_t cb)
{
    _cb = cb;
}

void MQTTBasic::loop()
{
    _client.loop();
}

int MQTTBasic::state()
{
    return _client.state();
}
