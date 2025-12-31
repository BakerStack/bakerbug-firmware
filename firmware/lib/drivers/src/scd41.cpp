#include "co2.hpp"

#include <Wire.h>
#include <SensirionI2cScd4x.h>

static SensirionI2cScd4x sensor;

static uint16_t last_co2 = 0;
static float last_temp = 0.0f;
static float last_rh   = 0.0f;

void co2_init(void) {
  Wire.begin();
  delay(50);

  sensor.begin(Wire, SCD41_I2C_ADDR_62);

  // Clean state (enligt Sensirion-rekommendation)
  sensor.wakeUp();
  delay(20);
  sensor.stopPeriodicMeasurement();
  delay(500);
  sensor.reinit();
  delay(20);

  sensor.startPeriodicMeasurement();
  Serial.println("CO2: SCD41 periodic measurement started");
}

bool co2_measure(void) {
  static unsigned long lastPoll = 0;
  if (millis() - lastPoll < 5000) {
    return false;
  }
  lastPoll = millis();

  bool dataReady = false;
  int16_t err = sensor.getDataReadyStatus(dataReady);
  if (err != 0 || !dataReady) {
    return false;
  }

  uint16_t co2 = 0;
  float t = 0.0f;
  float rh = 0.0f;

  err = sensor.readMeasurement(co2, t, rh);
  if (err != 0) {
    return false;
  }

  /* tillåt warm-up, men markera */
  last_co2  = co2;
  last_temp = t;
  last_rh   = rh;

  return true;
}


uint16_t co2_ppm(void) {
  return last_co2;
}

float co2_temperature(void) {
  return last_temp;
}

float co2_humidity(void) {
  return last_rh;
}

