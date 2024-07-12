/*
 * WLED Arduino IDE compatibility file.
 * 
 * Where has everything gone?
 * 
 * In April 2020, the project's structure underwent a major change. 
 * Global variables are now found in file "wled.h"
 * Global function declarations are found in "fcn_declare.h"
 * 
 * Usermod compatibility: Existing wled06_usermod.ino mods should continue to work. Delete usermod.cpp.
 * New usermods should use usermod.cpp instead.
 */

#include "certs.h"
#ifndef DEVICE_NAME 
#error "Please include the correct certs.h file."
#endif
#include <WiFiClientSecure.h>
#include "shadow.pb.h"

#include "wled.h"

// NOTE: Import everything else before Golain.h
#include "Golain.h"

Golain golain(DEVICE_NAME, ROOT_TOPIC, GOLAIN_CERT_CA, GOLAIN_DEVICE_CERT, GOLAIN_PRIVATE_KEY);

void onDeviceShadowUpdate(Shadow current, Shadow last) {
    Serial.println("Device shadow updated");
}

void setup() {
    Serial.begin(115200);

    WiFi.begin("<SSID>", "<password>");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to WiFi");

    golain.setDeviceShadowCallback(&onDeviceShadowUpdate);

    golain.begin();

    WLED::instance().setup();
  }

void loop() {
  WLED::instance().loop();

  golain.loop();

  if (millis() % 5000 == 0) {
      GlobalShadow.buttonPressedCount = 30;
      golain.updateShadow();
  }

  delay(1000);
}
