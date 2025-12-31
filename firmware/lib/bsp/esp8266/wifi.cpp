// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#ifdef CONFIG_ARCH_ESP8266

#include <ESP8266WiFi.h>
#include <bsp/wifi.h>
#include <bsp/log.h> 

static WiFiClient wifi;

Client* bsp_tcp_client()
{
    return &wifi;
}

bool bsp_wifi_connect(const char* ssid, const char* password)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        log_printf(LOG_LEVEL_DEBUG, "WiFi already connected, IP=%s", WiFi.localIP().toString().c_str());
        return true;
    }

    log_printf(LOG_LEVEL_INFO, "WiFi connecting to SSID '%s'", ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
    {
        delay(250);
        log_printf(LOG_LEVEL_DEBUG, "WiFi status = %d", WiFi.status());
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        log_printf(LOG_LEVEL_INFO, "WiFi connected, IP=%s", WiFi.localIP().toString().c_str());
       
        return true;
    }
    else
    {
        log_printf(LOG_LEVEL_ERROR, "WiFi connect timeout");
        return false;
    }
}


bool bsp_wifi_is_connected()
{
    bool ok = (WiFi.status() == WL_CONNECTED);
    log_printf(LOG_LEVEL_DEBUG, "WiFi status check = %d", ok);
    return ok;
}

void bsp_wifi_disconnect()
{   
    log_printf(LOG_LEVEL_INFO, "WiFi disconnect");
    WiFi.disconnect(true);
}

const char* bsp_wifi_ip()
{
    static String ip;
    ip = WiFi.localIP().toString();
    return ip.c_str();
}


#endif