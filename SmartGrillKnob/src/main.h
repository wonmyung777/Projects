
#ifndef _MAIN_H_
#define _MAIN_H_

#define BLYNK_PRINT Serial

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "BlynkSimpleEsp8266.h"
#include "Adafruit_NeoPixel.h"

#define CONNECTION_DELAY 5000
#define LED_PIN D7
#define NUM_OF_PIXELS 16

Adafruit_NeoPixel pixels(NUM_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

char auth[] = "NcvvHwbtBNlKj2J75knyR7fEdhT6Z2HT";
char ssid[] = "Happyhouse";
char pass[] = "01027643122";

unsigned long lastConnectionAttempt = millis();

BlynkTimer timer;

void app_main();

#endif //_MAIN_H_