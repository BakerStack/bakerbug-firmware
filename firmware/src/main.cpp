#include <Arduino.h>

#include <bsp/log.h>
#include <bsp/wifi.h>
#include <bsp/tls.h>
#include <bsp/time.h>
#include <secrets/wifi.h>
#include <mqtt/mqtt_basic.h>
#include <secrets/mqtt_credentials.h>

#ifndef BB_MQTT_USE_TLS
#error "This firmware requires BB_MQTT_USE_TLS. Refusing to build insecure binary."
#endif

#ifndef BB_MQTT_BROKER_CA_PEM
#error "No CA file specified. TLS trust anchor missing."
#endif

MQTTBasic *mqtt = nullptr;

void on_message(const char *topic, const char *payload)
{
    log_printf(LOG_LEVEL_INFO, "RX %s = %s", topic, payload);
}

void setup()
{
    log_init(115200);
    delay(100);
    log_printf(LOG_LEVEL_INFO, "Booting");

    if (!bsp_wifi_connect(WIFI_SSID_STR, WIFI_PASSWORD_STR))
    {
        log_printf(LOG_LEVEL_ERROR, "WiFi connect failed");
        return;
    }

    log_printf(LOG_LEVEL_INFO, "WiFi OK, IP=%s", bsp_wifi_ip());

    bsp_time_is_valid();

    // mqtt = new MQTTBasic("test.mosquitto.org", 1883);
    mqtt = new MQTTBasic("test.mosquitto.org", 8883);

    mqtt->set_callback(on_message);

    // Client* tcp = bsp_tcp_client();
    bsp_tls_begin(&MQTT_TRUST);
    Client *transport = bsp_tls_client();

    if (!mqtt->connect(*transport))
    {
        int state = mqtt->state();
        log_printf(LOG_LEVEL_ERROR, "MQTT state=%d", state);
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
