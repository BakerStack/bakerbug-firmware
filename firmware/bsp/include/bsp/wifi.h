// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#pragma once
class Client;   // forward declaration, no Arduino dependency
bool bsp_wifi_connect(const char* ssid, const char* password);
bool bsp_wifi_is_connected();
void bsp_wifi_disconnect();
const char* bsp_wifi_ip();
Client* bsp_tcp_client();