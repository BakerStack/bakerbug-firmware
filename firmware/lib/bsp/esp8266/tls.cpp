// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#ifdef CONFIG_ARCH_ESP8266

#include <WiFiClientSecureBearSSL.h>
#include <bsp/tls.h>
#include <bsp/log.h>

static BearSSL::WiFiClientSecure tls_client;
static std::unique_ptr<BearSSL::X509List> trust_anchor;

bool bsp_tls_begin(const tls_trust_bundle* trust)
{
    if (!trust || !trust->ca_pem) {
        log_printf(LOG_LEVEL_ERROR, "No TLS trust bundle provided, skipping TLS setup");
        return false;
    }
    log_printf(LOG_LEVEL_DEBUG, "Beginning TLS named [%s]", trust->name ? trust->name : "");
     log_printf(LOG_LEVEL_DEBUG, "Beginning TLS to host [%s]", trust->host ? trust->host : "");
    log_printf(LOG_LEVEL_DEBUG, "Beginning TLS with [%s]", trust->ca_pem ? trust->ca_pem : "");
    
    trust_anchor.reset(new BearSSL::X509List(trust->ca_pem));
    tls_client.setTrustAnchors(trust_anchor.get());
    log_printf(LOG_LEVEL_DEBUG, "TLS trust anchors set [%s]", trust->name ? trust->name : "");
    
    return true;
}   
Client* bsp_tls_client()
{
    log_printf(LOG_LEVEL_DEBUG, "Fetching TLS client");    
    return &tls_client;
}
    
#endif