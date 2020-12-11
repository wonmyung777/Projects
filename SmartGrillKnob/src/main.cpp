#include "main.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Booting");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Blynk.config(auth, server, port);
    Blynk.connect();

    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // Port defaults to 8266
    ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname("My8266");

    // No authentication by default
    ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
            type = "sketch";
        }
        else
        { // U_SPIFFS
            type = "filesystem";
        }

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);

        if (error == OTA_AUTH_ERROR)
        {
            Serial.println("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            Serial.println("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            Serial.println("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            Serial.println("Receive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
            Serial.println("End Failed");
        }
    });

    ArduinoOTA.begin();

    terminal.clear();
    VirtualTerminal((char *)"Device is started");
    VirtualTerminal((char *)"IP address");
    terminal.print(WiFi.localIP());
    terminal.flush();
    timer.setInterval(1L, app_main);

    pixels.begin();
    pixels.clear();
    ShowColorToKnob(0, 0, 0);

    //Reset button to off
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V2, 0);
}

void loop()
{
    // check WiFi connection:
    if (WiFi.status() != WL_CONNECTED)
    {
        // (optional) "offline" part of code
        
        // check delay:
        if ((millis() - lastConnectionAttempt) >= CONNECTION_DELAY)
        {
            lastConnectionAttempt = millis();

            // attempt to connect to Wifi network:
            if (pass && strlen(pass))
            {
                WiFi.begin((char *)ssid, (char *)pass);
            }
            else
            {
                WiFi.begin((char *)ssid);
            }
        }
    }
    else
    {
        Blynk.run();
        ArduinoOTA.handle();
    }
}

void app_main()
{
}

BLYNK_CONNECTED()
{
    Serial.println("Connected");
    // VirtualTerminal((char *)"Connected");
    //Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
    if (param.asInt())
    {
        Serial.println("Power ON");
        VirtualTerminal((char *)"Power ON");
        ShowColorToKnob(255, 255, 255);
        power = true;
    }
    else
    {
        Serial.println("Power OFF");
        VirtualTerminal((char *)"Power OFF");
        ShowColorToKnob(0, 0, 0);
        power = false;
    }
}

// BLYNK_WRITE(V2)
// {
//     if (param.asInt())
//     {
//         ESP.restart();
//     }
// }

BLYNK_WRITE(V3)
{
    if (power)
        ShowColorToKnob(param[0].asInt(), param[1].asInt(), param[2].asInt());
}

void VirtualTerminal(char *msg)
{
    terminal.print(msg);
    terminal.flush();
}

void ShowColorToKnob(uint8_t red, uint8_t green, uint8_t blue)
{
    for (int i = 0; i < NUM_OF_PIXELS; i++)
    {
        pixels.setPixelColor(i, red, green, blue);
    }
    pixels.show();
}
