#include "main.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Booting");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Blynk.config(auth, server, port);
    Blynk.connect();
    timer.setInterval(10L, app_main);

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

    pixels.begin();
    pixels.clear();
    ShowColorToKnob(0, 0, 0);

    //Reset button to off
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V2, 0);
}

void loop()
{
    timer.run();
    ArduinoOTA.handle();
    if (Blynk.connected())
    {
        Blynk.run();
    }
    else if (ReCnctFlag == 0)
    {
        ReCnctFlag = 1;
        Serial.println("Starting reconnection timer in 5 seconds...");
        timer.setTimeout(5000L, []() {
            ReCnctFlag = 0;
            ReCnctCount++;
            Serial.print("Attempting reconnection #");
            Serial.println(ReCnctCount);
            Blynk.connect();
        });
    }
}

void app_main()
{
    switch (status)
    {
    case DONOTHING:
        break;
    case POWERON:
        ShowColorToKnob(red, green, blue);
        break;
    case POWEROFF:
        ShowColorToKnob(0, 0, 0);
        break;
    case RGB:
        ShowColorToKnob(red, green, blue);
        break;
    default:
        break;
    }
    status = DONOTHING;
}

BLYNK_CONNECTED()
{
    Serial.println("Connected");
    //Blynk.syncAll();
    ReCnctCount = 0;
}

BLYNK_WRITE(V0)
{
    if (param.asInt())
    {
        Serial.println("Power ON");
        VirtualTerminal((char *)"Power ON");
        status = POWERON;
        power = POWERON;
        if (red == 0 && green == 0 && blue == 0)
        {
            red = 125;
            green = 125;
            blue = 125;
        }
    }
    else
    {
        Serial.println("Power OFF");
        VirtualTerminal((char *)"Power OFF");
        status = POWEROFF;
        power = POWEROFF;
    }
}

BLYNK_WRITE(V1)
{
    if (String("reboot") == param.asStr())
    {
        VirtualTerminal((char *)"Device will be rebooted shortly");
        delay(200);
        ESP.restart();
    }
    else if (String("reset") == param.asStr())
    {
        VirtualTerminal((char *)"Device will be restarted shortly");
        delay(200);
        ESP.reset();
    }
    else if (String("clear") == param.asStr())
    {
        terminal.clear();
    }
    else if (String("command") == param.asStr())
    {
        terminal.clear();
        terminal.println("-------cmd-------");
        terminal.print("reboot, reset, clear");
        terminal.flush();
    }
    else
    {
        VirtualTerminal((char *)"Not existing command");
    }
}

BLYNK_WRITE(V3)
{
    if (power == POWERON)
    {
        VirtualTerminal((char *)"RGB color is set");
        red = param[0].asInt();
        green = param[1].asInt();
        blue = param[2].asInt();
        if (red == 255 && green == 255 && blue == 255)
        {
            red = 125;
            green = 125;
            blue = 125;
        }
        status = RGB;
    }
    else
    {
        VirtualTerminal((char *)"RGB color is not set because device is off");
    }
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
