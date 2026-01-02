# MQTT over TLS verification (ESP8266 + test.mosquitto.org)

This document verifies that the firmware stack supports
fully validated TLS + MQTT.

It covers:
- WiFi
- NTP time synchronisation
- X.509 CA validation
- TLS handshake
- MQTT publish and subscribe

This is the baseline security check for all devices.

## 1. Select application

Applications live under:

    applications/examples/<name>/src/main.cpp

Select one via environment variable:

    export BOS_APPLICATION=applications/examples/mqtt_basic_tls

## 2. Create development environment

Create your personal secrets file:

    cp secrets-template.sh secrets-yourname.sh

Edit it and set WiFi, MQTT broker, and any private values.

Build the application 

    make dev

## 3. Start MQTT listener on Mac

Install mosquitto if needed:

    brew install mosquitto

Then:

    cd certs
    mosquitto_sub -h test.mosquitto.org -p 8883 \
    --cafile mosquitto_ca.pem \
    -t 'bakerbug/#' -v

## 4. Flash and open serial

    make upload
    make serial

You should see MQTT connected and messages.

## 5. Send a message

    mosquitto_pub -h test.mosquitto.org -p 8883 \
    --cafile mosquitto_ca.pem \
    -t bakerbug/cmd \
    -m "hello"
