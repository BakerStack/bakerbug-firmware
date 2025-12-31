#include <secrets/mqtt_credentials.h>

const char *MQTT_BROKER_HOST = BB_MQTT_BROKER_HOST;
const int MQTT_BROKER_PORT_NR = atoi(BB_MQTT_BROKER_PORT);
const char *MQTT_USER_STR = BB_MQTT_USER;
const char *MQTT_PASS_STR = BB_MQTT_PASS;
 

#ifdef BB_MQTT_USE_TLS

#include  BB_MQTT_BROKER_CA_PEM

#endif // BB_MQTT_USE_TLS