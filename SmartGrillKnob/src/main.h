
#ifndef _MAIN_H_
#define _MAIN_H_

#define BLYNK_PRINT Serial

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "BlynkSimpleEsp8266.h"
#include "Adafruit_NeoPixel.h"
#include "ESP8266mDNS.h"
#include "WiFiUdp.h"
#include "ArduinoOTA.h"
#include <stdio.h>

#define CONNECTION_DELAY 5000
#define LED_PIN D7
#define NUM_OF_PIXELS 16

enum
{
    DONOTHING,
    POWERON,
    POWEROFF,
    RGB,
};

Adafruit_NeoPixel pixels(NUM_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
WidgetTerminal terminal(V1);

char auth[] = "NcvvHwbtBNlKj2J75knyR7fEdhT6Z2HT";
// char ssid[] = "Happyhouse";
// char pass[] = "01027643122";
char ssid[] = "KT_GiGA_2G_Wave2_3413";
char pass[] = "kzcfdc6664";
char server[] = "blynk-cloud.com";

uint8_t status, red, green, blue;
uint8_t power = POWEROFF;
int port = 8080;
unsigned long lastConnectionAttempt = millis();

BlynkTimer timer;

void app_main();
void VirtualTerminal(char *msg);
void ShowColorToKnob(uint8_t red, uint8_t green, uint8_t blue);

#endif //_MAIN_H_
