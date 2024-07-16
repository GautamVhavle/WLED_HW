#include <Arduino.h>
#include "wled.h"

#include "certs.h"

#ifndef DEVICE_NAME 
#error "Please include the correct certs.h file."
#endif

#define LED_BUILTIN 2
#include "shadow.pb.h"


#include <WiFi.h>
#include <WiFiClientSecure.h>

// NOTE: Import everything else before Golain.h
#include "Golain.h"




Golain golain(DEVICE_NAME, ROOT_TOPIC, GOLAIN_CERT_CA, GOLAIN_DEVICE_CERT, GOLAIN_PRIVATE_KEY);

void onDeviceShadowUpdate(Shadow current, Shadow last) {
    Serial.println("Device shadow updated");

    // Control the LED based on the shadow state
    if (current.on) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        Serial.println("LED is ON");

    } else {
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        Serial.println("LED is OFF");
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

    golain.setDeviceShadowCallback(&onDeviceShadowUpdate);

    golain.begin();
    WLED::instance().setup();


    pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
    // Call this function to keep the connection alive.
    golain.loop();
    WLED::instance().loop();

    if (millis() % 5000 == 0) {
        GlobalShadow.buttonPressedCount = 10;
        golain.updateShadow();
    }
    delay(1000);
}