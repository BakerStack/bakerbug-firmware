// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>
#include <ESP8266WiFi.h>
#include <bsp/log.h>
#include <bsp/wifi.h>

#include <cstring>

static WiFiClient wifi;
static WiFiEventHandler s_ap_probe_handler;
static WiFiEventHandler s_ap_join_handler;
static WiFiEventHandler s_ap_leave_handler;

static void wifi_prepare()
{
    WiFi.persistent(false);
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
}

static void wifi_register_ap_events()
{
    s_ap_probe_handler = WiFi.onSoftAPModeProbeRequestReceived(
        [](const WiFiEventSoftAPModeProbeRequestReceived& evt) {
            log_printf(LOG_LEVEL_DEBUG, "AP probe mac=%02X:%02X:%02X:%02X:%02X:%02X rssi=%d",
                       evt.mac[0], evt.mac[1], evt.mac[2], evt.mac[3], evt.mac[4], evt.mac[5],
                       evt.rssi);
        });
    s_ap_join_handler = WiFi.onSoftAPModeStationConnected(
        [](const WiFiEventSoftAPModeStationConnected& evt) {
            log_printf(LOG_LEVEL_INFO,
                       "AP station joined mac=%02X:%02X:%02X:%02X:%02X:%02X aid=%u heap=%u",
                       evt.mac[0], evt.mac[1], evt.mac[2], evt.mac[3], evt.mac[4], evt.mac[5],
                       evt.aid, ESP.getFreeHeap());
        });
    s_ap_leave_handler = WiFi.onSoftAPModeStationDisconnected(
        [](const WiFiEventSoftAPModeStationDisconnected& evt) {
            log_printf(LOG_LEVEL_INFO, "AP station left mac=%02X:%02X:%02X:%02X:%02X:%02X aid=%u",
                       evt.mac[0], evt.mac[1], evt.mac[2], evt.mac[3], evt.mac[4], evt.mac[5],
                       evt.aid);
        });
}

Client* bsp_tcp_client()
{
    return &wifi;
}

bool bsp_wifi_connect(const char* ssid, const char* password)
{
    wifi_prepare();

    if (WiFi.status() == WL_CONNECTED)
    {
        log_printf(LOG_LEVEL_DEBUG, "WiFi already connected, IP=%s", WiFi.localIP().toString().c_str());
        return true;
    }

    log_printf(LOG_LEVEL_INFO, "WiFi STA connecting to '%s'", ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
    {
        delay(250);
        log_printf(LOG_LEVEL_DEBUG, "WiFi STA status=%d", WiFi.status());
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        log_printf(LOG_LEVEL_INFO, "WiFi STA connected, IP=%s", WiFi.localIP().toString().c_str());
        return true;
    }

    log_printf(LOG_LEVEL_ERROR, "WiFi STA connect timeout status=%d", WiFi.status());
    return false;
}

bool bsp_wifi_softap_begin(const char* ssid, const char* password)
{
    wifi_prepare();

    if (ssid == nullptr || ssid[0] == '\0')
    {
        log_printf(LOG_LEVEL_ERROR, "WiFi AP ssid empty");
        return false;
    }

    const size_t pass_len = password != nullptr ? strlen(password) : 0;
    if (pass_len < 8)
    {
        log_printf(LOG_LEVEL_ERROR, "WiFi AP password too short (%u) — need >= 8", pass_len);
        return false;
    }

    log_printf(LOG_LEVEL_INFO, "WiFi AP starting ssid='%s' pass_len=%u", ssid, pass_len);

    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(50);
    WiFi.mode(WIFI_AP);

    // channel 1, visible, max 4 stations — same params that worked in early TEST 2
    if (!WiFi.softAP(ssid, password, 1, 0, 4))
    {
        log_printf(LOG_LEVEL_ERROR, "WiFi.softAP failed");
        return false;
    }

    wifi_register_ap_events();
    delay(100);

    const String ap_ssid = WiFi.softAPSSID();
    log_printf(LOG_LEVEL_INFO, "WiFi AP OK ip=%s ch=%u bssid=%s ssid_verify='%s' stations=%u heap=%u",
               WiFi.softAPIP().toString().c_str(), WiFi.channel(), WiFi.softAPmacAddress().c_str(),
               ap_ssid.c_str(), WiFi.softAPgetStationNum(), ESP.getFreeHeap());
    if (!ap_ssid.equals(ssid))
    {
        log_printf(LOG_LEVEL_WARN, "AP ssid mismatch wanted='%s' got='%s'", ssid, ap_ssid.c_str());
    }
    return true;
}

bool bsp_wifi_is_connected()
{
    const bool ok = (WiFi.status() == WL_CONNECTED);
    log_printf(LOG_LEVEL_DEBUG, "WiFi STA connected=%d", ok);
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

const char* bsp_wifi_softap_ip()
{
    static String ip;
    ip = WiFi.softAPIP().toString();
    return ip.c_str();
}

uint8_t bsp_wifi_softap_station_count()
{
    return WiFi.softAPgetStationNum();
}

void bsp_wifi_print_diag()
{
#ifdef SERIAL_DEBUG
    log_printf(LOG_LEVEL_DEBUG, "WiFi.printDiag() — mode=%d status=%d ch=%u heap=%u", WiFi.getMode(),
               WiFi.status(), WiFi.channel(), ESP.getFreeHeap());
    log_printf(LOG_LEVEL_DEBUG, "AP side: ssid='%s' stations=%u", WiFi.softAPSSID().c_str(),
               WiFi.softAPgetStationNum());
    // printDiag() shows STA config slot — often empty in AP-only mode; not an error.
    WiFi.printDiag(Serial);
#else
    (void)0;
#endif
}
