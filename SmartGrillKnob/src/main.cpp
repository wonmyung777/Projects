
#include "main.h"

void setup()
{
    Serial.begin(115200);
    pixels.begin();
    Blynk.begin(auth, ssid, pass);
    pixels.clear();

    timer.setInterval(1L, app_main);

    // pixels.setPixelColor(17, 255, 255, 255);

    // pixels.show(); // Send the updated pixel colors to the hardware.
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
    }
}

void app_main()
{
}

BLYNK_CONNECTED()
{
    Serial.println("Connected");
    //Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
    if (param.asInt())
    {
        Serial.println("Power ON");
    }
    else
    {
        Serial.println("Power OFF");
    }
}
