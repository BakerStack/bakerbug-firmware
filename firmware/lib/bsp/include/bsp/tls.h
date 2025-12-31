 
#pragma once
#include <Client.h>

struct tls_trust_bundle
{
    const char* ca_pem; // PEM-encoded CA certificate(s)
    const char* name;  // Internal name for the trust bundle
    const char* host; // Name or IP address of the server
};


bool bsp_tls_begin(const tls_trust_bundle* trust);
Client* bsp_tls_client();