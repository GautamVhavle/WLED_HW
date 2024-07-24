//integration

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

#include "wled.h"

#include "certs.h"
#ifndef DEVICE_NAME 
#error "Please include the correct certs.h file."
#endif

#define LED_BUILTIN 2


#include "shadow.pb.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HardwareSerial.h>

#include <pbhelper.h>

// NOTE: Import everything else before Golain.h
#include "Golain.h"




Golain golain(DEVICE_NAME, ROOT_TOPIC, GOLAIN_CERT_CA, GOLAIN_DEVICE_CERT, GOLAIN_PRIVATE_KEY);


void onDeviceShadowUpdate(Shadow current, Shadow last) {
    Serial.println("Device shadow updated");

    // Control the LED based on the shadow state
    if (current.ledState) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        Serial.println("LED is ON");
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        Serial.println("LED is OFF");
    }

    if(current.has_macAddr){
      Serial.println(current.macAddr);
    }

}

void setup() {
    Serial.begin(115200);

    // Fill in your Wi-Fi credentials here to connect to the internet:
    WiFi.begin("Quoppo Ventures ", "Quopeupp");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to WiFi");
    Serial.print("IP address:\t");
    IPAddress myIP = WiFi.localIP();
    Serial.println(myIP);

    golain.setDeviceShadowCallback(&onDeviceShadowUpdate);

    golain.begin();
    pinMode(LED_BUILTIN, OUTPUT);
    
    WLED::instance().setup();

}

void loop() {

    // Call this function to keep the connection alive.
    golain.loop();

    Shadow current;
    String dest;

    if (millis() % 5000 == 0) {
        GlobalShadow.buttonPress = 1;
        golain.updateShadow();
    }
    Serial.println("Entering function");
    toggleOnOff();
}
