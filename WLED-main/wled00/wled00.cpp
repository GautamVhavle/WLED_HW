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

#include <Arduino.h>
#include <shadow.pb.h>

#include<certs.h>
#ifndef DEVICE_NAME 
#error "Please include the correct certs.h file."
#endif
#include <WiFiClientSecure.h>
#include "wled.h"

// NOTE: Import everything else before Golain.h
#include "Golain.h"

Golain golain(GOLAIN_CERT_CA, GOLAIN_DEVICE_CERT, GOLAIN_PRIVATE_KEY);

void onDeviceShadowUpdate(Shadow current, Shadow last) {
    Serial.println("Device shadow updated");

    // Control the LED based on the shadow state
    if (current.on) {
      Serial.println("Toggled LED");
    } else {
      Serial.println("Toggled LED");
    }
    // Serial output char data type given from the console
    // if(current.has_macAddr){
    //   Serial.println(current.macAddr);
    // }
}


void setup() {
  Serial.begin(115200);

  WiFi.begin("Quoppo Ventures ", "Quopeupp");

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
  golain.loop();
  WLED::instance().loop();

  if (millis() % 5000 == 0) {
      // GlobalShadow.buttonPressedCount = 30;
      golain.updateShadow();
    }

  delay(1000);
}