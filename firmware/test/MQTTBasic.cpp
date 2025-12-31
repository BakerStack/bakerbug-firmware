#include <Arduino.h>

#include <bsp/log.h>
#include <bsp/wifi.h>
#include <secrets/wifi.h>
#include <mqtt/mqtt_basic.h>

MQTTBasic* mqtt = nullptr;

void on_message(const char* topic, const char* payload)
{
    log_printf(LOG_LEVEL_INFO, "RX %s = %s", topic, payload);
}

void setup()
{
    log_init(115200);
    log_printf(LOG_LEVEL_INFO, "Booting");

    if (!bsp_wifi_connect(WIFI_SSID_STR, WIFI_PASSWORD_STR))
    {
        log_printf(LOG_LEVEL_ERROR, "WiFi connect failed");
        return;
    }

    log_printf(LOG_LEVEL_INFO, "WiFi OK, IP=%s", bsp_wifi_ip());

    mqtt = new MQTTBasic("test.mosquitto.org", 1883);
    mqtt->set_callback(on_message);

    Client* tcp = bsp_tcp_client();

    if (!mqtt->connect(*tcp))
    {
        log_printf(LOG_LEVEL_ERROR, "MQTT connect failed");
        return;
    }

    log_printf(LOG_LEVEL_INFO, "MQTT connected");

    mqtt->subscribe("bakerbug/cmd");
    mqtt->publish("bakerbug/test", "online");
}

void loop()
{
    if (mqtt)
        mqtt->loop();
}
