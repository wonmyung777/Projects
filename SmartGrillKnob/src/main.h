
#ifndef _MAIN_H_
#define _MAIN_H_

#define BLYNK_PRINT Serial

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "BlynkSimpleEsp8266.h"
#include "Adafruit_NeoPixel.h"

char auth[] = "NcvvHwbtBNlKj2J75knyR7fEdhT6Z2HT";
char ssid[] = "KT_GiGA_2G_Wave2_3413";
char pass[] = "kzcfdc6664";
char server[] = "blynk-cloud.com";

int port = 8080;
int ReCnctFlag, ReCnctCount;

BlynkTimer timer;

void app_main();

#endif //_MAIN_H_